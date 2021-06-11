#include "Scheme.h"

#include <assert.h>

Scheme::Scheme(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
               const vector<bool>& typeVec, const vector<Edge>& edgeVec)
    : graphs_(Graph::getConnectedGraphVec(N, edgeVec))
    , stations_(graphs_.size())
    , satellites_(graphs_.size())
    , coeff_(C)
    , limit_(D)
    , site_(PS)
{
    for(GraphIndex i = 0; i < graphs_.size(); ++i) {

        const vector<Graph::NodeId> &nodes = graphs_[i].get_nodes();
        satellites_.reserve(nodes.size());
        stations_.reserve(nodes.size());

        for(Graph::NodeIndex j = 0; j < nodes.size(); ++j) {
            if(typeVec[nodes[j]]) {
                satellites_[i].push_back(j);
            } else {
                stations_[i].push_back(j);
            }
        }
    }
}

void Scheme::solveConnected(GraphIndex graph_index) 
{
    assert(graphs_.size() > graph_index);
}