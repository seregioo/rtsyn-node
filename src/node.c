/**
 * @file node.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for the RTSyn Runtime Node
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <rtsyn/collection.h>
#include <rtsyn/port/descriptor.h>
#include <stdlib.h>
#include <string.h>

#include "rtsyn/internal/node.h"
#include "rtsyn/node.h"
#include "rtsyn/node/instance/device.h"
#include "rtsyn/node/instance/plugin.h"
#include "rtsyn/node/instance/transmisor.h"
#include "rtsyn/node/value.h"

static bool
rtsyn_node_check_port_names_valid(uint32_t num_ports, const rtsyn_abi_port_descriptor_t ports[]);

static bool
rtsyn_node_is_active(rtsyn_abi_node_type_t node_type, rtsyn_node_id_t id);

/**
 * @brief Table that contains the default initialization of the nodes.
 *
 * If a new node is added, it must be added to this table too.
 *
 * @hideinitializer
 */
static const rtsyn_node_t RTSYN_NODE_INITIALIZATION_TABLE[RTSYN_ABI_NODE_COUNT] = {
    RTSYN_NODE_INITIALIZED_ENTRY(RTSYN_NODE_ID_INVALID, RTSYN_ABI_NODE_PLUGIN, "Plugin",
                                 rtsyn_node_plugin_executor), /**< Plugin node initialization */
    RTSYN_NODE_INITIALIZED_ENTRY(RTSYN_NODE_ID_INVALID, RTSYN_ABI_NODE_DEVICE, "Device",
                                 rtsyn_node_device_executor), /**< Device node initialization */
    RTSYN_NODE_INITIALIZED_ENTRY(
        RTSYN_NODE_ID_INVALID, RTSYN_ABI_NODE_TRANSMISOR, "Transmission",
        rtsyn_node_transmisor_executor), /**< Transmission node initialization */
};

static bool
rtsyn_node_check_port_names_valid(uint32_t num_ports, const rtsyn_abi_port_descriptor_t ports[])
{
    bool valid = true;
    if (num_ports > 0 && !ports)
    {
        return false;
    }

    for (int i = 0; i < num_ports; i++)
    {
        for (int j = 0; j < num_ports; j++)
        {
            if (i != j && strcmp(ports[i].name, ports[j].name) == 0
                && ports[i].direction == ports[j].direction)
            {
                valid = false;
            }
        }
    }

    return valid;
}

static bool
rtsyn_node_port_cmp_by_id(rtsyn_port_t *port, rtsyn_port_id_t *port_id)
{
    if (!port || *port_id == RTSYN_PORT_ID_INVALID)
    {
        return false;
    }
    return rtsyn_port_get_id(port) == *port_id;
}

static bool
rtsyn_node_port_cmp_by_name(rtsyn_port_t *port, const char *port_name)
{
    if (!port || !port_name)
    {
        return false;
    }
    return strcmp(rtsyn_port_get_name(port), port_name) == 0;
}

static bool
rtsyn_node_is_active(rtsyn_abi_node_type_t node_type, rtsyn_node_id_t id)
{
    return true;
}

