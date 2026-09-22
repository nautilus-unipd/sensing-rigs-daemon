#include "fnct_runtime.h"

/* fnct_runtime.h pulls in constants.h (DAEMON_PATH*, MAX_COMMAND_SIZE,
 * SET_FLAG / FLAG_ERR_CAMS, ROOT_PATH, thresholds, ...). The headers below are
 * additionally required by the new sequence-counter code and are NOT included
 * by fnct_runtime.h, so we include them here. */
#include <time.h>     /* time()                       */
#include <dirent.h>   /* opendir / readdir / closedir */
#include <pthread.h>  /* pthread_mutex_*              */
#include <string.h>   /* strlen                       */
#include <unistd.h>   /* fork, exec, usleep, sleep    */
#include <signal.h>   /* kill, SIGUSR1, SIGTERM       */
#include <fcntl.h>    /* open                         */
#include <sys/wait.h> /* waitpid                      */
#ifdef __linux__
#include <sys/prctl.h> /* prctl, PR_SET_PDEATHSIG      */
#endif

/* ============================================================================
 *  Capture command
 *  ---------------------------------------------------------------------------
 *  Only TWO changes vs your original (working) command:
 *      - "--timestamp" REMOVED: we now put the filename in the -o path ourselves
 *        (sequence + timestamp), so ordering no longer depends on the clock.
 *      - "-o %s%s%s" (a directory prefix) became "-o %s" (the full file path we
 *        build in shoot()).
 *  Everything else (-v 0, --zsl, resolution, --immediate) is unchanged, so the
 *  capture behaves exactly as before.
 *
 *  Format arguments: (int camera, const char* full_output_path)  -> TWO of them.
 *
 *  IMPORTANT — keep exactly ONE definition of PHOTO_SHOOT_COMMAND in the whole
 *  project, and make sure the string in effect has exactly TWO specifiers
 *  (%d and %s). If it is defined in constants.c, put the NEW string THERE and
 *  DELETE this block (two definitions = duplicate-symbol link error). If it
 *  lived at the top of this file, keep it here.
 * ==========================================================================*/
// const char* const PHOTO_SHOOT_COMMAND =
//     "rpicam-still -v 0 --zsl --width 1920 --height 1080 --camera %d --immediate -o %s";

/* ============================================================================
 *  Per-camera capture sequence counters
 *  ---------------------------------------------------------------------------
 *  g_seq[0] -> camera 0 -> rx directory
 *  g_seq[1] -> camera 1 -> lx directory
 *  Seeded once at startup from the highest file already on disk, so the count
 *  survives restarts and never resets. Guarded by a mutex because the two
 *  cameras are shot from separate threads.
 * ==========================================================================*/
static unsigned long g_seq[2] = {0, 0};

/* Highest leading number among files named "NNN_...".jpg or "NNN.jpg" in dir.
 * Returns 0 if the directory is empty or cannot be opened. */
static unsigned long highest_seq_in(const char *dir)
{
    DIR *d = opendir(dir);
    if (d == NULL)
    {
        return 0;
    }

    unsigned long hi = 0;
    struct dirent *e;
    while ((e = readdir(d)) != NULL)
    {
        char *endp = NULL;
        unsigned long v = strtoul(e->d_name, &endp, 10);
        /* accept only "<digits>_..." or "<digits>." (our naming schemes) */
        if (endp != e->d_name && (*endp == '_' || *endp == '.'))
        {
            if (v > hi)
            {
                hi = v;
            }
        }
    }

    closedir(d);
    return hi;
}

/* Call ONCE at startup, AFTER the capture directories have been created
 * (e.g. at the end of daemon_create(), just before arming the timer). */
void init_seq_counters(void)
{
    char dir[512];

    snprintf(dir, sizeof(dir), "%s%s%s", DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_RX);
    g_seq[0] = highest_seq_in(dir) + 1;

    snprintf(dir, sizeof(dir), "%s%s%s", DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_LX);
    g_seq[1] = highest_seq_in(dir) + 1;
}

/* ============================================================================
 *  Health checks (unchanged)
 * ==========================================================================*/
bool check_space(void)
{
    struct statvfs stat;
    if (statvfs(ROOT_PATH, &stat) != 0)
    {
        return true;
    }
    if ((stat.f_bfree * stat.f_frsize) <= ((unsigned long int)MIN_FREE_SPACE))
    {
        return true;
    }
    return false;
}

bool check_temperature(void)
{
    FILE *temp_file = fopen(FILE_PATH_TEMP, "r");
    if (temp_file == NULL)
    {
        return true;
    }
    uint32_t temp = 0;
    if (fscanf(temp_file, "%u", &temp) == EOF)
    {
        fclose(temp_file);
        return true;
    }
    fclose(temp_file);
    if (temp >= MAX_TEMP)
    {
        return true;
    }
    return false;
}

