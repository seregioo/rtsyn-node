/**
 * @file node.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for the RTSyn Runtime Node
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <stdlib.h>
#include <string.h>

#include "rtsyn/internal/node.h"
#include "rtsyn/port/spec.h"
#include "rtsyn/node.h"
#include "rtsyn/node/node_instance/device.h"
#include "rtsyn/node/node_instance/plugin.h"
#include "rtsyn/node/node_instance/transmisor.h"

static bool
rtsyn_node_check_port_names_valid(uint32_t num_ports, const rtsyn_port_spec_t ports[]);

static void
rtsyn_node_default_executor(void *);

static bool
rtsyn_node_is_active(rtsyn_node_type_t node_type, rtsyn_node_id_t id);

/**
 * @brief Table that contains the default initialization of the nodes.
 *
 * If a new node is added, it must be added to this table too.
 *
 * @hideinitializer
 */
static const rtsyn_node_t
    RTSYN_NODE_INITIALIZATION_TABLE[RTSYN_NODE_COUNT] = {
        RTSYN_NODE_INITIALIZED_ENTRY(
            RTSYN_NODE_ID_INVALID, RTSYN_NODE_INVALID, "Invalid",
            rtsyn_node_default_executor), /**< Invalid node initialization */
        RTSYN_NODE_INITIALIZED_ENTRY(
            RTSYN_NODE_ID_INVALID, RTSYN_NODE_PLUGIN, "Plugin",
            rtsyn_node_plugin_executor), /**< Plugin node initialization */
        RTSYN_NODE_INITIALIZED_ENTRY(
            RTSYN_NODE_ID_INVALID, RTSYN_NODE_DEVICE, "Device",
            rtsyn_node_device_executor), /**< Device node initialization */
        RTSYN_NODE_INITIALIZED_ENTRY(
            RTSYN_NODE_ID_INVALID, RTSYN_NODE_TRANSMISOR, "Transmission",
            rtsyn_node_transmisor_executor), /**< Transmission node initialization */
};

static bool
rtsyn_node_check_port_names_valid(uint32_t num_ports, const rtsyn_port_spec_t ports[])
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
            if (i != j && strcmp(ports[i].name, ports[j].name) == 0)
            {
                valid = false;
            }
        }
    }

    return valid;
}

static void
rtsyn_node_default_executor(void *)
{
}

static bool
rtsyn_node_is_active(rtsyn_node_type_t node_type, rtsyn_node_id_t id)
{
    return true;
}

rtsyn_node_t *
rtsyn_node_create(rtsyn_node_type_t node_type, rtsyn_node_id_t id,
                          uint32_t num_input_ports, const rtsyn_port_spec_t input_ports[],
                          uint32_t num_output_ports, const rtsyn_port_spec_t output_ports[])
{

    rtsyn_port_t **input_ports_arr = nullptr;
    rtsyn_port_t **output_ports_arr = nullptr;
    rtsyn_node_type_t validated_type = rtsyn_node_validate_type(node_type);

    if (!rtsyn_node_check_port_names_valid(num_input_ports, input_ports)
        || !rtsyn_node_check_port_names_valid(num_input_ports, input_ports)
        || id == RTSYN_NODE_ID_INVALID)
    {

        return nullptr;
    }

    rtsyn_node_t *node = (rtsyn_node_t *)malloc(sizeof(rtsyn_node_t));

    if (!node)
    {
        return nullptr;
    }

    if (num_input_ports > 0)
    {
        input_ports_arr = (rtsyn_port_t **)malloc(sizeof(rtsyn_port_t *) * num_input_ports);
        if (!input_ports_arr)
        {
            free(node);
            return nullptr;
        }
    }

    if (num_output_ports > 0)
    {
        output_ports_arr = (rtsyn_port_t **)malloc(sizeof(rtsyn_port_t *) * num_output_ports);
        if (!output_ports_arr)
        {
            free(node);
            free(input_ports_arr);
            return nullptr;
        }
    }

    for (int i = 0; i < num_input_ports; i++)
    {
        input_ports_arr[i] = rtsyn_port_create(&input_ports[i], RTSYN_PORT_DIRECTION_IN, i);

        if (!input_ports_arr[i])
        {
            for (int j = i - 1; j >= 0; j--)
            {
                rtsyn_port_destroy(input_ports_arr[j]);
            }
            free(node);
            free(input_ports_arr);
            free(output_ports_arr);
            return nullptr;
        }
    }

    for (int i = 0; i < num_output_ports; i++)
    {
        output_ports_arr[i] = rtsyn_port_create(&output_ports[i], RTSYN_PORT_DIRECTION_OUT, i);

        if (!output_ports_arr[i])
        {
            for (int j = i - 1; j >= 0; j--)
            {
                rtsyn_port_destroy(output_ports_arr[j]);
            }
            free(node);
            free(input_ports_arr);
            free(output_ports_arr);
            return nullptr;
        }
    }

    memcpy(node, &RTSYN_NODE_INITIALIZATION_TABLE[validated_type],
           sizeof(rtsyn_node_t));

    node->id = id;

    node->num_input_ports = num_input_ports;
    node->input_ports = input_ports_arr;

    node->num_output_ports = num_output_ports;
    node->output_ports = output_ports_arr;

    return node;
}

