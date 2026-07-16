/**
 * @file rtsyn/internal/node.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file that contains the private type definitions for RTSync Runtime Node
 * Aggregation.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_INTERNAL_NODE_AGGREGATION_H
#define RTSYN_INTERNAL_NODE_AGGREGATION_H

#include <stdbool.h>

#include "rtsyn/node.h"
#include "rtsyn/node/aggregation.h"

typedef struct rtsyn_node_aggregation_node_s rtsyn_node_aggregation_node_t;

struct rtsyn_node_aggregation_node_s {
    rtsyn_node_t *node_data;
    rtsyn_node_aggregation_node_t *next_node;
};

/**
 * @brief Node Aggregation type definition.
 */
struct rtsyn_node_aggregation_s {
    rtsyn_node_aggregation_node_t *first_node;
    rtsyn_node_aggregation_node_t *last_node;
    uint64_t n_nodes;
};

#endif /* RTSYN_INTERNAL_NODE_AGGREGATION_H */
