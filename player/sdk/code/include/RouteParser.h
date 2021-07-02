#pragma once

#include "data.h"
#include <vector>

using std::vector;

class RouteParser {
public:
    RouteParser() = default;
    RouteParser(const SateGraph &graph);
    ~RouteParser() = default;
    RouteParser(const RouteParser &) = delete;
    RouteParser &operator=(const RouteParser &) = delete;
    RouteParser(RouteParser &&);
    RouteParser &operator=(RouteParser &&);

    void createRoute(SateGraph::NodeIndex sate,
                     const vector<SateGraph::NodeIndex> &bases);
    void trimRoute();
    vector<Route> getRoute();

private:
    vector<SateGraph::NodeIndex> lasts_;
};