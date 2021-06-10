#include "Dijkstra.h"

#include <set>
#include <queue>

#include <assert.h>

using namespace std;

Dijkstra::Dijkstra(Graph& graph, Graph::NodeIndex source) 
    : source_(source)
    , last_node_(graph.get_order())
    , dists_(graph.get_order(), Graph::Inf)
{
    assert(source < graph.get_order());

    const Graph::AdjList& adjlist(graph.get_adjlist());
    const Graph::AdjMatrix& adjmatrix(graph.get_adjmatrix());

    vector<bool> is_visited(adjlist.size(), false);
    using PDI = pair<Graph::Dist, Graph::NodeIndex>;
    priority_queue<PDI, vector<PDI>, greater<PDI>> min_heap;
    
    last_node_[source] = source;
    dists_[source] = 0;
    min_heap.push(make_pair(0, source));
    
    while(!min_heap.empty()) {

        PDI top = min_heap.top();
        min_heap.pop();
        Graph::NodeIndex from = top.second;
        if(is_visited[from]) {
            continue;
        }
        is_visited[from] = true;

        for(Graph::NodeIndex to : adjlist[from]) {
            Graph::Dist dist = adjmatrix[from][to];
            if(dists_[to] > dist + dists_[from]) {
                dists_[to] = dist + dists_[from];
                last_node_[to] = from;
                if(is_visited[to] == false) {
                    min_heap.push(make_pair(dists_[to], to));
                }
            }
        }
    }
}

Dijkstra::~Dijkstra()
{
}

void Dijkstra::display_dists() const 
{
    cout << "------------dists------------" << endl;
    for(Graph::Dist dist : dists_) {
        cout << dist << " ";
    }
    cout << endl;
    cout << "-----------------------------" << endl;
}

void Dijkstra::display_route() const
{
    cout << "------------routes-------------" << endl;
    for(Graph::NodeIndex cur = 0; cur < last_node_.size(); ++cur) {
        cout << last_node_[cur] << "->" << cur << endl;
    }
    cout << "-------------------------------" << endl;
}