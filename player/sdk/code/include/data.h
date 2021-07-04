#pragma once

#include "Graph.h"

#include <iostream>
#include <set>
#include <vector>

using std::set;
using std::vector;

struct Edge {
    uint32_t send; // 发送站点
    uint32_t recv; // 接受站点
    uint32_t dist; // 距离
};

using Route = vector<uint32_t>;

struct Sate {
    using ID = uint32_t;
    ID id;
    bool isSate;
};
using SateGraph = Graph<Sate>;
using Power = uint32_t;

extern SateGraph aGraph; /* aGraph : 当且仅当不同节点之间存在边时
                            之间有连线 */
extern SateGraph bGraph; /* bGraph : 当且仅当卫星和基站存在
                            长度小于最大路径的路时 之间有连线 */
extern Power kPowerPerDist;
extern Power kPowerPerSite;
extern SateGraph::Dist klimitDist;

extern vector<SateGraph::NodeIndex> baseSubset;
extern vector<SateGraph::NodeIndex> sateSubset;

SateGraph::NodeIndex
getNearSateFromSet(const set<SateGraph::NodeIndex> &recvSateSet,
                   SateGraph::NodeIndex base);
Power getPowerSum(const set<SateGraph::NodeIndex> &recvSateSet);
void removeRedundantSate(set<SateGraph::NodeIndex> &recvSateSet);