/**
 * @file rtsyn/internal/node/value.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Private type definitions for values owned by an RTSyn Runtime Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_INTERNAL_NODE_VALUE_H
#define RTSYN_INTERNAL_NODE_VALUE_H

#include <rtsyn/value.h>

#include "rtsyn/node/value.h"

struct rtsyn_node_value_s {
    rtsyn_node_value_id_t id;
    char *name;
    char *description;
    rtsyn_value_t *value;
};

#endif /* RTSYN_INTERNAL_NODE_VALUE_H */
