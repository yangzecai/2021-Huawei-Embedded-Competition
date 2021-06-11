#pragma once

#include "Graph.h"

#include <vector>

class Graph;

class Dijkstra {
public:
    Dijkstra(Graph& graph, Graph::NodeIndex source);
    ~Dijkstra();

    Dijkstra(const Dijkstra&) = delete;
    Dijkstra& operator= (const Dijkstra&) = delete;

    void displayDists() const; // for debug
    void displayRoute() const; // for debug

private:
    Graph::NodeIndex    source_;
    vector<Graph::NodeIndex> last_;
    vector<Graph::Dist> dists_;
};