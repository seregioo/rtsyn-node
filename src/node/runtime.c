#include <stdlib.h>
#include <string.h>

#include "rtsyn/internal/node/runtime.h"

rtsyn_node_runtime_t *
rtsyn_node_runtime_create(const rtsyn_abi_node_descriptor_t *descriptor)
{
    if (!descriptor)
    {
        return nullptr;
    }

    rtsyn_node_runtime_t *node_runtime =
        (rtsyn_node_runtime_t *)malloc(sizeof(rtsyn_node_runtime_t));

    if (!node_runtime)
    {
        return nullptr;
    }

    node_runtime->current = RTSYN_NODE_RUNTIME_STATE_INVALID;
    node_runtime->descriptor = descriptor;

    return node_runtime;
}

void
rtsyn_node_runtime_destroy(rtsyn_node_runtime_t *node_runtime)
{
    free(node_runtime);
}

rtsyn_abi_status_t
rtsyn_node_runtime_step(rtsyn_node_runtime_t *node_runtime, void *instance,
                        rtsyn_abi_runtime_context_t *context)
{
    if (!node_runtime)
    {
        return RTSYN_ABI_STATUS_INVALID_ARGUMENT;
    }
    switch (node_runtime->current)
    {
        case RTSYN_NODE_RUNTIME_STATE_INIT:
            RTSYN_NODE_RUNTIME_IF_FAIL_RETURN(
                node_runtime->descriptor->callbacks.create(&instance));
            break;
        case RTSYN_NODE_RUNTIME_STATE_START:
            RTSYN_NODE_RUNTIME_IF_FAIL_RETURN(node_runtime->descriptor->callbacks.start(instance));
            break;
        case RTSYN_NODE_RUNTIME_STATE_PROCESS:
            RTSYN_NODE_RUNTIME_IF_FAIL_RETURN(
                node_runtime->descriptor->callbacks.process(instance, context));
            break;
        case RTSYN_NODE_RUNTIME_STATE_RESTART:
            RTSYN_NODE_RUNTIME_IF_FAIL_RETURN(node_runtime->descriptor->callbacks.stop(instance));
            RTSYN_NODE_RUNTIME_IF_FAIL_RETURN(node_runtime->descriptor->callbacks.start(instance));
            break;
        case RTSYN_NODE_RUNTIME_STATE_FINI:
            node_runtime->descriptor->callbacks.destroy(instance);
            break;
        default:
            return RTSYN_ABI_STATUS_INVALID_STATE;
    }

    return RTSYN_ABI_STATUS_OK;
}

bool
rtsyn_node_runtime_transition(rtsyn_node_runtime_t *node_runtime,
                              rtsyn_node_runtime_state_t next_step)
{
    if (!node_runtime || !rtsyn_node_runtime_state_is_valid(next_step))
    {
        return false;
    }

    node_runtime->current = next_step;

    return true;
}

rtsyn_node_runtime_state_t
rtsyn_node_runtime_state_get(rtsyn_node_runtime_t *runtime)
{
    if (!runtime)
    {
        return RTSYN_NODE_RUNTIME_STATE_INVALID;
    }

    return runtime->current;
}
