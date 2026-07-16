
/**
 * @file rtsyn/internal/node.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file that contains the private type definitions for RTSync Runtime Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_INTERNAL_NODE_H
#define RTSYN_INTERNAL_NODE_H
#include <rtsyn/collection.h>
#include <rtsyn/module_loader.h>
#include <rtsyn/port.h>

#include "rtsyn/node.h"
#include "rtsyn/node/runtime.h"

/**
 * @brief Defines an initialized entry for a runtime node initialized table.
 *
 * Expands to a designated initializer for one runtime node table entry, which gives a
 * default initialization for each type, indexed by @p node_type.
 *
 * @param node_id Runtime node identifier.
 * @param node_type Runtime node type identifier.
 * @param node_name Human-readable runtime node name.
 * @param logic_executor Function used to retrieve events for the node.
 */
#define RTSYN_NODE_INITIALIZED_ENTRY(node_id, node_type, node_name, logic_executor)                \
    [node_type] = {                                                                                \
        .id = node_id,                                                                             \
        .type = node_type,                                                                         \
        .name = node_name,                                                                         \
        .execute_logic = logic_executor,                                                           \
    }

/**
 * @brief Node type definition.
 */
struct rtsyn_node_s {
    rtsyn_node_id_t id;                                        /**< Node type ID. */
    rtsyn_abi_node_type_t type;                                /**< Node type. */
    rtsyn_collection_t *ports[RTSYN_ABI_PORT_DIRECTION_COUNT]; /**< Node ports. */
    const char *name;                                          /**< Node name string. */
    rtsyn_node_runtime_t *runtime;
    rtsyn_module_loader_t *module_loader;
    rtsyn_collection_t *states;
    rtsyn_collection_t *params;
    void *module_instance;
    void (*execute_logic)(void *); /**< Node executor. */
};

#endif /* RTSYN_INTERNAL_NODE_H */
