#pragma once

#include "Graph.h"

#include <vector>

class Graph;

class Dijkstra {
public:
    Dijkstra(const Graph& graph, Graph::NodeIndex source, 
             Graph::Dist limit = Graph::kInf);
    ~Dijkstra();

    Dijkstra(const Dijkstra&) = delete;
    Dijkstra& operator= (const Dijkstra&) = delete;

    Graph::Dist getDist(const Graph::NodeIndex node) const
    { return dists_[node]; }
    Graph::Dist getDistSum(const vector<Graph::NodeIndex>&) const;

    Graph::Route getRoute(Graph::NodeIndex) const;
    vector<Graph::Route> getRouteAll(vector<Graph::NodeIndex>&) const;

    const vector<Graph::NodeIndex>& getReached() const
    { return reached_; }

    vector<Graph::NodeIndex>& getLast()
    { return last_; }

    void displayDists() const; // for debug
    void displayRoute() const; // for debug
    void displayReached() const; // for debug

private:
    vector<Graph::NodeIndex> last_;
    vector<Graph::Dist> dists_;
    vector<Graph::NodeIndex> reached_;
};