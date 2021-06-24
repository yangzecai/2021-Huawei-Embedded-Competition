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
    , bGraph_(graph_.getNodes(), vector<Graph::Edge>())
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

    initBGraph();
}

Solution::Solution(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                   const vector<bool>& typeVec, 
                   const vector<::Edge>& edgeVec)
    : graph_()
    , bGraph_()
    , coeff_(C)
    , limit_(D)
    , site_(PS)
    , lasts_(N, N)
    , bases_()
    , sates_()
{   
    for(Graph::NodeIndex i = 0; i < N; ++i) {
        graph_.addNode(Node{static_cast<Node::ID>(i), typeVec[i]});
        bGraph_.addNode(Node{static_cast<Node::ID>(i), typeVec[i]});
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
    initBGraph();
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


void Solution::initBGraph()
{

    vector<Graph::NodeIndex> reached;
    Dijkstra<Graph> d1(graph_, limit_, Dijkstra<Graph>::unused_, reached);
    for(auto sate : sates_) {
        vector<Graph::Dist> dists(d1(sate));
        for(Graph::NodeIndex node : reached) {
            if(!bGraph_.getNode(node).isSate) {
                bGraph_.addEdge(Graph::Edge{sate, node, dists[node]});
            }
        }
        reached.clear();
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
    set<Graph::NodeIndex> uncoverBases(bases_.begin(), bases_.end());
    const Graph::AdjList &bAdjList = bGraph_.getAdjList();
    set<Graph::NodeIndex> bestSateSet;
    while(!uncoverBases.empty()) {
        Graph::NodeIndex bestSate = bGraph_.getOrder();
        
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
                    curSumDist += bGraph_.getDist(sate, base);
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
        }
        bestSateSet.insert(bestSate);
        for(Graph::NodeIndex base : bestCover) {
            uncoverBases.erase(base);
        }
    }
    
    for(Graph::NodeIndex base : bases_) {
        Graph::Dist bestDist = Graph::kInf;
        Graph::NodeIndex bestSate = bGraph_.getOrder();
        for(Graph::NodeIndex sate : bAdjList[base]) {
            if(bestSateSet.find(sate) != bestSateSet.end()) {
                Graph::Dist curDist = bGraph_.getDist(base, sate);
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

vector<::Route> Solution::PlanC()
{

}

void Solution::test(void)
{
    AntAlgorithm test(this, 1, 7, 0.1, 2, 1);
    test.displayCurValue();
    for(int i = 0; i < 100; ++i) {
        test.iterate(1);
        test.displayCurValue();
    }
}

/*************************************************/
/*******************************************************/

AntAlgorithm::AntAlgorithm(Solution *solu, uint8_t alpha, uint8_t beta, 
                           float rho, uint8_t Q, uint16_t antNum = 100)
    : solu_(solu)
    , phers_(solu_->bGraph_.getOrder(), 0.0000001) // FIXME
    , deltaPhers_(solu_->bGraph_.getOrder(), 0)
    , alpha_(alpha)
    , beta_(beta)
    , rho_(rho)
    , Q_(Q)
    , antNum_(antNum)
{
}

void AntAlgorithm::iterate(uint16_t iterNum)
{
    for(int i = 0; i < iterNum; ++i) {
        for(int j = 0; j < antNum_; ++j) {
            Ant ant(solu_, phers_, alpha_, beta_, Q_);
            ant.run();
            updateDeltaPhers(ant);
        }
        updatePhers();
    }
}

void AntAlgorithm::updateDeltaPhers(const Ant &ant)
{
    for(Graph::NodeIndex sate : ant.getSets()) {
        deltaPhers_[sate] += ant.getPher();
    }
}

void AntAlgorithm::updatePhers()
{
    for(Graph::NodeIndex sate : solu_->sates_) {
        phers_[sate] *= rho_;
        phers_[sate] += deltaPhers_[sate];
    }
    deltaPhers_ = vector<double>(solu_->bGraph_.getOrder(), 0);
}

void AntAlgorithm::displayCurValue() 
{
    Ant ant(solu_, phers_, 1, 0, Q_);
    ant.test();

    cout << ant.getCount() << endl;
}

AntAlgorithm::Ant::Ant(Solution *solu, const vector<double> &envPhers,
                       uint8_t alpha, uint8_t beta, uint8_t Q)
    : solu_(solu)
    , envPhers_(envPhers)
    , retSets_()
    , unusedSates_(solu_->sates_.begin(), solu_->sates_.end())
    , uncoverBases_(solu_->bases_.begin(), solu_->bases_.end())
    , probs_()
    , alpha_(alpha)
    , beta_(beta)
    , Q_(Q)
    , pher_(0)
    , count_(0)
{
}

void AntAlgorithm::Ant::run()
{
    assert(!uncoverBases_.empty());
    while(!uncoverBases_.empty()) {
        selectRandSet();
    }
    genPher();
}

void AntAlgorithm::Ant::test()
{
    assert(!uncoverBases_.empty());
    while(!uncoverBases_.empty()) {
        selectBestSet();
    }
    genPher();
}

void AntAlgorithm::Ant::selectRandSet()
{
    assert(probs_.empty());
    
    for(Graph::NodeIndex sate : unusedSates_) {
        probs_.push_back(make_pair(sate, calProb(sate)));
    }
    normProbs();
    Graph::NodeIndex sate = roulette();
    unusedSates_.erase(sate);
    retSets_.insert(sate);
    for(Graph::NodeIndex base : solu_->bGraph_.getAdjList()[sate]) {
        if(uncoverBases_.find(base) != uncoverBases_.end()) {
            uncoverBases_.erase(base);
        }
    }
    probs_.clear();
}

void AntAlgorithm::Ant::selectBestSet()
{
    assert(probs_.empty());
    Graph::NodeIndex bestSate;
    double bestProb = 0;
    for(Graph::NodeIndex sate : unusedSates_) {
        double curProb = calProb(sate);
        if(curProb > bestProb) {
            bestProb = curProb;
            bestSate = sate;
        }
    }
    unusedSates_.erase(bestSate);
    retSets_.insert(bestSate);
    for(Graph::NodeIndex base : solu_->bGraph_.getAdjList()[bestSate]) {
        if(uncoverBases_.find(base) != uncoverBases_.end()) {
            uncoverBases_.erase(base);
        }
    }
    probs_.clear();
}

double AntAlgorithm::Ant::calProb(Graph::NodeIndex sate) 
{
    assert(unusedSates_.find(sate) != unusedSates_.end());

    Graph::Dist sumDist = 0;
    size_t coverNum = 0;
    const vector<Graph::NodeIndex>& adjBases = solu_->bGraph_.getAdjList()[sate];
    for(Graph::NodeIndex base : adjBases) {
        if(uncoverBases_.find(base) != uncoverBases_.end()) {
            ++coverNum;
            sumDist += solu_->bGraph_.getDist(sate, base);
        }
    }

    float p = 0;    // FIXME 
    if(coverNum != 0) {
        double t = envPhers_[sate];
        double n = (double) coverNum / (solu_->coeff_ * sumDist + solu_->site_);
        p = pow(t, alpha_) + pow(n, beta_);
    }

    return p;
}

void AntAlgorithm::Ant::normProbs()
{
    float sum = 0;
    for(PBP p : probs_) {
        sum += p.second;
    }
    for(PBP &p : probs_) {
        p.second /= sum;
    }
}

Graph::NodeIndex AntAlgorithm::Ant::roulette() 
{
    assert(!probs_.empty());

    float randNum = (double) rand() / RAND_MAX;

    float cur = 0;
    for(PBP prob : probs_) {
        cur += prob.second;
        if(cur >= randNum) {
            return prob.first;
        }
    }
    return (Graph::NodeIndex)solu_->bGraph_.getOrder();
}

void AntAlgorithm::Ant::genPher()
{
    uint32_t pher = 0;
    
    const Graph::AdjList &bAdjList = solu_->bGraph_.getAdjList();
    for(Graph::NodeIndex base : solu_->bases_) {
        Graph::Dist bestDist = Graph::kInf;
        for(Graph::NodeIndex sate : bAdjList[base]) {
            if(retSets_.find(sate) != retSets_.end()) {
                Graph::Dist curDist = solu_->bGraph_.getDist(base, sate);
                if(curDist < bestDist) {
                    bestDist = curDist;
                }
            }
        }
        pher += bestDist;
    }
    pher *= solu_->coeff_;
    pher += solu_->site_ * retSets_.size();
    count_ = pher;
    pher_ = (double) Q_ / pher;  // FIXME
}

void AntAlgorithm::Ant::displaySets() {
    cout << "-----------sets-----------" << endl;
    for(Graph::NodeIndex sate : getSets()) {
        cout << sate << endl;
    }
    cout << "--------------------------" << endl;
}

}   // namespace my