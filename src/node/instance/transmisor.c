/**
 * @file rtsyn/node/instance/transmisor.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for RTSyn Connection Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "rtsyn/node/instance/transmisor.h"

static rtsyn_node_id_t transmisor_id = 0;

rtsyn_node_t *
rtsyn_node_create_transmisor(const char *module_path, uint32_t num_ports,
                             const rtsyn_abi_port_descriptor_t ports_descriptors[],
                             uint32_t num_states,
                             const rtsyn_abi_state_descriptor_t states_descriptors[],
                             uint32_t num_params,
                             const rtsyn_abi_param_descriptor_t params_descriptors[])

{
    rtsyn_node_t *node = rtsyn_node_create(module_path, RTSYN_ABI_NODE_TRANSMISOR, transmisor_id,
                                           num_ports, ports_descriptors, num_states,
                                           states_descriptors, num_params, params_descriptors);
    if (node)
    {
        transmisor_id++;
    }
    return node;
}

void
rtsyn_node_transmisor_executor(void *)
{
}
