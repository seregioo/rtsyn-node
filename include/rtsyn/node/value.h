/**
 * @file rtsyn/node/value.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for values owned by an RTSyn Runtime Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_VALUE_H
#define RTSYN_NODE_VALUE_H

#include <rtsyn/abi/value.h>
#include <stdbool.h>
#include <stdint.h>

#define RTSYN_NODE_VALUE_ID_INVALID UINT32_MAX

typedef uint32_t rtsyn_node_value_id_t;

/**
 * @brief Opaque value owned by a node.
 */
typedef struct rtsyn_node_value_s rtsyn_node_value_t;

rtsyn_node_value_t *
rtsyn_node_value_create(rtsyn_node_value_id_t id, const char *name, const char *description,
                        rtsyn_abi_value_type_t value_type);

void
rtsyn_node_value_destroy(rtsyn_node_value_t *node_value);

bool
rtsyn_node_value_cmp_key(const rtsyn_node_value_t *node_value,
                         const rtsyn_node_value_id_t *id);

#endif /* RTSYN_NODE_VALUE_H */
