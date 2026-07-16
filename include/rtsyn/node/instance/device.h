/**
 * @file rtsyn/node/instance/device.h
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
rtsyn_node_create_device(const rtsyn_abi_node_descriptor_t *descriptor);

#endif /* RTSYN_NODE_DEVICE_H */
