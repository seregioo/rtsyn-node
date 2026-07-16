/**
 * @file rtsyn/node/aggregation.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for RTSyn Node Aggregation.
 *
 * This heder defines the aggregation of different node_aggregations inside an object that manages
 * their IDs.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_AGGREGATION_H
#define RTSYN_NODE_AGGREGATION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rtsyn/node.h"

/**
 * @brief Aggregation of nodes that automaticaly manages their ids.
 */
typedef struct rtsyn_node_aggregation_s rtsyn_node_aggregation_t;

/**
 * @brief Create an RTSyn Runtime Node.
 *
 * @param max_per_type Max number of values per type.
 * @return Pointer to the created Node Aggregation, or NULL if creation fails.
 */
rtsyn_node_aggregation_t *
rtsyn_node_aggregation_create(uint32_t max_per_type);
/**
 * @brief Destroy an RTSyn Runtime Node.
 *
 * @param node_aggregation Node Aggregation to be destroyed.
 */
void
rtsyn_node_aggregation_destroy(rtsyn_node_aggregation_t *node_aggregation);

/**
 * @brief Adds a node.
 *
 * @param node_aggregation Aggregation to add the node.
 * @param node Node to be added.
 */
void
rtsyn_node_aggregation_add(rtsyn_node_aggregation_t *node_aggregation,
                                   rtsyn_node_t *node);

/**
 * @brief Removes a node.
 *
 * @param node_aggregation Aggregation that will remove a node.
 * @param node_type Node type of the node be removed.
 * @param node_id Node ID of the node be removed.
 */
void
rtsyn_node_aggregation_remove(rtsyn_node_aggregation_t *node_aggregation,
                                      rtsyn_node_type_t node_type,
                                      rtsyn_node_id_t node_id);

/**
 * @brief Quries a node.
 *
 * @param node_aggregation Aggregation that will remove a node.
 * @param node_type Node type of the node be removed.
 * @param node_id Node ID of the node be removed.
 *
 * @return True if is in node aggregation, false otherwise.
 */
bool
rtsyn_node_aggregation_has_node(rtsyn_node_aggregation_t *node_aggregation,
                                        rtsyn_node_type_t node_type,
                                        rtsyn_node_id_t node_id);

/**
 * @brief Gets a node.
 *
 * @param node_aggregation Aggregation that will remove a node.
 * @param node_type Node type of the node be removed.
 * @param node_id Node ID of the node be removed.
 *
 * @return Pointer to node if is in node aggregation, false otherwise.
 */
rtsyn_node_t *
rtsyn_node_aggregation_get_node(rtsyn_node_aggregation_t *node_aggregation,
                                        rtsyn_node_type_t node_type,
                                        rtsyn_node_id_t node_id);

#endif /* RTSYN_NODE_AGGREGATION_H */
