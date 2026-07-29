#include <stdbool.h>

#include "rtsyn/node/runtime/state.h"

bool
rtsyn_node_runtime_state_is_valid(rtsyn_node_runtime_state_t state)
{
    return !(state < RTSYN_NODE_RUNTIME_STATE_INIT || state == RTSYN_NODE_RUNTIME_STATE_INVALID
             || state >= RTSYN_NODE_RUNTIME_STATE_COUNT);
}
