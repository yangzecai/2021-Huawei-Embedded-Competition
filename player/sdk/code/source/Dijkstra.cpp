#include "Dijkstra.h"

#include <set>
#include <queue>

#include <assert.h>

using namespace std;

Dijkstra::Dijkstra(Graph& graph, Graph::NodeIndex source) 
    : source_(source)
    , last_(graph.getOrder())
    , dists_(graph.getOrder(), Graph::Inf)
{
    assert(source < graph.getOrder());

    const Graph::AdjList& adjList(graph.getAdjList());
    const Graph::AdjMatrix& adjMatrix(graph.getAdjMatrix());

    vector<bool> isVisited(adjList.size(), false);
    using PDI = pair<Graph::Dist, Graph::NodeIndex>;
    priority_queue<PDI, vector<PDI>, greater<PDI>> minHeap;
    
    last_[source] = source;
    dists_[source] = 0;
    minHeap.push(make_pair(0, source));
    
    while(!minHeap.empty()) {

        PDI top = minHeap.top();
        minHeap.pop();
        Graph::NodeIndex from = top.second;
        if(isVisited[from]) {
            continue;
        }
        isVisited[from] = true;

        for(Graph::NodeIndex to : adjList[from]) {
            Graph::Dist dist = adjMatrix[from][to];
            if(dists_[to] > dist + dists_[from]) {
                dists_[to] = dist + dists_[from];
                last_[to] = from;
                if(isVisited[to] == false) {
                    minHeap.push(make_pair(dists_[to], to));
                }
            }
        }
    }
}

Dijkstra::~Dijkstra()
{
}

void Dijkstra::displayDists() const 
{
    cout << "------------dists------------" << endl;
    for(Graph::Dist dist : dists_) {
        cout << dist << " ";
    }
    cout << endl;
    cout << "-----------------------------" << endl;
}

void Dijkstra::displayRoute() const
{
    cout << "------------routes-------------" << endl;
    for(Graph::NodeIndex cur = 0; cur < last_.size(); ++cur) {
        cout << last_[cur] << "->" << cur << endl;
    }
    cout << "-------------------------------" << endl;
}