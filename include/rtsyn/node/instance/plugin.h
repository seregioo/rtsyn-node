/**
 * @file rtsyn/node/instance/plugin.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for RTSyn Plugin Node.
 *
 * This node executes a plugin.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_PLUGIN_H
#define RTSYN_NODE_PLUGIN_H
#include "rtsyn/node.h"

rtsyn_node_t *
rtsyn_node_create_plugin(const rtsyn_abi_node_descriptor_t *descriptor);

void
rtsyn_node_plugin_executor(void *);

#endif /* RTSYN_NODE_PLUGIN_H */
