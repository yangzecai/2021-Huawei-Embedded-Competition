#include "Solution.h"
#include "Dijkstra.h"
#include "DisJointSet.h"
#include <assert.h>

vector<Route> Solution::Plan(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                             const vector<bool> &typeVec,
                             const vector<Edge> &edgeVec)
{
    initGlobalData(N, C, D, PS, typeVec, edgeVec);
    initMemData();
    
    ACO antColony(1, 7, 0.5, 1, 10);
    antColony.iterate(100);
    set<SateGraph::NodeIndex> bestSateSet = antColony.getBestSets();

    const SateGraph::AdjList &bAdjList = bGraph.getAdjList();
    for (SateGraph::NodeIndex base : baseSubset) {
        SateGraph::Dist bestDist = SateGraph::kInf;
        SateGraph::NodeIndex bestSate = bGraph.getOrder();
        for (SateGraph::NodeIndex sate : bAdjList[base]) {
            if (bestSateSet.find(sate) != bestSateSet.end()) {
                SateGraph::Dist curDist = bGraph.getDist(base, sate);
                if (curDist < bestDist) {
                    bestDist = curDist;
                    bestSate = sate;
                }
            }
        }
        routeParser_.createRoute(bestSate, vector<SateGraph::NodeIndex>{base});
    }
    routeParser_.trimRoute();
    return routeParser_.getRoute();
}

void Solution::initMemData() 
{ 
    assert(aGraph.getOrder() != 0);
    routeParser_ = RouteParser(aGraph);

}

void Solution::initGlobalData(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                              const vector<bool> &typeVec,
                              const vector<Edge> &edgeVec)
{
    kPowerPerDist = C;
    kPowerPerSite = PS;
    klimitDist = D;
    initAGraph(N, typeVec, edgeVec);
    initSubset();
    initBGraph();
}

void Solution::initAGraph(uint32_t N, const vector<bool> &typeVec,
                          const vector<Edge> &edgeVec)
{
    vector<Sate> nodes;
    nodes.reserve(N);
    for (SateGraph::NodeIndex i = 0; i < N; ++i) {
        nodes.push_back(Sate{static_cast<Sate::ID>(i), typeVec[i]});
    }

    vector<SateGraph::Edge> edges;
    edges.reserve(edgeVec.size());
    for (Edge edge : edgeVec) {
        edges.push_back(SateGraph::Edge{edge.send, edge.recv, edge.dist});
    }

    aGraph = SateGraph(nodes, edges);
}

void Solution::initSubset()
{
    assert(aGraph.getOrder() != 0);

    for (SateGraph::NodeIndex i = 0; i < aGraph.getOrder(); ++i) {
        if (aGraph.getNode(i).isSate) {
            sateSubset.push_back(i);
        } else {
            baseSubset.push_back(i);
        }
    }
}

void Solution::initBGraph()
{
    assert(aGraph.getOrder() != 0);
    assert(!sateSubset.empty());

    const vector<Sate> &nodes = aGraph.getNodes();
    bGraph = SateGraph(nodes);

    vector<SateGraph::NodeIndex> reached;
    Dijkstra<SateGraph> d1(aGraph, klimitDist, Dijkstra<SateGraph>::unused_,
                           reached);
    for (auto sate : sateSubset) {
        vector<SateGraph::Dist> dists(d1(sate));
        for (SateGraph::NodeIndex node : reached) {
            if (!bGraph.getNode(node).isSate) {
                bGraph.addEdge(SateGraph::Edge{sate, node, dists[node]});
            }
        }
        reached.clear();
    }
}

void Solution::test(void)
{
    ACO test(1, 7, 0.5, 1, 100);
    test.displayCurValue();
    for (int i = 0; i < 100; ++i) {
        test.iterate(1);
        test.displayCurValue();
    }
    // test.iterate(100);
    // test.displayCurValue();
}