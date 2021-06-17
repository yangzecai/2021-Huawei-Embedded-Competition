#pragma once

#include "DisJointSet.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <algorithm>

// namespace my {

using namespace std;

template<typename Node>
class Graph {
public:
    using NodeIndex = typename vector<Node>::size_type;
    using Dist = uint32_t;
    struct Edge {
        NodeIndex send;
        NodeIndex recv;
        Dist      dist;
    };
    using Route = vector<NodeIndex>;
    using AdjList = vector<vector<NodeIndex>>;
    using AdjMatrix = vector<vector<Dist>>;
    using Color = uint32_t;

    static const Dist kInf;

    Graph(const vector<Node> &nodes = vector<Node>(),
          const vector<Edge> &edges = vector<Edge>());
    virtual ~Graph() = default;

    Graph(const Graph&) = delete;
    Graph& operator= (const Graph&) = delete;

    void swap(Graph &&rhs);

    Graph(Graph &&rhs);                 //move
    Graph& operator= (Graph &&rhs);

    vector<Graph<Node>> getConnSubgraphs() const;

    void addNode(const Node &node);
    void addNode(Node &&node);
    const vector<Node> &getNodes() const 
    { return nodes_; }
    const Node &getNode(NodeIndex i) const
    { return nodes_[i]; }
    typename Node::ID getNodeID(NodeIndex i) const 
    { return nodes_[i].id; }

    void addEdge(const Edge &edge);
    void addEdge(Edge &&edge);
    const vector<Edge> &getEdges() const
    { return edges_; }

    size_t getOrder() const
    { return nodes_.size(); }

    size_t getDegree(NodeIndex i) const
    { updateAdjList(); return adjList_[i].size(); }
    size_t getMaxDegree() const;

    Dist getDist(NodeIndex i, NodeIndex j) const
    { updateAdjMatrix(); return adjMatrix_[i][j]; }

    const AdjList& getAdjList() const;
    const AdjList& getAdjListSorted() const;
    const AdjMatrix& getAdjMatrix() const;
    const vector<Color>& getColors() const;

    Graph getGraphBar() const;

    void displayNodesID() const;    // for debug
    void displayAdjList() const;    // for debug
    void displayAdjListID() const;  // for debug
    void displayColors() const;     // for debug

private:
    vector<Node> nodes_;
    vector<Edge> edges_;

    mutable bool isAdjListNew_;
    mutable AdjList adjList_;

    mutable bool isAdjMatrixNew_;
    mutable AdjMatrix adjMatrix_;

    mutable bool isColorsNew_;
    mutable vector<Color> colors_;

    void updateAdjList() const;
    void updateAdjMatrix() const;
    void updateColors() const;

    void sortAdjList();
};  // class Graph

// }   // namespace my


/*************************************define************************************/

// namespace my {

template<typename Node>
const typename Graph<Node>::Dist Graph<Node>::kInf = UINT32_MAX;

template<typename Node>
Graph<Node>::Graph(const vector<Node> &nodes, 
                   const vector<Edge> &edges) 
    : nodes_(nodes)
    , edges_(edges) 
    , isAdjListNew_(false)
    , adjList_()
    , isAdjMatrixNew_(false)
    , adjMatrix_()
    , isColorsNew_(false)
    , colors_()
{
}

template<typename Node>
void Graph<Node>::swap(Graph &&rhs) 
{
    using std::swap;

    swap(this->nodes_, rhs.nodes_);
    swap(this->edges_, rhs.edges_);

    swap(this->isAdjListNew_, rhs.isAdjListNew_);
    swap(this->adjList_, rhs.adjList_);

    swap(this->isAdjMatrixNew_, rhs.isAdjMatrixNew_);
    swap(this->adjMatrix_, rhs.adjMatrix_);

    swap(this->isColorsNew_, rhs.isColorsNew_);
    swap(this->colors_, rhs.colors_);
}

template<typename Node>
Graph<Node>::Graph(Graph &&rhs)
    : nodes_(std::move(rhs.nodes_))
    , edges_(std::move(rhs.edges_))
    , isAdjListNew_(rhs.isAdjListNew_)
    , adjList_(std::move(rhs.adjList_))
    , isAdjMatrixNew_(rhs.isAdjMatrixNew_)
    , adjMatrix_(std::move(rhs.adjMatrix_))
    , isColorsNew_(rhs.isColorsNew_)
    , colors_(std::move(rhs.colors_))
{   
}

template<typename Node>
Graph<Node>& Graph<Node>::operator= (Graph &&rhs) 
{
    swap(std::move(rhs));
    return *this;
}

template<typename Node>
vector<Graph<Node>> Graph<Node>::getConnSubgraphs() const
{
    using GraphIndex = typename vector<Graph<Node>>::size_type;
    
    DisJointSet djs(getOrder());

    vector<Graph> retSubgraphs;
    unordered_map<typename Node::ID, GraphIndex> graphIndexMap;  //Node::ID to subgraphIndex
    vector<NodeIndex> nodeIndexMap(getOrder());      //Node::ID to NodeIndex in subgraph
    
    // 求连通节点集合
    for(auto iter = edges_.cbegin(); iter != edges_.cend(); ++iter) {
        djs.merge(iter->send, iter->recv);
    }
    
    // 将节点添加到相应连通图中
    for(typename Node::ID i = 0; i < getOrder(); ++i) {
        typename Node::ID setID = djs.find(i);
        if(graphIndexMap.find(setID) == graphIndexMap.end()) {  //还未创建所在连通子图
            graphIndexMap[setID] = retSubgraphs.size();
            retSubgraphs.push_back(Graph());
        }
        GraphIndex graphIndex = graphIndexMap[setID];
        NodeIndex nodeIndex = retSubgraphs[graphIndex].getOrder();
        nodeIndexMap[i] = nodeIndex;
        retSubgraphs[graphIndex].addNode(nodes_[i]);
    }

    // 将边添加到相应的连通图中
    for(auto iter = edges_.cbegin(); iter != edges_.cend(); ++iter) {
        GraphIndex graphIndex = graphIndexMap[djs.find(iter->send)];
        NodeIndex send = nodeIndexMap[iter->send];
        NodeIndex recv = nodeIndexMap[iter->recv];
        retSubgraphs[graphIndex].addEdge({send, recv, iter->dist});
    }
    return retSubgraphs;
}

