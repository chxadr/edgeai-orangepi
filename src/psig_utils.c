/**
 * @file psig_utils.c
 * @author Adrien Chevrier
 * 
 * @brief Implementation file for the header @c psig_utils.h .
 *
 * @see psig_utils.h
 * 
 * @version 0.1
 * @date 2025-07-15
 *
 * @copyright Copyright (c) 2025 Adrien Chevrier
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "psig_utils.h"

static volatile sig_atomic_t sigint_received = 0;

/**
 * @brief Signal handler.
 * 
 * This function is called when a POSIX signal is received. It sets the flag to 
 * indicate that the signal has been received and performs necessary cleanup, 
 * such as releasing IPC resources.
 * 
 * @param signal The signal number that triggered the handler. 
 *               Currently only handles SIGINT (Ctrl+C).
 */
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
