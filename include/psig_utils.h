/**
 * @file psig_utils.h
 * @author Adrien Chevrier
 * 
 * @brief Signal handling utilities for thread management.
 *
 * This file defines functions to handle POSIX signals (such as SIGINT).
 * The signal handler also ensures releasing IPC resources if necessary,
 * like on killing signals.
 *
 * @see psig_utils.c
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

#ifndef PSIG_UTILS_H
#define PSIG_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>
#include <stdbool.h>

#include "ipc_elements.h"

/**
 * @brief Installs the signal handler.
 * 
 * This function installs the custom signal handler.
 * For instance, when the SIGINT signal is triggered, the @c on_signal handler will be called.
 *
 * @note Only SIGINT is installed for now.
 * @note Not sure if calling it one time for the whole application is enough.
 *
 * @todo Figure out if calling it only one time at the begining,
 * like in @c init_board(), is sufficient.
 */
void psig_install_handler(void);

/**
 * @brief Checks if a termination signal has been received.
 * 
 * This function checks if the program has received a SIGINT signal (Ctrl+C).
 * It is typically used in a loop to check if the program should exit.
 * 
 * @return @c true if SIGINT was received, otherwise @c false.
 */
bool psig_kill_requested(void);

#ifdef __cplusplus
}
#endif

#endif // PSIG_UTILS_H
