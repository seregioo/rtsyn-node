/**
 * @file value.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for values owned by an RTSyn Runtime Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <stdlib.h>
#include <string.h>

#include "rtsyn/internal/node/value.h"

rtsyn_node_value_t *
rtsyn_node_value_create(rtsyn_node_value_id_t id, const char *name, const char *description,
                        rtsyn_abi_value_type_t value_type)
{
    if (id == RTSYN_NODE_VALUE_ID_INVALID || !name || !description
        || !rtsyn_abi_value_is_valid(value_type))
    {
        return nullptr;
    }

    char *name_aux = (char *)malloc(strlen(name) + 1);
    if (!name_aux)
    {
        return nullptr;
    }
    strcpy(name_aux, name);

    char *description_aux = (char *)malloc(strlen(description) + 1);
    if (!description_aux)
    {
        free(name_aux);
        return nullptr;
    }
    strcpy(description_aux, description);

    rtsyn_value_t *value = rtsyn_value_create(value_type);
    if (!value)
    {
        free(description_aux);
        free(name_aux);
        return nullptr;
    }

    rtsyn_node_value_t *node_value =
        (rtsyn_node_value_t *)malloc(sizeof(rtsyn_node_value_t));
    if (!node_value)
    {
        rtsyn_value_destroy(value);
        free(description_aux);
        free(name_aux);
        return nullptr;
    }

    node_value->id = id;
    node_value->name = name_aux;
    node_value->description = description_aux;
    node_value->value = value;

    return node_value;
}

void
rtsyn_node_value_destroy(rtsyn_node_value_t *node_value)
{
    if (!node_value)
    {
        return;
    }

    rtsyn_value_destroy(node_value->value);
    free(node_value->description);
    free(node_value->name);
    free(node_value);
}

bool
rtsyn_node_value_cmp_key(const rtsyn_node_value_t *node_value,
                         const rtsyn_node_value_id_t *id)
{
    if (!node_value || !id || *id == RTSYN_NODE_VALUE_ID_INVALID)
    {
        return false;
    }

    return node_value->id == *id;
}

const rtsyn_value_t *
rtsyn_node_value_get_state(const rtsyn_node_value_t *node_value)
{
    if (!node_value)
    {
        return nullptr;
    }

    return node_value->value;
}

bool
rtsyn_node_value_set_cached(rtsyn_node_value_t *node_value, void *value)
{
    if (!node_value)
    {
        return false;
    }

    return rtsyn_value_set(node_value->value, value);
}
