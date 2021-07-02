#pragma once

#include "ACO.h"
#include "Graph.h"
#include "RouteParser.h"
#include "data.h"
#include <assert.h>
#include <cmath>
#include <vector>

using std::vector;

class Solution {
public:
    Solution() = default;
    ~Solution() = default;

    Solution(const Solution &rhs) = delete;
    Solution &operator=(const Solution &rhs) = delete;

    vector<Route> Plan(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                       const vector<bool> &typeVec,
                       const vector<Edge> &edgeVec);
    void connBaseFromSet();
    SateGraph::NodeIndex getNearRecvSateFromSet(SateGraph::NodeIndex base);
    Power getMinPowerSum(); // for debug
private:
    RouteParser routeParser_;
    set<SateGraph::NodeIndex> minRecvSateSet_;

    void initMemData();
    void initGlobalData(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                        const vector<bool> &typeVec,
                        const vector<Edge> &edgeVec);
                        
    void initAGraph(uint32_t N, const vector<bool> &typeVec,
                    const vector<Edge> &edgeVec);
    void initSubset();
    void initBGraph();


}; // class Solution
