/**
 * @file rtsyn/node/node_instance/plugin.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for RTSyn Plugin Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "rtsyn/node/node_instance/plugin.h"

void
rtsyn_node_plugin_executor(void *)
{
}

rtsyn_node_t *
rtsyn_node_create_plugin(rtsyn_node_id_t id, uint32_t num_input_ports,
                                 const rtsyn_port_spec_t input_ports[], uint32_t num_output_ports,
                                 const rtsyn_port_spec_t output_ports[])
{
    return rtsyn_node_create(RTSYN_NODE_PLUGIN, id, num_input_ports, input_ports,
                                     num_output_ports, output_ports);
}