rtsyn_node_t *
rtsyn_node_create(const char *module_path, rtsyn_abi_node_type_t node_type, rtsyn_node_id_t id,
                  uint32_t num_ports, const rtsyn_abi_port_descriptor_t ports_descriptors[],
                  uint32_t num_states, const rtsyn_abi_state_descriptor_t states_descriptors[],
                  uint32_t num_params, const rtsyn_abi_param_descriptor_t params_descriptors[])
{
    if (!module_path || !rtsyn_node_check_port_names_valid(num_ports, ports_descriptors)
        || id == RTSYN_NODE_ID_INVALID || !rtsyn_abi_node_type_is_valid(node_type))
    {

        return nullptr;
    }

    rtsyn_collection_t *states = rtsyn_collection_create_linked_list(

        (rtsyn_collection_cmp_key_fn_t)rtsyn_node_value_cmp_key,
        (rtsyn_collection_destroy_elem_fn_t)rtsyn_node_value_destroy);
    if (!states)
    {
        return nullptr;
    }

    for (uint32_t i = 0; i < num_states; i++)
    {
        rtsyn_node_value_t *value = rtsyn_node_value_create(i, states_descriptors[i].name,
                                                            states_descriptors[i].description,
                                                            states_descriptors[i].value_type);

        if (!rtsyn_collection_add(states, value, &i))
        {
            rtsyn_collection_destroy(states);
            return nullptr;
        }
    }

    rtsyn_collection_t *params = rtsyn_collection_create_linked_list(
        (rtsyn_collection_cmp_key_fn_t)rtsyn_node_value_cmp_key,
        (rtsyn_collection_destroy_elem_fn_t)rtsyn_node_value_destroy);
    if (!params)
    {
        rtsyn_collection_destroy(states);
        return nullptr;
    }

    for (uint32_t i = 0; i < num_params; i++)
    {
        rtsyn_node_value_t *value = rtsyn_node_value_create(i, params_descriptors[i].name,
                                                            params_descriptors[i].description,
                                                            params_descriptors[i].value_type);

        if (!rtsyn_collection_add(params, value, &i))
        {
            rtsyn_collection_destroy(states);
            rtsyn_collection_destroy(params);
            return nullptr;
        }
    }

    rtsyn_module_loader_t *module_loader = rtsyn_module_loader_create(module_path);

    if (!module_loader)
    {
        rtsyn_collection_destroy(states);
        rtsyn_collection_destroy(params);
        return nullptr;
    }

    const rtsyn_abi_node_descriptor_t *descriptor =
        rtsyn_module_loader_get_descriptor(module_loader);

    if (!descriptor)
    {
        rtsyn_module_loader_destroy(module_loader);
        rtsyn_collection_destroy(params);
        rtsyn_collection_destroy(states);
        return nullptr;
    }

    rtsyn_node_t *node = (rtsyn_node_t *)malloc(sizeof(rtsyn_node_t));

    if (!node)
    {
        return nullptr;
    }

    memcpy(node, &RTSYN_NODE_INITIALIZATION_TABLE[node_type], sizeof(rtsyn_node_t));

    node->runtime = rtsyn_node_runtime_create(descriptor);

    if (!node->runtime)
    {
        rtsyn_collection_destroy(params);
        rtsyn_collection_destroy(states);
        rtsyn_module_loader_destroy(module_loader);
        free(node);
        return nullptr;
    }

    for (uint32_t i = 0; i < RTSYN_ABI_PORT_DIRECTION_COUNT; i++)
    {
        node->ports[i] = rtsyn_collection_create_linked_list(
            (rtsyn_collection_cmp_key_fn_t)rtsyn_node_port_cmp_by_id,
            (rtsyn_collection_destroy_elem_fn_t)rtsyn_port_destroy);

        if (!node->ports[i])
        {
            for (uint32_t j = i; j > 0; j--)
            {
                rtsyn_collection_destroy(node->ports[j]);
            }
            rtsyn_collection_destroy(params);
            rtsyn_collection_destroy(states);
            rtsyn_module_loader_destroy(module_loader);
            rtsyn_node_runtime_destroy(node->runtime);
            free(node);
            return nullptr;
        }
    }

    for (uint32_t i = 0; i < num_ports; i++)
    {
        rtsyn_port_t *port = rtsyn_port_create(&ports_descriptors[i], i);
        if (!rtsyn_collection_add(node->ports[ports_descriptors[i].direction], port, &i))
        {
            for (uint32_t j = 0; j < RTSYN_ABI_PORT_DIRECTION_COUNT; j++)
            {
                rtsyn_collection_destroy(node->ports[j]);
            }
            rtsyn_collection_destroy(params);
            rtsyn_collection_destroy(states);
            rtsyn_module_loader_destroy(module_loader);
            rtsyn_node_runtime_destroy(node->runtime);
            free(node);
            return nullptr;
        }
    }

    node->id = id;
    node->module_loader = module_loader;
    node->params = params;
    node->states = states;

    return node;
}

