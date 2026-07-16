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
#include "rtsyn/internal/node/runtime.h"
#include "rtsyn/internal/node/value.h"
#include "rtsyn/node.h"
#include "rtsyn/node/instance/device.h"
#include "rtsyn/node/instance/plugin.h"
#include "rtsyn/node/value.h"

static bool
rtsyn_node_check_port_names_valid(uint32_t num_ports, const rtsyn_abi_port_descriptor_t ports[]);

static bool
rtsyn_node_is_active(rtsyn_abi_node_type_t node_type, rtsyn_node_id_t id);

static rtsyn_node_t *
rtsyn_node_create_with_loader(const rtsyn_abi_node_descriptor_t *descriptor, rtsyn_node_id_t id,
                              uint32_t num_ports,
                              const rtsyn_abi_port_descriptor_t ports_descriptors[],
                              uint32_t num_states,
                              const rtsyn_abi_state_descriptor_t states_descriptors[],
                              uint32_t num_params,
                              const rtsyn_abi_param_descriptor_t params_descriptors[]);

/**
 * @brief Table that contains the default initialization of the nodes.
 *
 * If a new node is added, it must be added to this table too.
 *
 * @hideinitializer
 */
static const rtsyn_node_t RTSYN_NODE_INITIALIZATION_TABLE[RTSYN_ABI_NODE_COUNT] = {
    RTSYN_NODE_INITIALIZED_ENTRY(RTSYN_NODE_ID_INVALID, RTSYN_ABI_NODE_PLUGIN,
                                 "Plugin"), /**< Plugin node initialization */
    RTSYN_NODE_INITIALIZED_ENTRY(RTSYN_NODE_ID_INVALID, RTSYN_ABI_NODE_DEVICE,
                                 "Device"), /**< Device node initialization */
};

static rtsyn_node_id_t rtsyn_next_node_id = 0;

