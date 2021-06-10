#include "Graph.h"
#include "UnionFindSet.h"

#include <unordered_map>

Graph::Graph() 
    : nodes_()
    , edges_() 
{
}

Graph::~Graph() 
{
}

void Graph::swap(Graph&& rhs) 
{
    using std::swap;
    swap(this->nodes_, rhs.nodes_);
    swap(this->edges_, rhs.edges_);
}

Graph::Graph(Graph&& rhs)
    : nodes_(std::move(rhs.nodes_))
    , edges_(std::move(rhs.edges_))
{   
}

Graph& Graph::operator= (Graph&& rhs) 
{
    swap(std::move(rhs));
    return *this;
}

void Graph::add_node(const Node &node) 
{
    nodes_.push_back(node);
}

void Graph::add_node(Node&& node) 
{
    nodes_.push_back(std::move(node));
}

void Graph::add_edge(const Edge &edge) 
{
    edges_.push_back(edge);
}

void Graph::add_edge(Edge&& edge) 
{
    edges_.push_back(std::move(edge));
}

std::vector<Graph> Graph::getConnectedGraphVec(uint32_t N,
                                               const vector<bool>& typeVec,
                                               const vector<Edge>& edgeVec)
{
    vector<Graph> retGraphVec;
    unordered_map<id_t, id_t> graphIdMap;
    UnionFindSet ufs(N);
    
    // 求连通节点集合
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        ufs.merge(iter->send, iter->recv);
    }
    
    // 将节点添加到相应连通图中
    for(id_t i = 0; i < N; ++i) {
        id_t set_id = ufs.find(i);
        if(graphIdMap.find(set_id) == graphIdMap.end()) {
            graphIdMap[set_id] = retGraphVec.size();
            retGraphVec.push_back(Graph());
        }
        id_t graphId = graphIdMap[set_id];
        retGraphVec[graphId].add_node(Node{i, typeVec[i]});
    }

    // 将边添加到相应的连通图中
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        id_t graphId = graphIdMap[ufs.find(iter->send)];
        retGraphVec[graphId].add_edge(*iter);
    }

    return retGraphVec;
}




void Graph::show_adjTable() const 
{
    std::unordered_map<id_t, vector<id_t>> adjTable;

    for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
        adjTable[iter->send].push_back(iter->recv);
        adjTable[iter->recv].push_back(iter->send);
    }

    std::cout << "-------------------------------" << std::endl;
    for(auto iter = adjTable.begin(); iter != adjTable.end(); ++iter) {
        std::cout << iter->first << " : ";
        for(auto vecIter = iter->second.begin(); 
            vecIter != iter->second.end(); ++vecIter) {
            std::cout << *vecIter << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}