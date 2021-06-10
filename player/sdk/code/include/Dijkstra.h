#pragma once

#include "Graph.h"

#include <vector>

class Graph;

class Dijkstra {
public:
    Dijkstra(const Graph& graph, Graph::NodeIndex source);
    ~Dijkstra();

    Dijkstra(const Dijkstra&) = delete;
    Dijkstra& operator= (const Dijkstra&) = delete;

private:
    static const Graph::Dist Inf;

    const Graph&        graph_;
    Graph::NodeIndex    source_;
    vector<Graph::NodeIndex> next_node_;
    vector<Graph::Dist> dists_;
};