rtsyn_node_id_t
rtsyn_node_next_id(void)
{
    if (rtsyn_next_node_id == RTSYN_NODE_ID_INVALID)
    {
        return RTSYN_NODE_ID_INVALID;
    }
    return rtsyn_next_node_id++;
}

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
rtsyn_node_create(const rtsyn_abi_node_descriptor_t *descriptor, rtsyn_node_id_t id)
{
    if (!rtsyn_abi_node_descriptor_is_valid(descriptor)
        || !rtsyn_node_check_port_names_valid(descriptor->port_count, descriptor->ports)
        || id == RTSYN_NODE_ID_INVALID)
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

    for (uint32_t i = 0; i < descriptor->state_count; i++)
    {
        rtsyn_node_value_t *value = rtsyn_node_value_create(i, descriptor->states[i].name,
                                                            descriptor->states[i].description,
                                                            descriptor->states[i].value_type);

        if (!rtsyn_collection_add(states, value, &i))
        {
            if (value)
            {
                rtsyn_node_value_destroy(value);
            }
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

    for (uint32_t i = 0; i < descriptor->param_count; i++)
    {
        rtsyn_node_value_t *value = rtsyn_node_value_create(i, descriptor->params[i].name,
                                                            descriptor->params[i].description,
                                                            descriptor->params[i].value_type);

        if (!rtsyn_collection_add(params, value, &i))
        {
            if (value)
            {
                rtsyn_node_value_destroy(value);
            }
            rtsyn_collection_destroy(states);
            rtsyn_collection_destroy(params);
            return nullptr;
        }
    }

    rtsyn_node_t *node = (rtsyn_node_t *)malloc(sizeof(rtsyn_node_t));

    if (!node)
    {
        rtsyn_collection_destroy(params);
        rtsyn_collection_destroy(states);
        return nullptr;
    }

    memcpy(node, &RTSYN_NODE_INITIALIZATION_TABLE[descriptor->node_type], sizeof(rtsyn_node_t));

    node->runtime = rtsyn_node_runtime_create(descriptor);

    if (!node->runtime)
    {
        rtsyn_collection_destroy(params);
        rtsyn_collection_destroy(states);
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
            rtsyn_node_runtime_destroy(node->runtime);
            free(node);
            return nullptr;
        }
    }

    for (uint32_t i = 0; i < descriptor->port_count; i++)
    {
        rtsyn_port_t *port = rtsyn_port_create(&descriptor->ports[i], i);
        if (!port || !rtsyn_collection_add(node->ports[descriptor->ports[i].direction], port, &i))
        {
            rtsyn_port_destroy(port);
            for (uint32_t j = 0; j < RTSYN_ABI_PORT_DIRECTION_COUNT; j++)
            {
                rtsyn_collection_destroy(node->ports[j]);
            }
            rtsyn_collection_destroy(params);
            rtsyn_collection_destroy(states);
            rtsyn_node_runtime_destroy(node->runtime);
            free(node);
            return nullptr;
        }
    }

    node->id = id;
    node->params = params;
    node->states = states;

    return node;
}

void
rtsyn_node_destroy(rtsyn_node_t *rtsyn_node)
{
    if (!rtsyn_node)
    {
        return;
    }

    for (uint32_t i = 0; i < RTSYN_ABI_PORT_DIRECTION_COUNT; i++)
    {
        rtsyn_collection_destroy(rtsyn_node->ports[i]);
    }

    rtsyn_collection_destroy(rtsyn_node->params);
    rtsyn_collection_destroy(rtsyn_node->states);
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
    if (!rtsyn_node)
    {
        return nullptr;
    }

    if (rtsyn_node->runtime && rtsyn_node->runtime->descriptor
        && rtsyn_node->runtime->descriptor->name)
    {
        return rtsyn_node->runtime->descriptor->name;
    }

    return rtsyn_node->name;
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
    return rtsyn_node_runtime_step(node->runtime, &node->module_instance, context);
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

rtsyn_node_runtime_state_t
rtsyn_node_get_runtime_state(rtsyn_node_t *node)
{
    if (!node)
    {
        return RTSYN_NODE_RUNTIME_STATE_INVALID;
    }

    return rtsyn_node_runtime_state_get(node->runtime);
}

const rtsyn_value_t *
rtsyn_node_get_state(rtsyn_node_t *node, rtsyn_node_value_id_t state_id)
{
    if (!node || state_id == RTSYN_NODE_VALUE_ID_INVALID)
    {
        return nullptr;
    }

    rtsyn_node_value_t *node_value =
        (rtsyn_node_value_t *)rtsyn_collection_get(node->states, &state_id);
    const rtsyn_value_t *state = rtsyn_node_value_get_state(node_value);
    if (!state)
    {
        return nullptr;
    }

    if (!node->module_instance || !node->runtime || !node->runtime->descriptor
        || !node->runtime->descriptor->callbacks.read_state)
    {
        return state;
    }

    union {
        float f32;
        double f64;
        int i64;
        uint64_t u64;
        char string[RTSYN_PORT_VALUE_STRING_MAX_SIZE];
    } value = {0};

    void *out_value = nullptr;
    switch (rtsyn_value_type_get(state))
    {
        case RTSYN_ABI_VALUE_F32:
            out_value = &value.f32;
            break;
        case RTSYN_ABI_VALUE_F64:
            out_value = &value.f64;
            break;
        case RTSYN_ABI_VALUE_I64:
            out_value = &value.i64;
            break;
        case RTSYN_ABI_VALUE_U64:
            out_value = &value.u64;
            break;
        case RTSYN_ABI_VALUE_STRING:
            out_value = value.string;
            break;
        default:
            return state;
    }

    if (node->runtime->descriptor->callbacks.read_state(node->module_instance, state_id, out_value)
        == RTSYN_ABI_STATUS_OK)
    {
        (void)rtsyn_node_value_set_cached(node_value, out_value);
    }

    return rtsyn_node_value_get_state(node_value);
}

bool
rtsyn_node_set_param(rtsyn_node_t *node, rtsyn_node_value_id_t param_id, const void *value)
{
    if (!node || param_id == RTSYN_NODE_VALUE_ID_INVALID || !value || !node->module_instance
        || !node->runtime || !node->runtime->descriptor
        || !node->runtime->descriptor->callbacks.set_param)
    {
        return false;
    }

    rtsyn_node_value_t *param = (rtsyn_node_value_t *)rtsyn_collection_get(node->params, &param_id);
    if (!param)
    {
        return false;
    }

    if (node->runtime->descriptor->callbacks.set_param(node->module_instance, param_id, value)
        != RTSYN_ABI_STATUS_OK)
    {
        return false;
    }

    return rtsyn_node_value_set_cached(param, (void *)value);
}
