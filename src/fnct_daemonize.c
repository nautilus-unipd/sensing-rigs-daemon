#include "fnct_daemonize.h"

FILE *open_pid_file(bool flag_write)
{
    char *restrict path_pid = (char *)malloc(sizeof(char) * (strlen(DAEMON_PATH) + strlen(DAEMON_PATH_PID) + 1));
    if (path_pid == NULL)
    {
        return NULL;
    }

    path_pid[0] = '\0';
    strcpy(path_pid, DAEMON_PATH);
    strcat(path_pid, DAEMON_PATH_PID);

    FILE *pid_file = NULL;

    if (flag_write)
    {
        pid_file = open_file(path_pid, FM_W);
    }
    else
    {
        pid_file = open_file(path_pid, FM_R);
    }

    free(path_pid);
    path_pid = NULL;

    return pid_file;
}

/*
 * SYSTEMD-COMPATIBLE VERSION:
 * - PID file NON viene usato per bloccare l’avvio
 * - serve solo come informazione/debug
 */
bool check_pid_file(void)
{
    FILE *restrict pid_file = open_pid_file(false);

    if (pid_file == NULL)
    {
        return false;
    }

    int c = fgetc(pid_file);
    fclose(pid_file);
    pid_file = NULL;

    /*
     * FIX CRITICO:
     * se file esiste ma è vuoto o corrotto NON bloccare il daemon
     */
    if (c == EOF || c == '\0')
    {
        return false;
    }

    /*
     * NON usiamo più errno = EEXIST come segnale di blocco.
     * systemd gestisce già la singola istanza.
     */
    return false;
}

bool create_pid_file(void)
{
    FILE *restrict pid_file = open_pid_file(true);

    if (pid_file == NULL)
    {
        return false;
    }

    /*
     * FIX IMPORTANTE:
     * scrittura PID ma senza logica di locking (systemd gestisce concorrenza)
     */
    if (fprintf(pid_file, "%d", getpid()) <= 0)
    {
        fclose(pid_file);
        pid_file = NULL;
        return false;
    }

    fflush(pid_file);

    fclose(pid_file);
    pid_file = NULL;

    return true;
}

/*
 * SAFE CLEANUP:
 * - non tenta di fare unlock (inutile senza locking reale)
 * - evita write su file descriptor chiuso
 */
void close_pid_file(void)
{
    FILE *restrict pid_file = open_pid_file(true);

    if (pid_file == NULL)
    {
        return;
    }

    /*
     * FIX:
     * NON usare flock unlock qui (non è più usato come meccanismo di esclusione)
     */

    fflush(pid_file);

    /*
     * opzionale: segnale di shutdown
     */
    ftruncate(fileno(pid_file), 0);

    fclose(pid_file);
    pid_file = NULL;
}

/*
 * MAIN DAEMON INIT (SYSTEMD VERSION)
 */
int daemon_create(void)
{
    // =========================
    // 1. INIT DIRECTORIES
    // =========================

    if (mkdir(DAEMON_PATH, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_log[256];
    snprintf(path_log, sizeof(path_log), "%s%s", DAEMON_PATH, DAEMON_PATH_LOG);

    if (mkdir(path_log, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_img[256];
    snprintf(path_img, sizeof(path_img), "%s%s", DAEMON_PATH, DAEMON_PATH_CAP);

    if (mkdir(path_img, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_img_rx[256];
    snprintf(path_img_rx, sizeof(path_img_rx), "%s%s%s",
             DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_RX);

    if (mkdir(path_img_rx, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }

    char path_img_lx[256];
    snprintf(path_img_lx, sizeof(path_img_lx), "%s%s%s",
             DAEMON_PATH, DAEMON_PATH_CAP, DAEMON_PATH_LX);

    if (mkdir(path_img_lx, 0755) != 0 && errno != EEXIST)
    {
        perror(ERR_DAEMON_CREATE);
        return EXIT_FAILURE;
    }
    /// align the timing
    init_seq_counters();
    // =========================
    // 2. LOGGING INIT
    // =========================

    if (init_logging() != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    if (append_log(INFO, MSG_DAEMON_STARTED) != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

    // =========================
    // 3. SIGNAL HANDLER
    // =========================

    if (init_sig_handler() != EXIT_SUCCESS)
    {
        append_log(ERROR, ERR_INIT_SH);
        return EXIT_FAILURE;
    }

    // =========================
    // 4. CAMERA PROCESSES INIT (OPTION A: SIGNAL MODE)
    // =========================

    if (start_camera_processes() != EXIT_SUCCESS)
    {
        append_log(ERROR, ERR_CAMERAS);
        return EXIT_FAILURE;
    }

    // =========================
    // 5. TIMER SETUP (5 FPS = 200 ms interval)
    // =========================

    struct itimerval timer_shoot;

    // Configurazione timer per acquisizione rapida a 5 FPS
    // 1 secondo / 5 scatti = 0.2 secondi di intervallo
    // => tv_sec = 0 e tv_usec = 200.000

    timer_shoot.it_value.tv_sec = 0;
    timer_shoot.it_value.tv_usec = 200000;

    timer_shoot.it_interval.tv_sec = 0;
    timer_shoot.it_interval.tv_usec = 200000;

    setitimer(ITIMER_REAL, &timer_shoot, NULL);

    return EXIT_SUCCESS;
}

/*
 * CLEAN EXIT (SYSTEMD SAFE)
 */
void daemon_terminate(bool close_pid)
{
    // Cleanly stop persistent camera processes first
    stop_camera_processes();

    if (close_pid)
    {
        append_log(INFO, MSG_DAEMON_KILLED);
        close_pid_file();
    }

    terminate_logging();

    close_all_fds();

    return;
}

