/**
 * @file node/aggregation.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for the RTSyn Runtime Node Aggregation
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <stdlib.h>

#include "rtsyn/internal/node/aggregation.h"
#include "rtsyn/node/aggregation.h"

rtsyn_node_aggregation_t *
rtsyn_node_aggregation_create(uint32_t max_per_type)
{
    (void)max_per_type;
    rtsyn_node_aggregation_t *aggregation =
        (rtsyn_node_aggregation_t *)malloc(sizeof(rtsyn_node_aggregation_t));

    aggregation->n_nodes = 0;
    aggregation->first_node = NULL;
    aggregation->last_node = NULL;

    return aggregation;
}

void
rtsyn_node_aggregation_destroy(rtsyn_node_aggregation_t *node_aggregation)
{
    rtsyn_node_aggregation_node_t *node = node_aggregation->first_node;
    rtsyn_node_aggregation_node_t *node_aux = NULL;
    while (node)
    {
        rtsyn_node_destroy(node->node_data);
        node_aux = node;
        node = node->next_node;
        free(node_aux);
    }
    free(node_aggregation);
}

void
rtsyn_node_aggregation_add(rtsyn_node_aggregation_t *node_aggregation,
                                   rtsyn_node_t *node)
{

    if (!node || !node_aggregation)
    {
        /* TODO: change return type */
        return;
    }

    rtsyn_node_aggregation_node_t *new_elem =
        (rtsyn_node_aggregation_node_t *)malloc(
            sizeof(rtsyn_node_aggregation_node_t));

    new_elem->node_data = node;
    new_elem->next_node = NULL;

    if (node_aggregation->n_nodes == 0)
    {
        node_aggregation->first_node = new_elem;
        node_aggregation->last_node = new_elem;
    } else
    {
        node_aggregation->last_node->next_node = new_elem;
    }

    node_aggregation->n_nodes++;
}

void
rtsyn_node_aggregation_remove(rtsyn_node_aggregation_t *node_aggregation,
                                      rtsyn_node_type_t node_type,
                                      rtsyn_node_id_t node_id)
{

    if (!node_aggregation || node_id == RTSYN_NODE_ID_INVALID)
    {
        /* TODO: change return type */
        return;
    }
    rtsyn_node_type_t validated_type = rtsyn_node_validate_type(node_type);
    rtsyn_node_aggregation_node_t *node = node_aggregation->first_node;
    rtsyn_node_aggregation_node_t *previous_node = NULL;

    while (node
           && (rtsyn_node_get_type(node->node_data) != node_type
               || rtsyn_node_get_id(node->node_data) != node_id))
    {
        previous_node = node;
        node = node->next_node;
    }

    if (node)
    {
        if (previous_node)
        {
            previous_node->next_node = node->next_node;
        }

        if (node_aggregation->first_node == node)
        {
            node_aggregation->first_node = node->next_node;
        }

        if (node_aggregation->last_node == node)
        {
            node_aggregation->last_node = node->next_node;
        }

        rtsyn_node_destroy(node->node_data);
        free(node);
    }

    node_aggregation->n_nodes--;
}

bool
rtsyn_node_aggregation_has_node(rtsyn_node_aggregation_t *node_aggregation,
                                        rtsyn_node_type_t node_type,
                                        rtsyn_node_id_t node_id)
{
    if (!node_aggregation || node_id == RTSYN_NODE_ID_INVALID)
    {
        return false;
    }

    rtsyn_node_aggregation_node_t *node = node_aggregation->first_node;
    while (node
           && (rtsyn_node_get_type(node->node_data) != node_type
               || rtsyn_node_get_id(node->node_data) != node_id))
    {
        node = node->next_node;
    }

    return node != NULL;
}

rtsyn_node_t *
rtsyn_node_aggregation_get_node(rtsyn_node_aggregation_t *node_aggregation,
                                        rtsyn_node_type_t node_type,
                                        rtsyn_node_id_t node_id)
{
    if (!node_aggregation || node_id == RTSYN_NODE_ID_INVALID)
    {
        return nullptr;
    }

    rtsyn_node_aggregation_node_t *node = node_aggregation->first_node;
    while (node
           && (rtsyn_node_get_type(node->node_data) != node_type
               || rtsyn_node_get_id(node->node_data) != node_id))
    {
        node = node->next_node;
    };

    if (node)
    {
        return node->node_data;
    }

    return NULL;
}
