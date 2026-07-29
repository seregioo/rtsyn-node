/**
 * @file rtsyn/node/runtime.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RTSyn Runtime Runtime.
 *
 * The RTSyn node Runtime defines how the node is formed.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_RUNTIME_H
#define RTSYN_NODE_RUNTIME_H
#include <rtsyn/abi/node.h>
#include <rtsyn/collection.h>

#include "rtsyn/node/runtime/state.h"

typedef struct rtsyn_node_runtime_s rtsyn_node_runtime_t;

rtsyn_node_runtime_t *
rtsyn_node_runtime_create(const rtsyn_abi_node_descriptor_t *descriptor);

void
rtsyn_node_runtime_destroy(rtsyn_node_runtime_t *node_runtime);

rtsyn_abi_status_t
rtsyn_node_runtime_step(rtsyn_node_runtime_t *node_runtime, void **instance,
                        rtsyn_abi_runtime_context_t *context);

bool
rtsyn_node_runtime_transition(rtsyn_node_runtime_t *node_runtime,
                              rtsyn_node_runtime_state_t next_step);

rtsyn_node_runtime_state_t
rtsyn_node_runtime_state_get(rtsyn_node_runtime_t *runtime);

#endif /* RTSYN_NODE_RUNTIME_H */
