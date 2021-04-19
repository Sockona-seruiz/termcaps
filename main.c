#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

static volatile unsigned char term_size_updated;

static void term_resize() {
    term_size_updated = 1;
}

static void term_get_size(int *cols, int *rows) {
    struct winsize ws;

    /* get screen dimensions from (pseudo) tty ioctl */
    if (ioctl(0, TIOCGWINSZ, &ws) == 0) {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
    } else {
        *cols = *rows = -1;
    }
}

int main() {
    struct sigaction sig;
    int cols, rows;

    /* set up terminal resize callback */
    sig.sa_handler = term_resize;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;
    sigaction(SIGWINCH, &sig, NULL);

    term_size_updated = 1;
    for (;;) {
        if (term_size_updated) {
            term_size_updated = 0;
            term_get_size(&cols, &rows);
            fprintf(stderr, "term_resize: cols=%d, rows=%d\n", cols, rows);
        }
        sleep(1);
    }
    return 0;
}
