/**
 * @file rtsyn/node/node_instance/device.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for RTSyn Device Node.
 *
 * This node handles the interaction with a device.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_DEVICE_H
#define RTSYN_NODE_DEVICE_H
#include "rtsyn/node.h"

rtsyn_node_t *
rtsyn_node_create_device(rtsyn_node_id_t id, uint32_t num_input_ports,
                                 const rtsyn_port_spec_t input_ports[], uint32_t num_output_ports,
                                 const rtsyn_port_spec_t output_ports[]);
void
rtsyn_node_device_executor(void *);

#endif /* RTSYN_NODE_DEVICE_H */
