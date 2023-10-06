#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    // Initialize all indegrees to 0
    for (const auto& [vertex, _] : graph) {
        indegrees[vertex] = 0;
    }

    // Compute indegrees of all vertices
    for (const auto& [from, adj_list] : graph) {
        for (const auto& [to, weight] : adj_list) {
            indegrees[to]++;
        }
    }
}
