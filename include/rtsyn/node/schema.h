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

typedef struct rtsyn_node_schema_s rtsyn_node_schema_t;

rtsyn_node_schema_t *rtsyn_node_schema_create(char *node_name);

bool rtsyn_node_schema_add_port(rtsyn_node_schema_t *node_schema,
                                rtsyn_port_t *port);

bool rtsyn_node_schema_add_ports(rtsyn_node_schema_t *node_schema,
                                 rtsyn_port_t *port[], uint32_t port_count);

void rtsyn_node_schema_destroy(rtsyn_node_schema_t *node_schema);

#endif /* RTSYN_NODE_SCHEMA_H */
