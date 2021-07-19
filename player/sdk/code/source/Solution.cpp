#include "Solution.h"
#include "Dijkstra.h"
#include "ACO.h"
#include <assert.h>

vector<Route> Solution::Plan(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                             const vector<bool> &typeVec,
                             const vector<int> &pwrVec,
                             const vector<Edge> &edgeVec)   // FIXME : pwrVec unused
{
    initGlobalData(N, C, D, PS, typeVec, pwrVec, edgeVec);
    initMemData();
    srand((unsigned) time(NULL));
    ACO antColony(1, 10, 0.1, 0.001, 10);
    antColony.iterate(1000);
    // for (int i = 0; i < 1000; ++i) {
    //     antColony.iterate(1);
    //     minRecvSateSet_ = antColony.getMinRecvSateSet();
    //     cout << antColony.getMinPowerSum()
    //          << " size : " << antColony.getMinRecvSateSet().size() << endl;
    // }
    minRecvSateSet_ = antColony.getMinRecvSateSet();
    connBaseFromSet();
    routeParser_.trimRoute();
    return routeParser_.getRoute();
}

void Solution::connBaseFromSet()
{
    for (SateGraph::NodeIndex base : baseSubset) {
        SateGraph::NodeIndex sate = getNearSateFromSet(minRecvSateSet_, base);
        routeParser_.createRoute(sate, vector<SateGraph::NodeIndex>{base});
    }
}

void Solution::initMemData()
{
    assert(aGraph.getOrder() != 0);
    routeParser_ = RouteParser(aGraph);
}

void Solution::initGlobalData(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                              const vector<bool> &typeVec,
                              const vector<int> &pwrVec,
                              const vector<Edge> &edgeVec)
{
    kPowerPerDist = C;
    kPowerPerSite = PS;
    klimitDist = D;
    initAGraph(N, typeVec, edgeVec);
    initSubset();
    initBGraph();
    Csites = pwrVec;
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
