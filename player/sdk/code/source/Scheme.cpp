#include "Scheme.h"

#include <algorithm>

#include <assert.h>

Scheme::Scheme(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
               const vector<bool>& typeVec, const vector<Edge>& edgeVec)
    : graphs_(Graph::getConnectedGraphVec(N, edgeVec))
    , stations_(graphs_.size())
    , satellites_(graphs_.size())
    , coeff_(C)
    , limit_(D)
    , site_(PS)
    , routes_(graphs_.size(), vector<Graph::Route>())
{
    for(GraphIndex i = 0; i < graphs_.size(); ++i) {    //分离卫星和基站

        const vector<Graph::NodeId> &nodes = graphs_[i].getNodes();
        satellites_[i].reserve(nodes.size());
        stations_[i].reserve(nodes.size());

        for(Graph::NodeIndex j = 0; j < nodes.size(); ++j) {
            if(typeVec[nodes[j]]) {
                satellites_[i].push_back(j);
            } else {
                stations_[i].push_back(j);
            }
        }
    }
}

Scheme::~Scheme()
{
}

void Scheme::solveConnected(GraphIndex graphIndex) 
{
    assert(graphs_.size() > graphIndex);

    Graph& graph = graphs_[graphIndex];
    const vector<Graph::NodeIndex>& stations = stations_[graphIndex];
    const vector<Graph::NodeIndex>& satellites = stations_[graphIndex];

    vector<Graph::Dist> dists;
    dists.reserve(satellites.size());

    for(Graph::NodeIndex satellite : satellites) {
        Dijkstra djks(graph, satellite);
        dists.push_back(djks.getDistSum(stations));
    }

    Graph::NodeIndex recvSate = 
        min_element(dists.cbegin(), dists.cend()) - dists.begin();

    vector<Graph::Route>& routes = routes_[graphIndex];
    Dijkstra djks(graph, recvSate);
    for(auto station : stations) {
        routes.push_back(std::move(djks.getRoute(station)));
    }
}

void Scheme::solve()
{
    for(GraphIndex i = 0; i < graphs_.size(); ++i) {
        solveConnected(i);
    }
}

vector<::Route> Scheme::getRouteVec()
{
    vector<::Route> retRouteVec;
    for(GraphIndex i = 0; i < routes_.size(); ++i) {
        for(RouteIndex j = 0; j < routes_[i].size(); ++j) {
            retRouteVec.push_back(::Route());
            Graph::Route& route = routes_[i][j];
            for(Graph::NodeIndex k : route) {
                retRouteVec.back().push_back(graphs_[i].getNodeId(k));
            }
        }
    }
    return retRouteVec;
}