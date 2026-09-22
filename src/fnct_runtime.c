#include "fnct_runtime.h"

/* fnct_runtime.h pulls in constants.h (DAEMON_PATH*, MAX_COMMAND_SIZE,
 * SET_FLAG / FLAG_ERR_CAMS, ROOT_PATH, thresholds, ...). The headers below are
 * additionally required by the new sequence-counter code and are NOT included
 * by fnct_runtime.h, so we include them here. */
#include <time.h>       /* time()                       */
#include <dirent.h>     /* opendir / readdir / closedir */
#include <pthread.h>    /* pthread_mutex_*              */
#include <string.h>     /* strlen                       */

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
//const char* const PHOTO_SHOOT_COMMAND =
//    "rpicam-still -v 0 --zsl --width 1920 --height 1080 --camera %d --immediate -o %s";


/* ============================================================================
 *  Per-camera capture sequence counters
 *  ---------------------------------------------------------------------------
 *  g_seq[0] -> camera 0 -> rx directory
 *  g_seq[1] -> camera 1 -> lx directory
 *  Seeded once at startup from the highest file already on disk, so the count
 *  survives restarts and never resets. Guarded by a mutex because the two
 *  cameras are shot from separate threads.
 * ==========================================================================*/
static unsigned long   g_seq[2] = {0, 0};
static pthread_mutex_t seq_lock = PTHREAD_MUTEX_INITIALIZER;

/* Highest leading number among files named "NNN_...".jpg or "NNN.jpg" in dir.
 * Returns 0 if the directory is empty or cannot be opened. */
static unsigned long highest_seq_in(const char* dir)
{
    DIR* d = opendir(dir);
    if(d == NULL)
    {
        return 0;
    }

    unsigned long hi = 0;
    struct dirent* e;
    while((e = readdir(d)) != NULL)
    {
        char* endp = NULL;
        unsigned long v = strtoul(e->d_name, &endp, 10);
        /* accept only "<digits>_..." or "<digits>." (our naming schemes) */
        if(endp != e->d_name && (*endp == '_' || *endp == '.'))
        {
            if(v > hi)
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
    if(statvfs(ROOT_PATH, &stat) != 0)
    {
        return true;
    }
    if((stat.f_bfree * stat.f_frsize) <= ((unsigned long int)MIN_FREE_SPACE))
    {
        return true;
    }
    return false;
}

bool check_temperature(void)
{
    FILE* temp_file = fopen(FILE_PATH_TEMP, "r");
    if(temp_file == NULL)
    {
        return true;
    }
    uint32_t temp = 0;
    if(fscanf(temp_file, "%u", &temp) == EOF)
    {
        fclose(temp_file);
        return true;
    }
    fclose(temp_file);
    if(temp >= MAX_TEMP)
    {
        return true;
    }
    return false;
}

bool check_voltage(void)
{
    FILE* volt_pipe;
    volt_pipe = popen(VOLT_COMMAND, "r");
    if(volt_pipe == NULL)
    {
        return true;
    }
    char* result = (char*)malloc(MAX_VOLT_BUFF_SIZE * sizeof(char));
    if(result == NULL)
    {
        pclose(volt_pipe);
        return true;
    }
    if(fgets(result, MAX_VOLT_BUFF_SIZE * sizeof(char), volt_pipe) == NULL)
    {
        free(result);
        result = NULL;
        pclose(volt_pipe);
        return true;
    }
    pclose(volt_pipe);
    if((*(result) != VOLT_CORRECT_1) || (*(result + 1) != VOLT_CORRECT_2))
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
 *  shoot() — capture one frame from `cam` into its directory.
 *  ---------------------------------------------------------------------------
 *  Filename is "NNNNNN_<unixts>.jpg" (sequence first -> ordering is correct
 *  regardless of the clock; the timestamp is kept only for display).
 *
 *  Error handling is IDENTICAL to the original: FLAG_ERR_CAMS is set only on a
 *  bad camera argument, an allocation/format failure, or popen() returning
 *  NULL. The exit status of rpicam-still is NOT treated as an error (just like
 *  before) — so a non-zero/benign exit code can never kill the daemon.
 * ==========================================================================*/
void* shoot(void* arg)
{
    uint8_t cam = *(uint8_t*)arg;
    if((cam != 0) && (cam != 1))
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    const char* subdir = (cam == 0) ? DAEMON_PATH_RX : DAEMON_PATH_LX;

    /* next sequence number for this camera */
    pthread_mutex_lock(&seq_lock);
    unsigned long seq = g_seq[cam]++;
    pthread_mutex_unlock(&seq_lock);

    /* full output path: <captures>/<rx|lx>/NNNNNN_<unixts>.jpg */
    char out_path[512];
    snprintf(out_path, sizeof(out_path), "%s%s%s%06lu_%ld.jpg",
             DAEMON_PATH, DAEMON_PATH_CAP, subdir, seq, (long)time(NULL));

    /* build the rpicam-still command: (camera, output path).
     * Size the buffer to the actual content so the full path (filename +
     * ".jpg") can never be truncated. The old fixed MAX_COMMAND_SIZE was too
     * small once the filename moved into the -o path. */
    size_t cmd_size = strlen(PHOTO_SHOOT_COMMAND) + strlen(out_path) + 16;
    char* restrict command = (char*)malloc(cmd_size);
    if(command == NULL)
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    if(snprintf(command, cmd_size, PHOTO_SHOOT_COMMAND, cam, out_path) < 0)
    {
        free(command);
        command = NULL;
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    FILE* pipe = popen(command, "r");
    free(command);
    command = NULL;
    if(pipe == NULL)
    {
        SET_FLAG(FLAG_ERR_CAMS);
        return NULL;
    }

    pclose(pipe);   /* blocks until the capture finishes; status ignored, as before */

    return NULL;
}
