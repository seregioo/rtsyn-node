/**
 * @file aggregation_node.cpp
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Test file for the aggregation node logic
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <gtest/gtest.h>

extern "C" {
#include "rtsyn/node.h"
#include "rtsyn/node/aggregation.h"
#include "rtsyn/node/node_instance/plugin.h"
}

class NodeAggregationTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        node_aggregation_ = rtsyn_node_aggregation_create(0);

        ASSERT_NE(node_aggregation_, nullptr);
    }

    void
    TearDown() override
    {
        rtsyn_node_aggregation_destroy(node_aggregation_);
        node_aggregation_ = nullptr;
    }

    rtsyn_node_aggregation_t *node_aggregation_ = nullptr;
};

TEST_F(NodeAggregationTest, AggregationAddNode)
{
    rtsyn_node_t *node = rtsyn_node_create_plugin(0, 0, nullptr, 0, nullptr);

    rtsyn_node_aggregation_add(node_aggregation_, node);

    EXPECT_TRUE(
        rtsyn_node_aggregation_has_node(node_aggregation_, RTSYN_NODE_PLUGIN, 0));

    rtsyn_node_t *aux_node =
        rtsyn_node_aggregation_get_node(node_aggregation_, RTSYN_NODE_PLUGIN, 9);
    EXPECT_EQ(aux_node, nullptr);
    EXPECT_FALSE(
        rtsyn_node_aggregation_has_node(node_aggregation_, RTSYN_NODE_PLUGIN, 1));
}

TEST_F(NodeAggregationTest, AggregationRemoveNode)
{
    rtsyn_node_t *node = rtsyn_node_create_plugin(0, 0, nullptr, 0, nullptr);

    rtsyn_node_aggregation_add(node_aggregation_, node);

    EXPECT_TRUE(
        rtsyn_node_aggregation_has_node(node_aggregation_, RTSYN_NODE_PLUGIN, 0));

    rtsyn_node_aggregation_remove(node_aggregation_, RTSYN_NODE_PLUGIN, 0);

    EXPECT_FALSE(
        rtsyn_node_aggregation_has_node(node_aggregation_, RTSYN_NODE_PLUGIN, 0));
}

TEST_F(NodeAggregationTest, AggregationGetNode)
{
    rtsyn_node_t *node = rtsyn_node_create_plugin(0, 0, nullptr, 0, nullptr);

    rtsyn_node_aggregation_add(node_aggregation_, node);

    rtsyn_node_t *node_b =
        rtsyn_node_aggregation_get_node(node_aggregation_, RTSYN_NODE_PLUGIN, 0);

    EXPECT_TRUE(rtsyn_node_cmp(node, node_b));
}