void
rtsyn_node_destroy(rtsyn_node_t *rtsyn_node)
{
    for (int i = 0; i < rtsyn_node->num_input_ports; i++)
    {
        rtsyn_port_destroy(rtsyn_node->input_ports[i]);
    }
    free(rtsyn_node->input_ports);

    for (int i = 0; i < rtsyn_node->num_output_ports; i++)
    {
        rtsyn_port_destroy(rtsyn_node->output_ports[i]);
    }
    free(rtsyn_node->output_ports);

    free(rtsyn_node);
}

rtsyn_node_id_t
rtsyn_node_get_id(rtsyn_node_t *rtsyn_node)
{
    /* TODO: think if it makes sense to have the node id inide the node, this attribute isn't really
     * owned by the node itself, but by the aggregator/runtime */
    if (!rtsyn_node)
    {
        return RTSYN_NODE_ID_INVALID;
    }

    return rtsyn_node->id;
}

rtsyn_node_type_t
rtsyn_node_get_type(rtsyn_node_t *rtsyn_node)
{
    if (!rtsyn_node)
    {
        return RTSYN_NODE_INVALID;
    }
    return rtsyn_node->type;
}

const char *
rtsyn_node_get_name(rtsyn_node_t *rtsyn_node)
{
    rtsyn_node_type_t node_type = RTSYN_NODE_INVALID;

    if (rtsyn_node)
    {
        node_type = rtsyn_node->type;
    }

    return rtsyn_node_get_name_from_type(node_type);
}

const char *
rtsyn_node_get_name_from_type(rtsyn_node_type_t node_type)
{
    return RTSYN_NODE_INITIALIZATION_TABLE[rtsyn_node_validate_type(node_type)]
        .name;
}

rtsyn_node_type_t
rtsyn_node_validate_type(rtsyn_node_type_t node_type)
{
    if (node_type <= RTSYN_NODE_INVALID || node_type >= RTSYN_NODE_COUNT)
    {
        return RTSYN_NODE_INVALID;
    }

    return node_type;
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
                                  rtsyn_port_direction_type_t node_direction)
{
    rtsyn_port_direction_type_t validated_direction = rtsyn_port_validate_direction(node_direction);

    if (!node || validated_direction == RTSYN_PORT_DIRECTION_INVALID)
    {
        return nullptr;
    }

    rtsyn_port_t **ports = nullptr;
    uint32_t num_ports = 0;

    switch (validated_direction)
    {
        case RTSYN_PORT_DIRECTION_IN:
            ports = node->input_ports;
            num_ports = node->num_input_ports;
            break;
        case RTSYN_PORT_DIRECTION_OUT:
            ports = node->output_ports;
            num_ports = node->num_output_ports;
            break;
        default:
            return nullptr;
    }

    if (num_ports == 0 || node_id >= num_ports)
    {
        return nullptr;
    }

    return ports[node_id];
}

rtsyn_port_t *
rtsyn_node_get_port_by_name(rtsyn_node_t *node, const char *port_name,
                                    rtsyn_port_direction_type_t node_direction)
{
    rtsyn_port_direction_type_t validated_direction = rtsyn_port_validate_direction(node_direction);

    if (!node || validated_direction == RTSYN_PORT_DIRECTION_INVALID)
    {
        return nullptr;
    }

    rtsyn_port_t **ports = nullptr;
    uint32_t num_ports = 0;

    switch (validated_direction)
    {
        case RTSYN_PORT_DIRECTION_IN:
            ports = node->input_ports;
            num_ports = node->num_input_ports;
            break;
        case RTSYN_PORT_DIRECTION_OUT:
            ports = node->output_ports;
            num_ports = node->num_output_ports;
            break;
        default:
            return nullptr;
    }

    if (num_ports == 0)
    {
        return nullptr;
    }

    rtsyn_port_t *node_port = nullptr;

    for (int i = 0; i < num_ports; i++)
    {
        if (strcmp(port_name, rtsyn_port_get_name(ports[i])) == 0)
        {
            node_port = ports[i];
        }
    }

    return node_port;
}

void
rtsyn_node_execute(rtsyn_node_t *node, void *node_args)
{
    node->execute_logic(node_args);
}
