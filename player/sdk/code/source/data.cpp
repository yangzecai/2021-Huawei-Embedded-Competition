#include "data.h"

SateGraph aGraph;
SateGraph bGraph;

Power kPowerPerDist;
Power kPowerPerSite;
SateGraph::Dist klimitDist;

vector<SateGraph::NodeIndex> baseSubset;
vector<SateGraph::NodeIndex> sateSubset;

SateGraph::NodeIndex
getNearSateFromSet(const set<SateGraph::NodeIndex> &recvSateSet,
                   SateGraph::NodeIndex base)
{
    SateGraph::Dist bestDist = SateGraph::kInf;
    SateGraph::NodeIndex bestSate = bGraph.getOrder();
    for (SateGraph::NodeIndex sate : recvSateSet) {
        SateGraph::Dist curDist = bGraph.getDist(sate, base);
        if (curDist < bestDist) {
            bestDist = curDist;
            bestSate = sate;
        }
    }
    return bestSate;
}

Power getPowerSum(const set<SateGraph::NodeIndex> &recvSateSet)
{
    Power PowerSum = 0;
    for (SateGraph::NodeIndex base : baseSubset) {
        SateGraph::NodeIndex sate = getNearSateFromSet(recvSateSet, base);
        PowerSum += bGraph.getDist(sate, base);
    }
    PowerSum *= kPowerPerDist;
    PowerSum += kPowerPerSite * recvSateSet.size();
    return PowerSum;
}

void removeRedundantSate(set<SateGraph::NodeIndex> &recvSateSet)
{
    vector<size_t> coverCnts(bGraph.getOrder(), 0);
    for (auto sate : recvSateSet) {
        for (auto base : bGraph.getAdjList()[sate]) {
            ++coverCnts[base];
        }
    }
    for (auto iter = recvSateSet.begin(); iter != recvSateSet.end(); ) {
        auto sate = *iter;
        bool isRedundant = true;
        for (auto base : bGraph.getAdjList()[sate]) {
            if (coverCnts[base] == 1) {
                isRedundant = false;
                break;
            }
        }
        if (isRedundant) {
            for (auto base : bGraph.getAdjList()[sate]) {
                --coverCnts[base];
            }
            recvSateSet.erase(iter++);
        } else {
            ++iter;
        }
    }
}