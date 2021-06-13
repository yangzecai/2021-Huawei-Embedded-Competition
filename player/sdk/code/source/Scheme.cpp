#include "Scheme.h"

#include <algorithm>

#include <assert.h>

Scheme::Scheme(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
               const vector<bool>& typeVec, const vector<Edge>& edgeVec)
    : schemeGraphs_()
    , coeff_(C)
    , limit_(D)
    , site_(PS)
{
    vector<Graph> graphs = Graph::getConnectedGraphVec(N, edgeVec);
    for(Graph& graph : graphs) {
        schemeGraphs_.push_back(SchemeGraph{std::move(graph),
                                            vector<Graph::NodeIndex>(),
                                            vector<Graph::NodeIndex>(),
                                            vector<Graph::Route>()});
    }

    for(SGIndex i = 0; i < schemeGraphs_.size(); ++i) {    //分离卫星和基站

        const vector<Graph::NodeId> &nodes = getGraph(i).getNodes();
        getStats(i).reserve(nodes.size());
        getSates(i).reserve(nodes.size());

        for(Graph::NodeIndex j = 0; j < nodes.size(); ++j) {
            if(typeVec[nodes[j]]) {
                getSates(i).push_back(j);
            } else {
                getStats(i).push_back(j);
            }
        }
    }
}

Scheme::~Scheme()
{
}

void Scheme::solveConnected(SGIndex sgIndex) 
{
    assert(schemeGraphs_.size() > sgIndex);

    const Graph& graph = getGraph(sgIndex);
    const vector<Graph::NodeIndex>& stations = getStats(sgIndex);
    const vector<Graph::NodeIndex>& satellites = getSates(sgIndex);

    vector<Graph::Dist> dists;
    dists.reserve(satellites.size());

    for(Graph::NodeIndex satellite : satellites) {
        Dijkstra djks(graph, satellite);
        dists.push_back(djks.getDistSum(stations));
    }

    Graph::NodeIndex recvSate = 
        min_element(dists.cbegin(), dists.cend()) - dists.begin();

    vector<Graph::Route>& routes = getRoutes(sgIndex);
    Dijkstra djks(graph, getSates(sgIndex)[recvSate]);

    // for fitting constraint
    Graph::AdjMatrix adjMatrix = getGraph(sgIndex).getAdjMatrix();
    vector<Graph::NodeIndex>& lasts = djks.getLast();
    for(Graph::NodeIndex node : stations) {
        Graph::Dist dist = 0;
        // cout << "---------route----------" << endl;
        // cout << node << " ";
        while(lasts[node] != node) {
            // cout << "(" << adjMatrix[node][lasts[node]] << ") ";
            dist += adjMatrix[node][lasts[node]];
            if(dist > limit_) {
                lasts[node] = node;
                break;
            }
            node = lasts[node];
            // cout << node << " ";
        }
        // cout << endl;
        // cout << "------------------------" << endl;
    }

    for(auto station : stations) {
        routes.push_back(std::move(djks.getRoute(station)));
    }
}

void Scheme::solve()
{
    for(SGIndex i = 0; i < schemeGraphs_.size(); ++i) {
        solveConnected(i);
    }
}

vector<::Route> Scheme::parseRoutes()
{
    vector<::Route> retRouteVec;
    for(SGIndex i = 0; i < schemeGraphs_.size(); ++i) {
        for(RouteIndex j = 0; j < getRoutes(i).size(); ++j) {
            retRouteVec.push_back(::Route());
            Graph::Route& route = getRoutes(i)[j];
            for(Graph::NodeIndex k : route) {
                retRouteVec.back().push_back(getGraph(i).getNodeId(k));
            }
        }
    }
    return retRouteVec;
}
