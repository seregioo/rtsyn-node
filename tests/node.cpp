/**
 * @file node.cpp
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Test file for the node logic
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <gtest/gtest.h>

#include "rtsyn/port/spec.h"

extern "C" {
#include <rtsyn/port.h>

#include "rtsyn/node.h"
#include "rtsyn/node/node_instance/plugin.h"
}

class NodeTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        node_ = rtsyn_node_create_plugin(0, num_inputs_, node_inputs_, num_outputs_,
                                                 node_outputs_);
        ASSERT_NE(node_, nullptr);
    }

    void
    TearDown() override
    {
        rtsyn_node_destroy(node_);
        node_ = nullptr;
    }

    rtsyn_node_t *node_ = nullptr;

    uint32_t num_inputs_ = RTSYN_PORT_VALUE_COUNT - 1;
    rtsyn_port_spec_t node_inputs_[RTSYN_PORT_VALUE_COUNT - 1] = {
        "float_in", RTSYN_PORT_VALUE_FLOAT, "double_in", RTSYN_PORT_VALUE_DOUBLE,
        "int_in",   RTSYN_PORT_VALUE_INT,   "uint_in",   RTSYN_PORT_VALUE_UINT,
        "str_in",   RTSYN_PORT_VALUE_STR};

    uint32_t num_outputs_ = RTSYN_PORT_VALUE_COUNT - 1;
    rtsyn_port_spec_t node_outputs_[RTSYN_PORT_VALUE_COUNT - 1] = {
        "float_out", RTSYN_PORT_VALUE_FLOAT, "double_out", RTSYN_PORT_VALUE_DOUBLE,
        "int_out",   RTSYN_PORT_VALUE_INT,   "uint_out",   RTSYN_PORT_VALUE_UINT,
        "str_out",   RTSYN_PORT_VALUE_STR};
};

TEST_F(NodeTest, CreateNode)
{
    rtsyn_node_t *node = rtsyn_node_create_plugin(0, 0, nullptr, 0, nullptr);

    ASSERT_NE(node, nullptr);

    rtsyn_node_destroy(node);
}

TEST_F(NodeTest, RetrievePortById)
{
    for (int i = 0; i < num_inputs_; i++)
    {
        rtsyn_port_t *found_port =
            rtsyn_node_get_port_by_id(node_, i, RTSYN_PORT_DIRECTION_IN);

        ASSERT_NE(found_port, nullptr);
        EXPECT_STREQ(rtsyn_port_get_name(found_port), node_inputs_[i].name);
    }

    for (int i = 0; i < num_outputs_; i++)
    {
        rtsyn_port_t *found_port =
            rtsyn_node_get_port_by_id(node_, i, RTSYN_PORT_DIRECTION_OUT);

        ASSERT_NE(found_port, nullptr);
        EXPECT_STREQ(rtsyn_port_get_name(found_port), node_outputs_[i].name);
    }
}

TEST_F(NodeTest, RetrievePortByName)
{
    for (int i = 0; i < num_inputs_; i++)
    {
        rtsyn_port_t *found_port = rtsyn_node_get_port_by_name(node_, node_inputs_[i].name,
                                                                       RTSYN_PORT_DIRECTION_IN);

        ASSERT_NE(found_port, nullptr);
        EXPECT_STREQ(rtsyn_port_get_name(found_port), node_inputs_[i].name);
    }

    for (int i = 0; i < num_outputs_; i++)
    {
        rtsyn_port_t *found_port = rtsyn_node_get_port_by_name(node_, node_outputs_[i].name,
                                                                       RTSYN_PORT_DIRECTION_OUT);

        ASSERT_NE(found_port, nullptr);
        EXPECT_STREQ(rtsyn_port_get_name(found_port), node_outputs_[i].name);
    }
}
