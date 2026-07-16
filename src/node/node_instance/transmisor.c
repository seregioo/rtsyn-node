/**
 * @file rtsyn/node/node_instance/transmisor.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for RTSyn Connection Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "rtsyn/node/node_instance/transmisor.h"

rtsyn_node_t *
rtsyn_node_create_transmisor(rtsyn_node_id_t id, uint32_t num_input_ports,
                                     const rtsyn_port_spec_t input_ports[],
                                     uint32_t num_output_ports,
                                     const rtsyn_port_spec_t output_ports[])
{
    return rtsyn_node_create(RTSYN_NODE_TRANSMISOR, id, num_input_ports,
                                     input_ports, num_output_ports, output_ports);
}

void
rtsyn_node_transmisor_executor(void *)
{
}
