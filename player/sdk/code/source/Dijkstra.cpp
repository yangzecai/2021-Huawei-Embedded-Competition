#include "Dijkstra.h"

#include <set>

const Graph::Dist Dijkstra::Inf = UINT32_MAX;

Dijkstra::Dijkstra(const Graph& graph, Graph::NodeIndex source) 
    : graph_(graph)
    , source_(source)
    , next_node_(graph.get_order())
    , dists_(graph.get_order(), Inf)
{
    
}

Dijkstra::~Dijkstra()
{
}