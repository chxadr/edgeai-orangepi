#ifndef PSIG_UTILS_H
#define PSIG_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>
#include <stdbool.h>

#include "ipc_elements.h"

void psig_install_handler(void);
bool psig_kill_requested(void);

#ifdef __cplusplus
}
#endif

#endif // PSIG_UTILS_H
