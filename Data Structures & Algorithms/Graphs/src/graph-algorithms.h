#pragma once

#include <istream>
#include <ostream>
#include <sstream>
#include <limits>
#include <list>
#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "weighted-graph.hpp"

#include "graph-types.h"

// if the arrow is a box, change to the other line
#define ARROW_SEPARATOR " \u2192 "
// #define ARROW_SEPARATOR " -> "

// contains initializeSingleSource, relax, & updateHeap as well as the DijkstraComaparator
#include "dijkstras-helpers.h"

/**
 * @brief Dijkstra's Algorithm - https://canvas.tamu.edu/courses/136654/files/35930572/preview Slide 16
 *
 * @tparam T type of data stored by a vertex
 * @param graph weighted, directed graph to find single-source shortest-path
 * @param initial_node source node in graph for shortest path
 * @param destination_node destination node in graph for shortest path
 * @return std::list<value_type<T>> list of nodes along shortest path including initial_node and destination_node, empty if no path exists
 */
template <typename T>
std::list<value_type<T>> dijkstrasAlgorithm(const WeightedGraph<T>& graph, vertex_type<T> initial_node, vertex_type<T> destination_node)
{
    std::unordered_map<value_type<T>, weight_type<T>> distances;
    std::unordered_map<value_type<T>, std::optional<value_type<T>>> predecessors;
    std::unordered_set<value_type<T>> s;
    std::priority_queue<value_type<T>, std::vector<value_type<T>>, DijkstraComparator<T>> q(DijkstraComparator<T>{distances});

    // TODO implement Dijkstra's Algorithm
    // Initialize distances and predecessors
    initializeSingleSource(graph, initial_node, distances, predecessors);

    // Add all vertices to the priority queue
    for (const auto& vertex : graph) {
        q.push(vertex.first);
    }

    while (!q.empty()) {
        // Get the vertex with the smallest distance
        value_type<T> u = q.top();
        q.pop();

        // If the distance is infinity, then remaining vertices are inaccessible
        if (distances[u] == infinity<T>()) {
            break;
        }

        // Add the vertex to the set of visited vertices
        s.insert(u);

        // For each neighbor of u, relax the edge if necessary
        for (const auto& pair : graph.at(u)) {
            value_type<T> v = pair.first;
            weight_type<T> w = pair.second;
            if (s.find(v) == s.end()) {
                if (relax<T>(u, v, w, distances, predecessors)) {
                    // Update the priority queue with the new distances
                    updateHeap<T>(q, distances);
                }
            }
        }
    }

    // Create list by walking backwards through predecessors from the end
    std::list<value_type<T>> path;
    value_type<T> node = destination_node;
    while (predecessors[node].has_value()) {
        path.push_front(node);
        node = predecessors[node].value();
    }
    if (path.empty() && initial_node != destination_node) {
        // There is no path to the destination
        return std::list<value_type<T>>();
    } else {
        path.push_front(initial_node);
        return path;
    }

    // TODO create list by walking backwards through predecessors from the end

    //return std::list<value_type<T>>();
}

#include "top-sort-helpers.h"

/**
 * @brief Returns a Topological Ordering of the Graph - https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search
 *
 * @tparam T type of data stored by a vertex
 * @param graph graph upon which to perform a topological ordering
 * @return std::list<value_type<T>> list of nodes in a topological order, or an empty list if no such ordering exists
 */
template <typename T>
std::list<value_type<T>> topologicalSort(const WeightedGraph<T>& graph)
{
    std::unordered_map<value_type<T>, int> indegrees;
    std::unordered_map<value_type<T>, int> topological_numbers;

    // WeightedGraph temp = graph;
    // iterator i = graph.cbegin();
    // int numvertices = 0;
    // while (i!=graph.cend()){
    //     numvertices++;
    // }
    
    // while (!temp.empty()){

    // }
    // Compute indegrees of all vertices
    computeIndegrees(graph, indegrees);

    std::list<value_type<T>> result;

    // Initialize queue with vertices with indegree 0
    std::queue<value_type<T>> q;
    for (const auto& [vertex, indegree] : indegrees) {
        if (indegree == 0) {
            q.push(vertex);
        }
    }

    int count = 0;
    // Perform BFS
    while (!q.empty()) {
        value_type<T> vertex = q.front();
        q.pop();
        topological_numbers[vertex] = count++;
        result.push_back(vertex);

        for (const auto& [to, weight] : graph.at(vertex)) {
            indegrees[to]--;
            if (indegrees[to] == 0) {
                q.push(to);
            }
        }
    }

    // Check if a valid topological ordering exists
    if (count != graph.size()) {
        return std::list<value_type<T>>();
    }

    return result;

    //return std::list<value_type<T>>();
}

template <typename T>
std::ostream &operator<<(std::ostream &o, const WeightedGraph<T> &graph)
{
    for (auto it = graph.begin(); it != graph.end(); ++it)
    {
        const vertex_type<T> &vertex = it->first;
        const adjacency_list<T> &list = it->second;
        o << vertex << ": ";
        for (auto iit = list.begin(); iit != list.end(); ++iit)
        {
            const vertex_type<T> &destination = iit->first;
            const weight_type<T> &weight = iit->second;
            o << destination << "(" << weight << ')';
            auto next_iter = iit;
            if (++next_iter != list.end())
            {
                o << ARROW_SEPARATOR;
            }
        }
        auto next_iter = it;
        if (++next_iter != graph.end())
        {
            o << "\n";
        }
    }
    return o;
}

template <typename T>
std::istream &readEdge(std::istream &i, value_type<T> &vertex, weight_type<T> &weight)
{
    std::string s_vertex, s_weight;
    std::getline(i, s_vertex, '(');
    std::getline(i, s_weight, ')');
    std::istringstream stream(s_vertex + " " + s_weight);
    stream >> vertex >> weight;
    if (stream.fail())
    {
        i.setf(stream.flags());
    }
    return i;
}

template <typename T>
std::istream &operator>>(std::istream &i, WeightedGraph<T> &graph)
{
    std::string line;
    while (std::getline(i, line))
    {
        if (line.empty())
            break;
        std::istringstream line_stream(line);
        value_type<T> vertex;
        std::string s_vertex;
        std::getline(line_stream, s_vertex, ':');
        std::istringstream stream_vertex(s_vertex);
        stream_vertex >> vertex;
        if (stream_vertex.fail())
        {
            i.setf(stream_vertex.flags());
            break;
        }
        graph.push_vertex(vertex);
        std::string separator;
        value_type<T> edge_end;
        weight_type<T> edge_weight;
        while (readEdge<T>(line_stream, edge_end, edge_weight))
        {
            line_stream >> separator;
            graph.push_edge(vertex, edge_end, edge_weight);
        }
    }

    if(i.eof() and i.fail())
        i.clear(std::ios::eofbit);
    
    return i;
}
