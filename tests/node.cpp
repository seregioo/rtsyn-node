/**
 * @file node.cpp
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Test file for the node logic
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <gtest/gtest.h>

#ifndef RTSYN_TEST_MODULE_PATH
#error RTSYN_TEST_MODULE_PATH must name the module loader test fixture
#endif

extern "C" {
#include <rtsyn/port.h>
#include <rtsyn/port/descriptor.h>

#include "rtsyn/node.h"
#include "rtsyn/node/instance/plugin.h"
}

class NodeTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {
        node_ = rtsyn_node_create_plugin(RTSYN_TEST_MODULE_PATH, num_ports_, ports_descriptors_,
                                         num_states_, state_descriptors_, num_params_,
                                         param_descriptors_);
        ASSERT_NE(node_, nullptr);
    }

    void
    TearDown() override
    {
        rtsyn_node_destroy(node_);
        node_ = nullptr;
    }

    rtsyn_node_t *node_ = nullptr;

    static const uint32_t num_ports_ = (RTSYN_ABI_VALUE_COUNT) * 2;
    static const uint32_t num_states_ = 2;
    static const uint32_t num_params_ = 2;

    rtsyn_abi_port_descriptor_t ports_descriptors_[num_ports_] = {
        "float_in",   RTSYN_ABI_VALUE_F32,    RTSYN_ABI_PORT_DIRECTION_IN,
        "double_in",  RTSYN_ABI_VALUE_F64,    RTSYN_ABI_PORT_DIRECTION_IN,
        "int_in",     RTSYN_ABI_VALUE_I64,    RTSYN_ABI_PORT_DIRECTION_IN,
        "uint_in",    RTSYN_ABI_VALUE_U64,    RTSYN_ABI_PORT_DIRECTION_IN,
        "str_in",     RTSYN_ABI_VALUE_STRING, RTSYN_ABI_PORT_DIRECTION_IN,
        "float_out",  RTSYN_ABI_VALUE_F32,    RTSYN_ABI_PORT_DIRECTION_OUT,
        "double_out", RTSYN_ABI_VALUE_F64,    RTSYN_ABI_PORT_DIRECTION_OUT,
        "int_out",    RTSYN_ABI_VALUE_I64,    RTSYN_ABI_PORT_DIRECTION_OUT,
        "uint_out",   RTSYN_ABI_VALUE_U64,    RTSYN_ABI_PORT_DIRECTION_OUT,
        "str_out",    RTSYN_ABI_VALUE_STRING, RTSYN_ABI_PORT_DIRECTION_OUT};

    rtsyn_abi_state_descriptor_t state_descriptors_[num_states_] = {
        {"state_a", "some state", RTSYN_ABI_VALUE_I64},
        {"state_b", "some state", RTSYN_ABI_VALUE_I64}};
    rtsyn_abi_param_descriptor_t param_descriptors_[num_params_] = {
        {"param_a", "some param", RTSYN_ABI_VALUE_I64},
        {"param_b", "some param", RTSYN_ABI_VALUE_I64}};
};

TEST_F(NodeTest, CreateNode)
{

    rtsyn_node_t *node =
        rtsyn_node_create_plugin(RTSYN_TEST_MODULE_PATH, 0, nullptr, 0, nullptr, 0, nullptr);

    ASSERT_NE(node, nullptr);

    rtsyn_node_destroy(node);
}

TEST_F(NodeTest, CreateNodeIdenticalDescriptors)
{

    const uint32_t num_descriptors = 2;
    rtsyn_abi_port_descriptor_t ports_descriptors[num_descriptors] = {
        {"port_a", RTSYN_ABI_VALUE_F32, RTSYN_ABI_PORT_DIRECTION_IN},
        {"port_a", RTSYN_ABI_VALUE_F32, RTSYN_ABI_PORT_DIRECTION_IN}};

    rtsyn_node_t *node = rtsyn_node_create_plugin(RTSYN_TEST_MODULE_PATH, num_descriptors,
                                                  ports_descriptors, 0, nullptr, 0, nullptr);
    ASSERT_EQ(node, nullptr);
    // TODO: Think a way of internally compare the descriptors names (or allow descriptors with same
    // name, which is a bit confussing)
    // rtsyn_abi_state_descriptor_t
    // states_descriptors[num_descriptors] = {
    //     {"some_a", "", RTSYN_ABI_VALUE_F32}, {"some_a", "", RTSYN_ABI_VALUE_F32}};
    // node = rtsyn_node_create_plugin(RTSYN_TEST_MODULE_PATH, 0, nullptr, num_descriptors,
    //                                 states_descriptors, 0, nullptr);
    // ASSERT_EQ(node, nullptr);
    //
    // rtsyn_abi_param_descriptor_t params_descriptors[num_descriptors] = {
    //     {"some_a", "", RTSYN_ABI_VALUE_F32}, {"some_a", "", RTSYN_ABI_VALUE_F32}};
    // node = rtsyn_node_create_plugin(RTSYN_TEST_MODULE_PATH, 0, nullptr, 0, nullptr,
    // num_descriptors,
    //                                 params_descriptors);
    // ASSERT_EQ(node, nullptr);
}

TEST_F(NodeTest, RetrievePortById)
{
    for (int i = 0; i < num_ports_; i++)
    {
        rtsyn_port_t *found_port =
            rtsyn_node_get_port_by_id(node_, i, ports_descriptors_[i].direction);
        rtsyn_port_t *target_port = rtsyn_port_create(&ports_descriptors_[i], i);

        ASSERT_NE(found_port, nullptr);
        EXPECT_TRUE(rtsyn_port_cmp(found_port, target_port));
        rtsyn_port_destroy(target_port);
    }
}

TEST_F(NodeTest, RetrievePortByName)
{
    for (int i = 0; i < num_ports_; i++)
    {
        rtsyn_port_t *found_port = rtsyn_node_get_port_by_name(node_, ports_descriptors_[i].name,
                                                               ports_descriptors_[i].direction);
        rtsyn_port_t *target_port = rtsyn_port_create(&ports_descriptors_[i], i);

        ASSERT_NE(found_port, nullptr);
        EXPECT_TRUE(rtsyn_port_cmp(found_port, target_port));
        rtsyn_port_destroy(target_port);
    }
}
