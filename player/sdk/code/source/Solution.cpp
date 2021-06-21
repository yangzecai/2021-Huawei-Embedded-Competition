#include "Solution.h"
#include "DisJointSet.h"
#include "Dijkstra.h"

#include <unordered_map>
#include <set>

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

void Solution::genPath(Graph::NodeIndex source, vector<Graph::NodeIndex> dests) 
{
    vector<Graph::NodeIndex> path(graph_.getOrder(), graph_.getOrder());
    Dijkstra<Graph> d(graph_, limit_, path);
    d(source);
    for(Graph::NodeIndex node : dests) {
        while(path[node] != node) {
            assert(path[node] < graph_.getOrder());
            lasts_[node] = path[node];
            node = lasts_[node];
        }
        lasts_[node] = node;
    }
}

vector<::Route> Solution::PlanA()
{
    vector<::Route> retRouteVec;
    vector<Graph> graphs = graph_.getConnSubgraphs();
    
    for(const Graph &graph : graphs) {
        Solution subSolu(coeff_, limit_, site_, 
                         graph.getNodes(), graph.getEdges());
        
        Graph::NodeIndex centre = 0;
        Graph::Dist minSum = Graph::kInf;

        Dijkstra<Graph> d1(subSolu.graph_);
        for(Graph::NodeIndex sate : subSolu.sates_) {
            vector<Graph::Dist> dists(d1(sate));
            Graph::Dist sum = 0;
            for_each(dists.begin(), dists.end(), [&sum](Graph::Dist dist)
                                                      { sum += dist; });
            if(minSum > sum) {
                minSum = sum;
                centre = sate;
            }
        }

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

vector<::Route> Solution::PlanB()
{
    // bGraph : 当且仅当卫星和基站存在 长度小于最大路径的路 时 之间有连线
    Graph bGraph(graph_.getNodes(), vector<Graph::Edge>());  

    // 构造 bGraph
    vector<Graph::NodeIndex> reached;
    Dijkstra<Graph> d1(graph_, limit_, Dijkstra<Graph>::unused_, reached);
    for(auto sate : sates_) {
        vector<Graph::Dist> dists(d1(sate));
        for(Graph::NodeIndex node : reached) {
            if(!bGraph.getNode(node).isSate) {
                bGraph.addEdge(Graph::Edge{sate, node, dists[node]});
            }
        }
        reached.clear();
    }

    set<Graph::NodeIndex> uncoverBases(bases_.begin(), bases_.end());
    const Graph::AdjList &bAdjList = bGraph.getAdjList();
    set<Graph::NodeIndex> bestSateSet;
    while(!uncoverBases.empty()) {
        Graph::NodeIndex bestSate = bGraph.getOrder();
        
        Graph::Dist bestSumDist = Graph::kInf;
        size_t bestCoverNum = 0;
        vector<Graph::NodeIndex> bestCover;
        uint32_t bestWeight = UINT32_MAX;
        for(Graph::NodeIndex sate : sates_) {
            Graph::Dist curSumDist = 0;
            size_t curCoverNum = 0;
            vector<Graph::NodeIndex> curCover;
            const vector<Graph::NodeIndex>& adjBases = bAdjList[sate];
            for(Graph::NodeIndex base : adjBases) {
                if(uncoverBases.find(base) != uncoverBases.end()) {
                    ++curCoverNum;
                    curSumDist += bGraph.getDist(sate, base);
                    curCover.push_back(base);
                }
            }

            if(curCoverNum != 0) {
                uint32_t curWeight = (coeff_ * curSumDist + site_) / curCoverNum;
                if(curWeight < bestWeight) {
                    bestWeight = curWeight;
                    bestSate = sate;
                    bestCoverNum = curCoverNum;
                    bestSumDist = curSumDist;
                    bestCover = std::move(curCover);
                }
            }
            // if((curCoverNum > bestCoverNum) ||  // 覆盖的基地最多
            //    (curCoverNum == bestCoverNum &&  // 覆盖的基地同样多
            //     curCoverNum != 0 &&    
            //     curSumDist < bestSumDist)) {    // 但总路径短

            //     bestSate = sate;
            //     bestCoverNum = curCoverNum;
            //     bestSumDist = curSumDist;
            //     bestCover = std::move(curCover);
            // }
        }
        bestSateSet.insert(bestSate);
        for(Graph::NodeIndex base : bestCover) {
            uncoverBases.erase(base);
        }
    }
    
    for(Graph::NodeIndex base : bases_) {
        Graph::Dist bestDist = Graph::kInf;
        Graph::NodeIndex bestSate = bGraph.getOrder();
        for(Graph::NodeIndex sate : bAdjList[base]) {
            if(bestSateSet.find(sate) != bestSateSet.end()) {
                Graph::Dist curDist = bGraph.getDist(base, sate);
                if(curDist < bestDist) {
                    bestDist = curDist;
                    bestSate = sate;
                }
            }
        }
        genPath(bestSate, vector<Graph::NodeIndex>{base});
    }

    meetCondition();

    return parseLasts();
}

}   // namespace my