bool check_voltage(void)
{
    FILE *volt_pipe;
    volt_pipe = popen(VOLT_COMMAND, "r");
    if (volt_pipe == NULL)
    {
        return true;
    }
    char *result = (char *)malloc(MAX_VOLT_BUFF_SIZE * sizeof(char));
    if (result == NULL)
    {
        pclose(volt_pipe);
        return true;
    }
    if (fgets(result, MAX_VOLT_BUFF_SIZE * sizeof(char), volt_pipe) == NULL)
    {
        free(result);
        result = NULL;
        pclose(volt_pipe);
        return true;
    }
    pclose(volt_pipe);
    if ((*(result) != VOLT_CORRECT_1) || (*(result + 1) != VOLT_CORRECT_2))
    {
        free(result);
        result = NULL;
        return true;
    }
    free(result);
    result = NULL;
    return false;
}

/* ============================================================================
 *  Persistent Camera Management (rpicam-still in signal mode)
 *  ---------------------------------------------------------------------------
 *  Processes are spawned once at startup with '-t 0 -s'.
 *  Captures are triggered instantly by sending SIGUSR1.
 * ==========================================================================*/
static pid_t camera_pids[2] = {-1, -1};

int start_camera_processes(void)
{
    for (uint8_t cam = 0; cam < 2; cam++)
    {
        const char *subdir = (cam == 0) ? DAEMON_PATH_RX : DAEMON_PATH_LX;
        char out_dir[512];
        snprintf(out_dir, sizeof(out_dir), "%s%s%s", DAEMON_PATH, DAEMON_PATH_CAP, subdir);

        char cmd[1024];
        snprintf(cmd, sizeof(cmd), PHOTO_SHOOT_COMMAND, cam, g_seq[cam], out_dir);

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork rpicam-still");
            SET_FLAG(FLAG_ERR_CAMS);
            stop_camera_processes();
            return EXIT_FAILURE;
        }

        if (pid == 0)
        {
            // CHILD PROCESS
#ifdef __linux__
            // Terminate child if parent daemon exits unexpectedly
            prctl(PR_SET_PDEATHSIG, SIGTERM);
#endif
            // Unblock signals in child so rpicam-still can receive SIGUSR1
            sigset_t empty_mask;
            sigemptyset(&empty_mask);
            sigprocmask(SIG_SETMASK, &empty_mask, NULL);

            // Restore default signal handlers
            signal(SIGUSR1, SIG_DFL);
            signal(SIGTERM, SIG_DFL);
            signal(SIGINT, SIG_DFL);
            signal(SIGALRM, SIG_DFL);
            signal(SIGHUP, SIG_DFL);

            // Redirect stdout and stderr to /dev/null
            int devnull = open(REDIRECT_FILE, O_WRONLY);
            if (devnull >= 0)
            {
                dup2(devnull, STDOUT_FILENO);
                dup2(devnull, STDERR_FILENO);
                close(devnull);
            }

            execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
            _exit(127);
        }

        camera_pids[cam] = pid;
    }

    // Allow camera hardware and pipeline 3A algorithms to initialize
    sleep(2);

    // Verify both camera processes are alive
    for (uint8_t cam = 0; cam < 2; cam++)
    {
        int status = 0;
        pid_t res = waitpid(camera_pids[cam], &status, WNOHANG);
        if (res != 0)
        {
            SET_FLAG(FLAG_ERR_CAMS);
            stop_camera_processes();
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void stop_camera_processes(void)
{
    for (uint8_t cam = 0; cam < 2; cam++)
    {
        if (camera_pids[cam] > 0)
        {
            kill(camera_pids[cam], SIGTERM);

            int status = 0;
            int exited = 0;
            for (int i = 0; i < 5; i++)
            {
                if (waitpid(camera_pids[cam], &status, WNOHANG) != 0)
                {
                    exited = 1;
                    break;
                }
                usleep(100000);
            }
            if (!exited)
            {
                kill(camera_pids[cam], SIGKILL);
                waitpid(camera_pids[cam], &status, 0);
            }
            camera_pids[cam] = -1;
        }
    }
}

int trigger_camera(uint8_t cam)
{
    if ((cam != 0) && (cam != 1))
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return EXIT_FAILURE;
    }

    if (camera_pids[cam] <= 0)
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return EXIT_FAILURE;
    }

    if (kill(camera_pids[cam], SIGUSR1) != 0)
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void *shoot(void *arg)
{
    uint8_t cam = *(uint8_t *)arg;
    if (trigger_camera(cam) != EXIT_SUCCESS)
    {
        SET_FLAG(FLAG_ERR_CAMS);
    }
    return NULL;
}
