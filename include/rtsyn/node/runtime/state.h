/**
 * @file rtsyn/node/runtime/state.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RTSyn Node Runtime State.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_RUNTIME_STATE_H
#define RTSYN_NODE_RUNTIME_STATE_H
#include <rtsyn/abi/port/direction.h>
#include <rtsyn/collection.h>

typedef void (*rtsyn_node_runtime_state_fn_t)(
    rtsyn_collection_t *ports[RTSYN_ABI_PORT_DIRECTION_COUNT]);

typedef enum rtsyn_node_runtime_state_e {
    RTSYN_NODE_RUNTIME_STATE_INIT = 0,
    RTSYN_NODE_RUNTIME_STATE_START,
    RTSYN_NODE_RUNTIME_STATE_PROCESS,
    RTSYN_NODE_RUNTIME_STATE_RESTART,
    RTSYN_NODE_RUNTIME_STATE_STOP,
    RTSYN_NODE_RUNTIME_STATE_FINI,
    RTSYN_NODE_RUNTIME_STATE_COUNT,
    RTSYN_NODE_RUNTIME_STATE_INVALID,
} rtsyn_node_runtime_state_t;

bool
rtsyn_node_runtime_state_is_valid(rtsyn_node_runtime_state_t state);

#endif /* RTSYN_NODE_RUNTIME_STATE_H */
