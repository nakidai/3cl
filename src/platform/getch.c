#ifdef _WIN32
void getch_init(void) {}
#else
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <termios.h>
#include <unistd.h>

static struct termios default_state = {0};

int getch(void)
{
    int ch = 0;
    struct termios old = {0};
    fflush(stdout);

    tcgetattr(0, &old);
    old.c_lflag &= ~ICANON;

    tcsetattr(0, TCSANOW, &old);
    ch = getchar();
    tcsetattr(0, TCSADRAIN, &default_state);

    return ch;
}

static void get_reset_terminal_state(void)
{
    tcsetattr(0, TCSANOW, &default_state);
}

static noreturn void get_reset_terminal_state_handler(int sig)
{
    get_reset_terminal_state();
    _Exit(sig);
}

void getch_init(void)
{
    tcgetattr(0, &default_state);

    atexit(get_reset_terminal_state);

    signal(SIGINT,  get_reset_terminal_state_handler);
    signal(SIGABRT, get_reset_terminal_state_handler);
    signal(SIGFPE,  get_reset_terminal_state_handler);
    signal(SIGILL,  get_reset_terminal_state_handler);
    signal(SIGSEGV, get_reset_terminal_state_handler);
    signal(SIGTERM, get_reset_terminal_state_handler);
}
#endif /* _WIN32 */
