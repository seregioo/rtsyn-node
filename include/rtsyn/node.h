/**
 * @file rtsyn/node.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RTSyn Runtime Node.
 *
 * The RTSyn Runtime Node abstracts the logic to be executed or selected by the RTSyn Runtime State.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_H
#define RTSYN_NODE_H
#include <rtsyn/port.h>
#include <stdbool.h>
#include <stdint.h>

#include "rtsyn/port/spec.h"

#define RTSYN_NODE_ID_INVALID UINT32_MAX

/**
 * @brief Type for the nodes IDs.
 */
typedef uint32_t rtsyn_node_id_t;

/**
 * @brief Opaque RTSyn Runtime Node object.
 *
 * The node owns the logic demanded to be executed.
 * Create it with `rtsyn_node_create` and release it with
 * `rtsyn_node_destroy`.
 */
typedef struct rtsyn_node_s rtsyn_node_t;

/**
 * @brief Identifiers for RTSyn Runtime Node.
 *
 * If a new state is added, it must be added to the `RTSYN_NODE_INITIALIZATION_TABLE` too.
 */
typedef enum rtsyn_node_type_e {
    RTSYN_NODE_INVALID = 0, /**< Invalid or uninitialized node. */
    RTSYN_NODE_PLUGIN,      /**< Plugin execution. */
    RTSYN_NODE_DEVICE,      /**< Device execution. */
    RTSYN_NODE_TRANSMISOR,  /**< Propagates the runtime data through the
                                                 transmisor. */
    RTSYN_NODE_COUNT        /**< Number of node type. */
} rtsyn_node_type_t;

/**
 * @brief Create an RTSyn Runtime Node.
 *
 * @param node_type Node type.
 * @param id Node id.
 * @param num_input_ports Node number of input ports.
 * @param input_ports Pointer to output ports spec array.
 * @param num_output_ports Node number of output ports.
 * @param output_ports Pointer to output ports spec array.
 * @return Pointer to the created node, or NULL if creation fails.
 */
rtsyn_node_t *
rtsyn_node_create(rtsyn_node_type_t node_type, rtsyn_node_id_t id,
                          uint32_t num_input_ports, const rtsyn_port_spec_t input_ports[],
                          uint32_t num_output_ports, const rtsyn_port_spec_t output_ports[]);

/**
 * @brief Destroy an RTSyn Runtime Node.
 *
 * @param rtsyn_node Node to be destroyed.
 */
void
rtsyn_node_destroy(rtsyn_node_t *rtsyn_node);

rtsyn_node_id_t
rtsyn_node_get_id(rtsyn_node_t *rtsyn_node);

rtsyn_node_type_t
rtsyn_node_get_type(rtsyn_node_t *rtsyn_node);

const char *
rtsyn_node_get_name(rtsyn_node_t *rtsyn_node);

const char *
rtsyn_node_get_name_from_type(rtsyn_node_type_t node_type);

rtsyn_node_type_t
rtsyn_node_validate_type(rtsyn_node_type_t node_type);

bool
rtsyn_node_cmp(rtsyn_node_t *node_a, rtsyn_node_t *node_b);

rtsyn_port_t *
rtsyn_node_get_port_by_id(rtsyn_node_t *node, rtsyn_port_id_t node_id,
                                  rtsyn_port_direction_type_t node_direction);

rtsyn_port_t *
rtsyn_node_get_port_by_name(rtsyn_node_t *node, const char *port_name,
                                    rtsyn_port_direction_type_t node_direction);

void
rtsyn_node_execute(rtsyn_node_t *node, void *node_args);

#endif /* RTSYN_NODE_H */
