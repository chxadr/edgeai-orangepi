#include "psig_utils.h"

static volatile sig_atomic_t sigint_received = 0;

static void on_signal(int signal)
{
    if (signal == SIGINT) {
        sigint_received = 1;
        ipc_release();
    }
}

void psig_install_handler(void)
{
    signal(SIGINT, on_signal);
}

bool psig_kill_requested(void)
{
    return sigint_received != 0;
}
