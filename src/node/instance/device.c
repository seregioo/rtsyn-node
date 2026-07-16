/**
 * @file node/instance/device.c
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Compile unit for RTSyn Device Node.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include "rtsyn/node/instance/device.h"
#include "rtsyn/internal/node.h"

rtsyn_node_t *
rtsyn_node_create_device(const rtsyn_abi_node_descriptor_t *descriptor)

{
    if (!descriptor || descriptor->node_type != RTSYN_ABI_NODE_DEVICE)
    {
        return nullptr;
    }

    return rtsyn_node_create(descriptor, rtsyn_node_next_id());
}
