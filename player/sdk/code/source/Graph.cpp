#include "Graph.h"
#include "UnionFindSet.h"

#include <unordered_map>
#include <algorithm>
#include <map>

using namespace std;

const Graph::Dist Graph::kInf = UINT32_MAX;

Graph::Graph(vector<NodeId> nodes, vector<Edge> edges) 
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

    swap(this->isColorsNew_, rhs.isColorsNew_);
    swap(this->colors_, rhs.colors_);
}

Graph::Graph(Graph&& rhs)
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
    isColorsNew_ = false;
    nodes_.push_back(node);
}

void Graph::addNode(NodeId&& node) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    isColorsNew_ = false;
    nodes_.push_back(std::move(node));
}

void Graph::addEdge(const Edge &edge) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    isColorsNew_ = false;
    edges_.push_back(edge);
}

void Graph::addEdge(Edge&& edge) 
{
    isAdjListNew_ = false;
    isAdjMatrixNew_ = false;
    isColorsNew_ = false;
    edges_.push_back(std::move(edge));
}

size_t Graph::getMaxDegree() const
{
    updateAdjList();
    size_t ret = 0;
    for(auto& item : adjList_) {
        ret = max(ret, item.size());
    }
    return ret;
}

const Graph::AdjList& Graph::getAdjList() const
{
    updateAdjList();
    return adjList_;
}

const Graph::AdjList& Graph::getAdjListSorted() const
{
    updateAdjList();
    for(auto& item : adjList_) {
        sort(item.begin(), item.end());
    }
    return adjList_;
}

const Graph::AdjMatrix& Graph::getAdjMatrix() const
{
    updateAdjMatrix();
    return adjMatrix_;
}

const vector<Graph::Color>& Graph::getColors() const
{
    updateColors();
    return colors_;
}

void Graph::updateAdjList() const
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

void Graph::updateAdjMatrix() const
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

void Graph::updateColors() const
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

Graph Graph::getGraphBar() const 
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

void Graph::displayNodesId() const
{
    cout << "------------nodes_id------------" << endl;
    for(auto &n : nodes_) {
        cout << n << " ";
    }
    cout << endl;
    cout << "--------------------------------" << endl;
}

void Graph::displayAdjList() const
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

void Graph::displayAdjListId() const
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

void Graph::displayColors() const
{
    updateColors();
    cout << "-------------colors--------------" << endl;
    for(Color color : colors_) {
        cout << color << " ";
    }
    cout << endl;
    cout << "---------------------------------" << endl;
}