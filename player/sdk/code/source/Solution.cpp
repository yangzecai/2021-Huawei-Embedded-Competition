#include "Solution.h"
#include "DisJointSet.h"
#include "Dijkstra.h"

#include <unordered_map>

namespace my {

Solution::Solution(const uint32_t coeff, const Graph::Dist limit, 
                   const Power site, const vector<Node>& nodes, 
                   const vector<Edge>& edges)
    : graph_(nodes, edges)
    , coeff_(coeff)
    , limit_(limit)
    , site_(site)
    , lasts_(nodes.size(), nodes.size())
    , bases_()
    , sates_()
{
    for(Graph::NodeIndex i = 0; i < nodes.size(); ++i) {
        if(nodes[i].isSate) {
            sates_.push_back(i);
        } else {
            bases_.push_back(i);
        }
    }
}

Solution::Solution(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                   const vector<bool>& typeVec, 
                   const vector<::Edge>& edgeVec)
    : graph_()
    , coeff_(C)
    , limit_(D)
    , site_(PS)
    , lasts_(N, N)
    , bases_()
    , sates_()
{   
    for(Graph::NodeIndex i = 0; i < N; ++i) {
        graph_.addNode(Node{static_cast<Node::ID>(i), typeVec[i]});
    }
    
    for(::Edge edge : edgeVec) {
        graph_.addEdge(Edge{edge.send, edge.recv, edge.dist});
    }

    for(Graph::NodeIndex i = 0; i < N; ++i) {
        if(graph_.getNode(i).isSate) {
            sates_.push_back(i);
        } else {
            bases_.push_back(i);
        }
    }
}

void Solution::swap(Solution &&rhs)
{
    using std::swap;
    swap(this->graph_, rhs.graph_);
    swap(this->lasts_, rhs.lasts_);
    swap(this->bases_, rhs.bases_);
    swap(this->sates_, rhs.sates_);
}

Solution::Solution(Solution &&rhs)
    : graph_(std::move(rhs.graph_))
    , coeff_(rhs.coeff_)
    , limit_(rhs.limit_)
    , site_(rhs.site_)
    , lasts_(rhs.lasts_)
    , bases_(std::move(rhs.bases_))
    , sates_(std::move(rhs.sates_))
{
}

Solution& Solution::operator= (Solution &&rhs)
{
    swap(std::move(rhs));
    return *this;
}

void Solution::meetCondition()
{
    const Graph::AdjMatrix &adjMatrix = graph_.getAdjMatrix();
    
    for(Graph::NodeIndex node : bases_) {
        Graph::Dist sumDist = 0;
        while (lasts_[node] != node) {
            assert(lasts_[node] < graph_.getOrder());
            sumDist += adjMatrix[node][lasts_[node]];
            if(sumDist > limit_) {
                lasts_[node] = node;
                break;
            }
            node = lasts_[node];
        }
    }
}

vector<::Route> Solution::parseLasts()
{
    vector<::Route> retRouteVec;
    retRouteVec.reserve(bases_.size());

    for(Graph::NodeIndex node : bases_) {
        retRouteVec.push_back(::Route());
        retRouteVec.back().push_back(graph_.getNodeID(node));
        while(lasts_[node] != node) {
            node = lasts_[node];
            assert(node < graph_.getOrder());
            retRouteVec.back().push_back(graph_.getNodeID(node));
        }
    }

    return retRouteVec;
}

void Solution::test(void)
{
    vector<Graph::NodeIndex> lasts(graph_.getOrder());
    Dijkstra<Graph> shortPath(graph_, 5, lasts);
    vector<Graph::Dist> dists (std::move(shortPath(0)));
    cout << "-----------------dist-----------------" << endl;
    for(auto dist : dists) {
        cout << dist << endl;
    }
    cout << "--------------------------------------" << endl;

    cout << "-----------------last-----------------" << endl;
    for(auto last : lasts) {
        cout << last << endl;
    }
    cout << "--------------------------------------" << endl;
}

vector<::Route> Solution::PlanA()
{
    vector<::Route> retRouteVec;
    vector<Graph> graphs = graph_.getConnSubgraphs();
    
    for(const Graph &graph : graphs) {
        Solution subSolu(coeff_, limit_, site_, 
                         graph.getNodes(), graph.getEdges());
        
        Graph::NodeIndex centre = subSolu.sates_[0];

        Dijkstra<Graph> shortPath(subSolu.graph_, Graph::kInf, 
                                  subSolu.lasts_);                    
        shortPath(centre);

        subSolu.meetCondition();

        vector<::Route> partRouteVec = std::move(subSolu.parseLasts());
        retRouteVec.insert(retRouteVec.end(), partRouteVec.begin(), 
                           partRouteVec.end());
    }

    return retRouteVec;
}

}   // namespace my