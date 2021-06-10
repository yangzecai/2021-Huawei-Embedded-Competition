#include "Graph.h"
#include "UnionFindSet.h"

#include <unordered_map>

using namespace std;

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

const vector<Graph::Node>& Graph::get_nodes() const 
{
    return nodes_;
}

void Graph::add_edge(const Edge &edge) 
{
    edges_.push_back(edge);
}

void Graph::add_edge(Edge&& edge) 
{
    edges_.push_back(std::move(edge));
}

const vector<Graph::Edge>& Graph::get_edges() const 
{
    return edges_;
}

vector<Graph> Graph::getConnectedGraphVec(uint32_t N,
                                          const vector<bool>& typeVec,
                                          const vector<::Edge>& edgeVec)
{
    using GraphIndex = vector<Graph>::size_type;

    UnionFindSet ufs(N);
    vector<Graph> ret_graph_vec;
    unordered_map<NodeId, GraphIndex> graph_index_map;  //NodeId to graphIndex
    vector<NodeIndex> node_index_map(N);   //NodeId to NodeIndex
    
    // 求连通节点集合
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        ufs.merge(iter->send, iter->recv);
    }
    
    // 将节点添加到相应连通图中
    for(NodeId i = 0; i < N; ++i) {
        NodeId set_id = ufs.find(i);
        if(graph_index_map.find(set_id) == graph_index_map.end()) {
            graph_index_map[set_id] = ret_graph_vec.size();
            ret_graph_vec.push_back(Graph());
        }
        GraphIndex graph_index = graph_index_map[set_id];
        NodeIndex node_index = ret_graph_vec[graph_index].get_nodes().size();
        node_index_map[i] = node_index;
        ret_graph_vec[graph_index].add_node(Node{i, typeVec[i]});
    }

    // 将边添加到相应的连通图中
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        NodeIndex send = node_index_map[iter->send];
        NodeIndex recv = node_index_map[iter->recv];
        GraphIndex graph_index = graph_index_map[ufs.find(iter->send)];
        ret_graph_vec[graph_index].add_edge(Edge{send, recv, iter->dist});
    }
    return ret_graph_vec;
}




void Graph::show_adjTable() const 
{
    std::unordered_map<NodeId, vector<NodeId>> adjTable;

    for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
        adjTable[iter->send].push_back(iter->recv);
        adjTable[iter->recv].push_back(iter->send);
    }

    std::cout << "-------------------------------" << std::endl;
    for(auto iter = adjTable.begin(); iter != adjTable.end(); ++iter) {
        std::cout << nodes_[iter->first].id << " : ";
        for(auto vecIter = iter->second.begin(); 
            vecIter != iter->second.end(); ++vecIter) {
            std::cout << nodes_[*vecIter].id << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;
}