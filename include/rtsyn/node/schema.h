/**
 * @file rtsyn/node/schema.h
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Header file for the RTSyn Runtime Schema.
 *
 * The RTSyn node Schema defines how the node is formed.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#ifndef RTSYN_NODE_SCHEMA_H
#define RTSYN_NODE_SCHEMA_H
#include <rtsyn/port.h>
#include <stdbool.h>
#include <stdint.h>

#include "rtsyn/node.h"

#define RTSYN_NODE_SCHEMA_NODE_NAME_MAX_LENGTH 256
#define RTSYN_NODE_SCHEMA_PORT_SPECS_MAX       128

typedef struct rtsyn_node_schema_s {
    char node_name[RTSYN_NODE_SCHEMA_NODE_NAME_MAX_LENGTH];
    rtsyn_abi_node_type_t node_type;
    uint32_t num_ports;
    rtsyn_abi_port_descriptor_t ports_descriptors[RTSYN_NODE_SCHEMA_PORT_SPECS_MAX];
} rtsyn_node_schema_t;

void
rtsyn_node_schema_init(rtsyn_node_schema_t *node_schema);

bool
rtsyn_node_schema_set(rtsyn_node_schema_t *node_schema, const char *node_name,
                      rtsyn_abi_node_type_t node_type, uint32_t num_ports,
                      const rtsyn_abi_port_descriptor_t ports_descriptors[]);

bool
rtsyn_node_schema_is_valid(rtsyn_node_schema_t *node_schema);

#endif /* RTSYN_NODE_SCHEMA_H */