template<typename Node>
void Graph<Node>::addNode(const Node &node) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    isColorsNew_ = false;
    nodes_.push_back(node);
}

template<typename Node>
void Graph<Node>::addNode(Node &&node) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    isColorsNew_ = false;
    nodes_.push_back(std::move(node));
}

template<typename Node>
void Graph<Node>::addEdge(const Edge &edge) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    isColorsNew_ = false;
    edges_.push_back(edge);
}

template<typename Node>
void Graph<Node>::addEdge(Edge &&edge) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    isColorsNew_ = false;
    edges_.push_back(std::move(edge));
}

template<typename Node>
size_t Graph<Node>::getMaxDegree() const
{
    updateAdjList();
    size_t ret = 0;
    for(auto& item : adjList_) {
        ret = max(ret, item.size());
    }
    return ret;
}

template<typename Node>
const typename Graph<Node>::AdjList& Graph<Node>::getAdjList() const
{
    updateAdjList();
    return adjList_;
}

template<typename Node>
const typename Graph<Node>::AdjList& Graph<Node>::getAdjListSorted() const
{
    updateAdjList();
    for(auto& item : adjList_) {
        sort(item.begin(), item.end());
    }
    return adjList_;
}

template<typename Node>
const typename Graph<Node>::AdjMatrix& Graph<Node>::getAdjMatrix() const
{
    updateAdjMatrix();
    return adjMatrix_;
}

template<typename Node>
const vector<typename Graph<Node>::Color>& Graph<Node>::getColors() const
{
    updateColors();
    return colors_;
}

template<typename Node>
void Graph<Node>::updateAdjList() const
{
    if(!isAdjListNew_) {
        adjList_ = AdjList(getOrder());
        for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
            adjList_[iter->send].push_back(iter->recv);
            adjList_[iter->recv].push_back(iter->send);
        }
        isAdjListNew_ = true;
    }
}

template<typename Node>
void Graph<Node>::updateAdjMatrix() const
{
    if(!isAdjMatrixNew_) {
        adjMatrix_ = AdjMatrix(getOrder(), vector<Dist>(getOrder(), kInf));
        for(NodeIndex i = 0; i < nodes_.size(); ++i) {
            adjMatrix_[i][i] = 0;
        }
        for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
            adjMatrix_[iter->send][iter->recv] = iter->dist;
            adjMatrix_[iter->recv][iter->send] = iter->dist;
        }
        isAdjMatrixNew_ = true;
    }
}

template<typename Node>
void Graph<Node>::updateColors() const
{
    if(!isColorsNew_) {
        colors_ = vector<Color>(getOrder());
        size_t maxColor = getMaxDegree() + 1;
        getAdjListSorted();

        for(NodeIndex node = 0; node < colors_.size(); ++node) {

            vector<bool> usedColor(maxColor, false);
            for(NodeIndex adjNode : adjList_[node]) {
                if(node > adjNode) {
                    usedColor[colors_[adjNode]] = true;
                } else {
                    auto iter = find(usedColor.begin(), usedColor.end(), false);
                    colors_[node] = iter - usedColor.begin();
                    break;
                }
            }
        }
    }
}

template<typename Node>
Graph<Node> Graph<Node>::getGraphBar() const 
{
    Graph retGraph(nodes_);

    updateAdjMatrix();
    for(NodeIndex i = 0; i < adjMatrix_.size(); ++i) {
        for(NodeIndex j = i+1; j < adjMatrix_[0].size(); ++j) {
            if(adjMatrix_[i][j] == kInf) {
                retGraph.addEdge({i, j, 1});
            }
        }
    }
    return retGraph;
}

template<typename Node>
void Graph<Node>::displayNodesID() const
{
    cout << "------------nodes_id------------" << endl;
    for(auto &n : nodes_) {
        cout << n.id << " ";
    }
    cout << endl;
    cout << "--------------------------------" << endl;
}

template<typename Node>
void Graph<Node>::displayAdjList() const
{
    updateAdjList();
    cout << "------------adjlist------------" << endl;
    for(NodeIndex i = 0; i < adjList_.size(); ++i) {
        cout << i << " : ";
        for(const NodeIndex j : adjList_[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------" << endl;
}

template<typename Node>
void Graph<Node>::displayAdjListID() const
{
    updateAdjList();
    cout << "-----------adjList_id----------" << endl;
    for(NodeIndex i = 0; i < adjList_.size(); ++i) {
        cout << getNodeID(i) << " : ";
        for(const NodeIndex j : adjList_[i]) {
            cout << getNodeID(j) << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------" << endl;
}

template<typename Node>
void Graph<Node>::displayColors() const
{
    updateColors();
    cout << "-------------colors--------------" << endl;
    for(Color color : colors_) {
        cout << color << " ";
    }
    cout << endl;
    cout << "---------------------------------" << endl;
}

// } // namespace my