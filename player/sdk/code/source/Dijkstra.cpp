#include "Dijkstra.h"

#include <set>
#include <queue>

#include <assert.h>

using namespace std;

Dijkstra::Dijkstra(const Graph& graph, Graph::NodeIndex source, 
                   Graph::Dist limit) 
    : last_(graph.getOrder())
    , dists_(graph.getOrder(), Graph::kInf)
    , reached_()
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
        reached_.push_back(from);

        for(Graph::NodeIndex to : adjList[from]) {
            Graph::Dist newDist = adjMatrix[from][to] + dists_[from];
            if(newDist <= limit && dists_[to] > newDist) {
                dists_[to] = newDist;
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

Graph::Dist Dijkstra::getDistSum(const vector<Graph::NodeIndex>& nodes) const
{
    Graph::Dist sum = 0;
    for(auto node : nodes) {
        assert(getDist(node) != Graph::kInf);
        sum += getDist(node);
    }
    return sum;
}

vector<Graph::NodeIndex> Dijkstra::getRoute(Graph::NodeIndex node) const
{
    vector<Graph::NodeIndex> retRoute;
    retRoute.push_back(node);
    while(last_[node] != node) {
        node = last_[node];
        retRoute.push_back(node);
    }
    return retRoute;
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

void Dijkstra::displayReached() const
{
    cout << "-----------reached------------" << endl;
    for(auto node : reached_) {
        cout << node << " ";
    }
    cout << endl;
    cout << "------------------------------" << endl;
}