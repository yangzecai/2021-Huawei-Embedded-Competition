#include "Graph.h"
#include "UnionFindSet.h"

#include <unordered_map>

using namespace std;

Graph::Graph() 
    : nodes_()
    , edges_() 
    , is_adjlist_new_(true)
    , adjlist_()
    , is_adjmatrix_new_(true)
    , adjmatrix_()
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

    swap(this->is_adjlist_new_, rhs.is_adjlist_new_);
    swap(this->adjlist_, rhs.adjlist_);

    swap(this->is_adjmatrix_new_, rhs.is_adjmatrix_new_);
    swap(this->adjmatrix_, rhs.adjmatrix_);
}

Graph::Graph(Graph&& rhs)
    : nodes_(std::move(rhs.nodes_))
    , edges_(std::move(rhs.edges_))
    , is_adjlist_new_(rhs.is_adjlist_new_)
    , adjlist_(std::move(rhs.adjlist_))
    , is_adjmatrix_new_(rhs.is_adjmatrix_new_)
    , adjmatrix_(std::move(rhs.adjmatrix_))
{   
}

Graph& Graph::operator= (Graph&& rhs) 
{
    swap(std::move(rhs));
    return *this;
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


void Graph::add_node(const Node &node) 
{
    is_adjlist_new_ = false;
    is_adjmatrix_new_ = false;
    nodes_.push_back(node);
}

void Graph::add_node(Node&& node) 
{
    is_adjlist_new_ = false;
    is_adjmatrix_new_ = false;
    nodes_.push_back(std::move(node));
}

void Graph::add_edge(const Edge &edge) 
{
    is_adjlist_new_ = false;
    is_adjmatrix_new_ = false;
    edges_.push_back(edge);
}

void Graph::add_edge(Edge&& edge) 
{
    is_adjlist_new_ = false;
    is_adjmatrix_new_ = false;
    edges_.push_back(std::move(edge));
}

const Graph::AdjList& Graph::get_adjlist() 
{
    update_adjlist();
    return adjlist_;
}

const Graph::AdjMatrix& Graph::get_adjmatrix()
{
    update_adjmatrix();
    return adjmatrix_;
}

void Graph::update_adjlist() 
{
    if(!is_adjlist_new_) {
        adjlist_ = AdjList(get_order());
        for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
            adjlist_[iter->send].push_back(iter->recv);
            adjlist_[iter->recv].push_back(iter->send);
        }
        is_adjlist_new_ = true;
    }
}

void Graph::update_adjmatrix()
{
    if(!is_adjmatrix_new_) {
        adjmatrix_ = AdjMatrix(get_order(), vector<bool>(get_order(), false));
        for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
            adjmatrix_[iter->send][iter->recv] = true;
            adjmatrix_[iter->recv][iter->send] = true;
        }
        is_adjmatrix_new_ = true;
    }
}

void Graph::display_ndoes_id() const
{
    cout << "------------nodes_id------------" << endl;
    for(auto &n : nodes_) {
        cout << n.id << " ";
    }
    cout << endl;
    cout << "--------------------------------" << endl;
}

void Graph::display_adjlist()
{
    update_adjlist();
    cout << "------------adjlist------------" << endl;
    for(NodeIndex i = 0; i < adjlist_.size(); ++i) {
        cout << i << " : ";
        for(const NodeIndex j : adjlist_[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------" << endl;
}

void Graph::display_adjlist_id()
{
    update_adjlist();
    cout << "-----------adjlist_id----------" << endl;
    for(NodeIndex i = 0; i < adjlist_.size(); ++i) {
        cout << get_node_id(i) << " : ";
        for(const NodeIndex j : adjlist_[i]) {
            cout << get_node_id(j) << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------" << endl;
}
