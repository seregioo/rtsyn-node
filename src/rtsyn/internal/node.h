
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
#include <rtsyn/port.h>

#include "rtsyn/node.h"

/**
 * @brief Defines an initialized entry for a runtime node initialized table.
 *
 * Expands to a designated initializer for one runtime node table entry, which gives a
 * default initialization for each type, indexed by @p node_type.
 *
 * @param node_id Runtime node identifier.
 * @param node_type Runtime node type identifier.
 * @param node_name Human-readable runtime node name.
 * @param logic_executer Function used to retrieve events for the node.
 */
#define RTSYN_NODE_INITIALIZED_ENTRY(node_id, node_type, node_name, logic_executer)        \
    [node_type] = {                                                                                \
        .id = node_id,                                                                             \
        .type = node_type,                                                                         \
        .name = node_name,                                                                         \
        .execute_logic = logic_executer,                                                           \
    }

/**
 * @brief Node type definition.
 */
struct rtsyn_node_s {
    rtsyn_node_id_t id;     /**< Node type ID. */
    rtsyn_node_type_t type; /**< Node type. */
    uint32_t num_input_ports;       /**< Node number of input ports. */
    rtsyn_port_t **input_ports;     /**< Node input ports. */
    uint32_t num_output_ports;      /**< Node number of output ports. */
    rtsyn_port_t **output_ports;    /**< Node output ports. */
    const char *name;               /**< Node name string. */
    void (*execute_logic)(void *);  /**< Node executor. */
};

#endif /* RTSYN_INTERNAL_NODE_H */
