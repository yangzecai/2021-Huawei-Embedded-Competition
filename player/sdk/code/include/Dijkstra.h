#pragma once

#include "Graph.h"

#include <vector>

class Dijkstra {
public:
    Dijkstra(const Graph&, id_t source);
    ~Dijkstra();

    Dijkstra(const Dijkstra&) = delete;
    Dijkstra& operator= (const Dijkstra&) = delete;

    void solve();
private:
    const Graph&    graph_;
    id_t            source_;
};