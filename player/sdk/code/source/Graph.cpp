#include "Graph.h"
#include "UnionFindSet.h"

#include <unordered_map>

using namespace std;

const Graph::Dist Graph::Inf = UINT32_MAX;

Graph::Graph() 
    : nodes_()
    , edges_() 
    , isAdjListNew_(true)
    , adjList_()
    , isAdjMatrixNew_(true)
    , adjMatrix_()
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

    swap(this->isAdjListNew_, rhs.isAdjListNew_);
    swap(this->adjList_, rhs.adjList_);

    swap(this->isAdjMatrixNew_, rhs.isAdjMatrixNew_);
    swap(this->adjMatrix_, rhs.adjMatrix_);
}

Graph::Graph(Graph&& rhs)
    : nodes_(std::move(rhs.nodes_))
    , edges_(std::move(rhs.edges_))
    , isAdjListNew_(rhs.isAdjListNew_)
    , adjList_(std::move(rhs.adjList_))
    , isAdjMatrixNew_(rhs.isAdjMatrixNew_)
    , adjMatrix_(std::move(rhs.adjMatrix_))
{   
}

Graph& Graph::operator= (Graph&& rhs) 
{
    swap(std::move(rhs));
    return *this;
}

vector<Graph> Graph::getConnectedGraphVec(uint32_t N,
                                          const vector<::Edge>& edgeVec)
{
    using GraphIndex = vector<Graph>::size_type;

    UnionFindSet ufs(N);
    vector<Graph> retGraphVec;
    unordered_map<NodeId, GraphIndex> graphIndexMap;  //NodeId to graphIndex
    vector<NodeIndex> nodeIndexMap(N);   //NodeId to NodeIndex
    
    // 求连通节点集合
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        ufs.merge(iter->send, iter->recv);
    }
    
    // 将节点添加到相应连通图中
    for(NodeId i = 0; i < N; ++i) {
        NodeId setId = ufs.find(i);
        if(graphIndexMap.find(setId) == graphIndexMap.end()) {
            graphIndexMap[setId] = retGraphVec.size();
            retGraphVec.push_back(Graph());
        }
        GraphIndex graphIndex = graphIndexMap[setId];
        NodeIndex nodeIndex = retGraphVec[graphIndex].getNodes().size();
        nodeIndexMap[i] = nodeIndex;
        retGraphVec[graphIndex].addNode(i);
    }

    // 将边添加到相应的连通图中
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        NodeIndex send = nodeIndexMap[iter->send];
        NodeIndex recv = nodeIndexMap[iter->recv];
        GraphIndex graphIndex = graphIndexMap[ufs.find(iter->send)];
        retGraphVec[graphIndex].addEdge(Edge{send, recv, iter->dist});
    }
    return retGraphVec;
}


void Graph::addNode(const NodeId &node) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    nodes_.push_back(node);
}

void Graph::addNode(NodeId&& node) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    nodes_.push_back(std::move(node));
}

void Graph::addEdge(const Edge &edge) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    edges_.push_back(edge);
}

void Graph::addEdge(Edge&& edge) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    edges_.push_back(std::move(edge));
}

const Graph::AdjList& Graph::getAdjList() 
{
    updateAdjList();
    return adjList_;
}

const Graph::AdjMatrix& Graph::getAdjMatrix()
{
    updateAdjMatrix();
    return adjMatrix_;
}

void Graph::updateAdjList() 
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

void Graph::updateAdjMatrix()
{
    if(!isAdjMatrixNew_) {
        adjMatrix_ = AdjMatrix(getOrder(), vector<Dist>(getOrder(), Inf));
        for(auto iter = edges_.begin(); iter != edges_.end(); ++iter) {
            adjMatrix_[iter->send][iter->recv] = iter->dist;
            adjMatrix_[iter->recv][iter->send] = iter->dist;
        }
        isAdjMatrixNew_ = true;
    }
}

void Graph::displayNodesId() const
{
    cout << "------------nodes_id------------" << endl;
    for(auto &n : nodes_) {
        cout << n << " ";
    }
    cout << endl;
    cout << "--------------------------------" << endl;
}

void Graph::displayAdjList()
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

void Graph::displayAdjListId()
{
    updateAdjList();
    cout << "-----------adjList_id----------" << endl;
    for(NodeIndex i = 0; i < adjList_.size(); ++i) {
        cout << getNodeId(i) << " : ";
        for(const NodeIndex j : adjList_[i]) {
            cout << getNodeId(j) << " ";
        }
        cout << endl;
    }
    cout << "-------------------------------" << endl;
}
