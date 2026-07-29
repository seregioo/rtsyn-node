
#include <string.h>

#include "rtsyn/node/schema.h"

void
rtsyn_node_schema_init(rtsyn_node_schema_t *node_schema)
{
    if (!node_schema)
    {
        return;
    }

    node_schema->node_name[0] = '\0';
    node_schema->node_type = RTSYN_ABI_NODE_INVALID;
    node_schema->num_ports = 0;
    for (uint32_t i = 0; i < RTSYN_NODE_SCHEMA_PORT_SPECS_MAX; i++)
    {
        rtsyn_port_descriptor_init(&node_schema->ports_descriptors[i]);
    }
}

bool
rtsyn_node_schema_set(rtsyn_node_schema_t *node_schema, const char *node_name,
                      rtsyn_abi_node_type_t node_type, uint32_t num_ports,
                      const rtsyn_abi_port_descriptor_t ports_descriptors[])
{
    if (!node_schema || !node_name || !rtsyn_abi_node_type_is_valid(node_type)
        || num_ports > RTSYN_NODE_SCHEMA_PORT_SPECS_MAX)
    {
        return false;
    }

    strcpy(node_schema->node_name, node_name);
    node_schema->node_type = node_type;
    node_schema->num_ports = num_ports;
    memcpy(node_schema->ports_descriptors, ports_descriptors, sizeof(rtsyn_abi_port_descriptor_t) * num_ports);
    return true;
}

bool
rtsyn_node_schema_is_valid(rtsyn_node_schema_t *node_schema)
{
    return !(!node_schema || node_schema->node_name[0] == '\0'
             || !rtsyn_abi_node_type_is_valid(node_schema->node_type)
             || node_schema->num_ports > RTSYN_NODE_SCHEMA_PORT_SPECS_MAX);
}
