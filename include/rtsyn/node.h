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
#include <rtsyn/abi/node.h>
#include <rtsyn/abi/port/direction.h>
#include <rtsyn/abi/runtime.h>
#include <rtsyn/abi/status.h>
#include <rtsyn/port.h>
#include <rtsyn/port/descriptor.h>
#include <stdbool.h>
#include <stdint.h>

#include "rtsyn/node/runtime/state.h"

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
rtsyn_node_create(const char *module_path, rtsyn_abi_node_type_t node_type, rtsyn_node_id_t id,
                  uint32_t num_ports, const rtsyn_abi_port_descriptor_t ports_descriptors[],
                  uint32_t num_states, const rtsyn_abi_state_descriptor_t states_descriptors[],
                  uint32_t num_params, const rtsyn_abi_param_descriptor_t params_descriptors[]);

/**
 * @brief Destroy an RTSyn Runtime Node.
 *
 * @param rtsyn_node Node to be destroyed.
 */
void
rtsyn_node_destroy(rtsyn_node_t *rtsyn_node);

rtsyn_node_id_t
rtsyn_node_get_id(rtsyn_node_t *rtsyn_node);

rtsyn_abi_node_type_t
rtsyn_node_get_type(rtsyn_node_t *rtsyn_node);

const char *
rtsyn_node_get_name(rtsyn_node_t *rtsyn_node);

const char *
rtsyn_node_get_name_from_type(rtsyn_abi_node_type_t node_type);

bool
rtsyn_abi_node_type_is_valid(rtsyn_abi_node_type_t node_type);

bool
rtsyn_node_cmp(rtsyn_node_t *node_a, rtsyn_node_t *node_b);

rtsyn_port_t *
rtsyn_node_get_port_by_id(rtsyn_node_t *node, rtsyn_port_id_t node_id,
                          rtsyn_abi_port_direction_t node_direction);

rtsyn_port_t *
rtsyn_node_get_port_by_name(rtsyn_node_t *node, const char *port_name,
                            rtsyn_abi_port_direction_t node_direction);

rtsyn_abi_status_t
rtsyn_node_step(rtsyn_node_t *node, rtsyn_abi_runtime_context_t *context);

bool
rtsyn_node_state_transition(rtsyn_node_t *node, rtsyn_node_runtime_state_t new_state);
#endif /* RTSYN_NODE_H */
