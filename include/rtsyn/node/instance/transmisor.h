/**
 * @file rtsyn/node/instance/transmisor.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for RTSyn Connection Node.
 *
 * This node that executes a transmisor to send the runtime data.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_TRANSMISOR_H
#define RTSYN_NODE_TRANSMISOR_H
#include "rtsyn/node.h"

rtsyn_node_t *
rtsyn_node_create_transmisor(const char *module_path, uint32_t num_ports,
                             const rtsyn_abi_port_descriptor_t ports_descriptors[],
                             uint32_t num_states,
                             const rtsyn_abi_state_descriptor_t states_descriptors[],
                             uint32_t num_params,
                             const rtsyn_abi_param_descriptor_t params_descriptors[]);

void
rtsyn_node_transmisor_executor(void *);

#endif /* RTSYN_NODE_TRANSMISOR_H */