void
rtsyn_node_destroy(rtsyn_node_t *rtsyn_node)
{
    for (uint32_t i = 0; i < RTSYN_ABI_PORT_DIRECTION_COUNT; i++)
    {
        rtsyn_collection_destroy(rtsyn_node->ports[i]);
    }

    rtsyn_collection_destroy(rtsyn_node->params);
    rtsyn_collection_destroy(rtsyn_node->states);
    rtsyn_module_loader_destroy(rtsyn_node->module_loader);
    rtsyn_node_runtime_destroy(rtsyn_node->runtime);
    free(rtsyn_node);
}

rtsyn_node_id_t
rtsyn_node_get_id(rtsyn_node_t *rtsyn_node)
{
    if (!rtsyn_node)
    {
        return RTSYN_NODE_ID_INVALID;
    }

    return rtsyn_node->id;
}

rtsyn_abi_node_type_t
rtsyn_node_get_type(rtsyn_node_t *rtsyn_node)
{
    if (!rtsyn_node)
    {
        return RTSYN_ABI_NODE_INVALID;
    }
    return rtsyn_node->type;
}

const char *
rtsyn_node_get_name(rtsyn_node_t *rtsyn_node)
{
    rtsyn_abi_node_type_t node_type = RTSYN_ABI_NODE_INVALID;

    if (rtsyn_node)
    {
        node_type = rtsyn_node->type;
    }

    return rtsyn_node_get_name_from_type(node_type);
}

const char *
rtsyn_node_get_name_from_type(rtsyn_abi_node_type_t node_type)
{
    if (!rtsyn_abi_node_type_is_valid(node_type))
    {
        return nullptr;
    }

    return RTSYN_NODE_INITIALIZATION_TABLE[node_type].name;
}

bool
rtsyn_node_cmp(rtsyn_node_t *node_a, rtsyn_node_t *node_b)
{
    if (!node_a || !node_b)
    {
        return false;
    }

    return node_a->type == node_b->type && node_a->id == node_b->id;
}

rtsyn_port_t *
rtsyn_node_get_port_by_id(rtsyn_node_t *node, rtsyn_port_id_t node_id,
                          rtsyn_abi_port_direction_t node_direction)
{

    if (!node || !rtsyn_abi_port_direction_is_valid(node_direction))
    {
        return nullptr;
    }

    return (rtsyn_port_t *)rtsyn_collection_lookup(
        node->ports[node_direction], &node_id,
        (rtsyn_collection_cmp_key_fn_t)rtsyn_node_port_cmp_by_id);
}

rtsyn_port_t *
rtsyn_node_get_port_by_name(rtsyn_node_t *node, const char *port_name,
                            rtsyn_abi_port_direction_t node_direction)
{

    if (!node || !rtsyn_abi_port_direction_is_valid(node_direction))
    {
        return nullptr;
    }

    return (rtsyn_port_t *)rtsyn_collection_lookup(
        node->ports[node_direction], (void *)port_name,
        (rtsyn_collection_cmp_key_fn_t)rtsyn_node_port_cmp_by_name);
}

rtsyn_abi_status_t
rtsyn_node_step(rtsyn_node_t *node, rtsyn_abi_runtime_context_t *context)
{

    if (!node)
    {
        return false;
    }
    return rtsyn_node_runtime_step(node->runtime, node->module_instance, context);
}

bool
rtsyn_node_state_transition(rtsyn_node_t *node, rtsyn_node_runtime_state_t new_state)
{
    if (!node || !rtsyn_node_runtime_state_is_valid(new_state))
    {
        return false;
    }

    return rtsyn_node_runtime_transition(node->runtime, new_state);
}
