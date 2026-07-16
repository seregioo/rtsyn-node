/**
 * @file node.cpp
 * @author Sergio Hidalgo (sergiohg.dev@gmail.com)
 * @brief Test file for the node logic
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 * @copyright Copyright (c) Sergio Hidalgo 2026
 */
#include <gtest/gtest.h>

extern "C" {
#include <rtsyn/port.h>
#include <rtsyn/port/descriptor.h>

#include "rtsyn/node.h"
#include "rtsyn/node/instance/device.h"
#include "rtsyn/node/instance/plugin.h"
}

namespace {

int test_param_value = 0;

extern "C" rtsyn_abi_status_t
test_create(void **out_instance)
{
    if (!out_instance)
    {
        return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
    }
    *out_instance = &test_param_value;
    return RTSYN_ABI_STATUS_OK;
}

extern "C" rtsyn_abi_status_t
test_set_param(void *instance, uint32_t param_index, const void *value)
{
    if (!instance || param_index != 0 || !value)
    {
        return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
    }

    test_param_value = *(const int *)value;
    return RTSYN_ABI_STATUS_OK;
}

extern "C" rtsyn_abi_status_t
test_read_state(const void *, uint32_t, void *)
{
    return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
}

extern "C" rtsyn_abi_status_t
test_step(void *)
{
    return RTSYN_ABI_STATUS_OK;
}

extern "C" rtsyn_abi_status_t
test_process(void *, const rtsyn_abi_runtime_context_t *)
{
    return RTSYN_ABI_STATUS_OK;
}

extern "C" void
test_destroy(void *)
{
}

} // namespace

class NodeTest : public ::testing::Test {
  protected:
    void
    SetUp() override
    {

        node_ = rtsyn_node_create_plugin(&_descriptor);
        ASSERT_NE(node_, nullptr);
    }

    void
    TearDown() override
    {
        rtsyn_node_destroy(node_);
        node_ = nullptr;
    }

    rtsyn_node_t *node_ = nullptr;

    rtsyn_abi_node_callbacks_t _callbacks = {
        .create = test_create,
        .set_param = test_set_param,
        .read_state = test_read_state,
        .start = test_step,
        .process = test_process,
        .stop = test_step,
        .destroy = test_destroy,
    };

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

    const rtsyn_abi_param_descriptor_t param_descriptors_[num_params_] = {
        {"param_a", "some param", RTSYN_ABI_VALUE_I64},
        {"param_b", "some param", RTSYN_ABI_VALUE_I64}};

    const rtsyn_abi_state_descriptor_t state_descriptors_[num_states_] = {
        {"state_a", "some state", RTSYN_ABI_VALUE_I64},
        {"state_b", "some state", RTSYN_ABI_VALUE_I64}};

    const rtsyn_abi_node_descriptor_t _descriptor = {.name = "node-test-param",
                                                     .node_type = RTSYN_ABI_NODE_PLUGIN,
                                                     .port_count = num_ports_,
                                                     .ports = ports_descriptors_,
                                                     .param_count = num_params_,
                                                     .params = param_descriptors_,
                                                     .state_count = num_states_,
                                                     .states = state_descriptors_,
                                                     .callbacks = _callbacks};
};

TEST_F(NodeTest, CreateNode)
{

    rtsyn_node_t *node = rtsyn_node_create_plugin(&_descriptor);

    ASSERT_NE(node, nullptr);

    rtsyn_node_destroy(node);

    rtsyn_abi_node_descriptor_t descr = _descriptor;

    descr.port_count = 0;
    descr.ports = nullptr;

    // Allow it to have no ports
    node = rtsyn_node_create_plugin(&descr);

    ASSERT_NE(node, nullptr);

    rtsyn_node_destroy(node);
}

TEST_F(NodeTest, PluginAndDeviceIdsAreGloballyUnique)
{
    rtsyn_abi_node_descriptor_t device_descriptor = _descriptor;
    device_descriptor.node_type = RTSYN_ABI_NODE_DEVICE;

    rtsyn_node_t *plugin = rtsyn_node_create_plugin(&_descriptor);
    rtsyn_node_t *device = rtsyn_node_create_device(&device_descriptor);

    ASSERT_NE(plugin, nullptr);
    ASSERT_NE(device, nullptr);
    EXPECT_NE(rtsyn_node_get_id(plugin), rtsyn_node_get_id(device));

    rtsyn_node_destroy(plugin);
    rtsyn_node_destroy(device);
}

TEST_F(NodeTest, CreateNodeIdenticalDescriptors)
{

    const uint32_t num_descriptors = 2;
    rtsyn_abi_port_descriptor_t ports_descriptors[num_descriptors] = {
        {"port_a", RTSYN_ABI_VALUE_F32, RTSYN_ABI_PORT_DIRECTION_IN},
        {"port_a", RTSYN_ABI_VALUE_F32, RTSYN_ABI_PORT_DIRECTION_IN}};
    rtsyn_abi_node_descriptor_t descr = _descriptor;

    descr.port_count = num_descriptors;
    descr.ports = ports_descriptors;

    rtsyn_node_t *node = rtsyn_node_create_plugin(&descr);
    ASSERT_EQ(node, nullptr);
    // TODO: Think a way of internally compare the descriptors names (or allow descriptors with same
    // name, which is a bit confussing)
    // rtsyn_abi_state_descriptor_t
    // states_descriptors[num_descriptors] = {
    //     {"some_a", "", RTSYN_ABI_VALUE_F32}, {"some_a", "", RTSYN_ABI_VALUE_F32}};
    // node = rtsyn_node_create_plugin_from_path(RTSYN_TEST_MODULE_PATH, 0, nullptr,
    // num_descriptors,
    //                                 states_descriptors, 0, nullptr);
    // ASSERT_EQ(node, nullptr);
    //
    // rtsyn_abi_param_descriptor_t params_descriptors[num_descriptors] = {
    //     {"some_a", "", RTSYN_ABI_VALUE_F32}, {"some_a", "", RTSYN_ABI_VALUE_F32}};
    // node = rtsyn_node_create_plugin_from_path(RTSYN_TEST_MODULE_PATH, 0, nullptr, 0, nullptr,
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

TEST_F(NodeTest, GetNodeState)
{
    const rtsyn_value_t *stored = rtsyn_node_get_state(node_, 1);
    ASSERT_NE(stored, nullptr);

    int out = 0;
    ASSERT_TRUE(rtsyn_value_get(stored, &out));
    EXPECT_EQ(out, 0);

    EXPECT_EQ(rtsyn_node_get_state(node_, RTSYN_NODE_VALUE_ID_INVALID), nullptr);
}

TEST_F(NodeTest, SetParamCallsModuleCallbackAfterCreate)
{
    rtsyn_node_t *node = rtsyn_node_create_plugin(&_descriptor);
    ASSERT_NE(node, nullptr);

    int value = 42;
    EXPECT_FALSE(rtsyn_node_set_param(node, 0, &value));

    ASSERT_TRUE(rtsyn_node_state_transition(node, RTSYN_NODE_RUNTIME_STATE_INIT));
    ASSERT_EQ(rtsyn_node_step(node, nullptr), RTSYN_ABI_STATUS_OK);

    ASSERT_TRUE(rtsyn_node_set_param(node, 0, &value));
    EXPECT_EQ(test_param_value, value);
    EXPECT_FALSE(rtsyn_node_set_param(node, RTSYN_NODE_VALUE_ID_INVALID, &value));

    rtsyn_node_destroy(node);
}
