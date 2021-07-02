#include "RouteParser.h"
#include "Dijkstra.h"

RouteParser::RouteParser(const SateGraph &graph)
    : lasts_(graph.getOrder(), graph.getOrder())
{
}

RouteParser::RouteParser(RouteParser &&rhs)
    : lasts_(std::move(rhs.lasts_))
{
}

RouteParser &RouteParser::operator=(RouteParser &&rhs)
{
    using std::swap;
    swap(rhs.lasts_, lasts_);
    return *this;
}

void RouteParser::createRoute(SateGraph::NodeIndex sate,
                              const vector<SateGraph::NodeIndex> &bases)
{
    vector<SateGraph::NodeIndex> path(aGraph.getOrder(), aGraph.getOrder());
    Dijkstra<SateGraph> d(aGraph, klimitDist, path);
    d(sate);
    for (SateGraph::NodeIndex base : bases) {
        while (path[base] != base) {
            assert(path[base] < aGraph.getOrder());
            lasts_[base] = path[base];
            base = lasts_[base];
        }
        lasts_[base] = base;
    }
}

void RouteParser::trimRoute()
{
    const SateGraph::AdjMatrix &adjMatrix = aGraph.getAdjMatrix();

    for (SateGraph::NodeIndex node : baseSubset) {
        SateGraph::Dist sumDist = 0;
        while (lasts_[node] != node) {
            assert(lasts_[node] < aGraph.getOrder());
            sumDist += adjMatrix[node][lasts_[node]];
            if (sumDist > klimitDist) {
                lasts_[node] = node;
                break;
            }
            node = lasts_[node];
        }
    }
}

vector<Route> RouteParser::getRoute()
{
    vector<Route> retRouteVec;
    retRouteVec.reserve(baseSubset.size());

    for (SateGraph::NodeIndex base : baseSubset) {
        retRouteVec.push_back(Route());
        retRouteVec.back().push_back(aGraph.getNodeID(base));
        while (lasts_[base] != base) {
            base = lasts_[base];
            assert(base < aGraph.getOrder());
            retRouteVec.back().push_back(aGraph.getNodeID(base));
        }
    }

    return retRouteVec;
}