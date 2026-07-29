/**
 * @file rtsyn/internal/node/runtime.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Internal header file for the RTSyn Runtime Runtime.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_INTERNAL_NODE_RUNTIME_H
#define RTSYN_INTERNAL_NODE_RUNTIME_H
#include "rtsyn/node/runtime.h"

struct rtsyn_node_runtime_s {
    rtsyn_node_runtime_state_t current;
    const rtsyn_abi_node_descriptor_t *descriptor;
};

#define RTSYN_NODE_RUNTIME_IF_FAIL_RETURN(call_result)                                             \
    do                                                                                             \
    {                                                                                              \
        if (call_result < 0)                                                                       \
        {                                                                                          \
            return call_result;                                                                    \
        }                                                                                          \
    } while (false);

#endif /* RTSYN_INTERNAL_NODE_RUNTIME_H */
