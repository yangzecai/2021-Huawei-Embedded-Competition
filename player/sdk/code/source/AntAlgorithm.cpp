// #include "AntAlgorithm.h"

// #include <cmath>
// #include <cstdlib>

// #include <assert.h>


// namespace my {

// AntAlgorithm::AntAlgorithm(Solution *solu, uint8_t alpha, uint8_t beta, 
//                            float rho, uint8_t Q, uint16_t antNum = 100)
//     : solu_(solu)
//     , phers_(solu_->bGraph_.getOrder(), 1) // FIXME
//     , deltaPhers_(solu_->bGraph_.getOrder(), 0)
//     , alpha_(alpha)
//     , beta_(beta)
//     , rho_(rho)
//     , Q_(Q)
//     , antNum_(antNum)
// {
// }

// void AntAlgorithm::iterate(uint16_t iterNum)
// {
//     for(int i = 0; i < iterNum; ++i) {
//         for(int j = 0; j < antNum_; ++j) {
//             Ant ant(solu_, phers_, alpha_, beta_, Q_);
//             ant.run();
//             updateDeltaPhers(ant);
//         }
//         updatePhers();
//     }
// }

// void AntAlgorithm::updateDeltaPhers(const Ant &ant)
// {
//     for(Graph::NodeIndex sate : ant.getSets()) {
//         deltaPhers_[sate] += ant.getPher();
//     }
// }

// void AntAlgorithm::updatePhers()
// {
//     for(Graph::NodeIndex sate : solu_->sates_) {
//         phers_[sate] *= rho_;
//         phers_[sate] += deltaPhers_[sate];
//     }
//     deltaPhers_ = vector<double>(solu_->bGraph_.getOrder(), 0);
// }

// void AntAlgorithm::displayCurValue() 
// {
//     Ant ant(solu_, phers_, 1, 0, Q_);
//     ant.run();
//     cout << ant.getPher() << " ";
// }

// AntAlgorithm::Ant::Ant(Solution *solu, const vector<double> &envPhers,
//                        uint8_t alpha, uint8_t beta, uint8_t Q)
//     : solu_(solu)
//     , envPhers_(envPhers)
//     , retSets_()
//     , unusedSates_(solu_->sates_.begin(), solu_->sates_.end())
//     , uncoverBases_(solu_->bases_.begin(), solu_->bases_.end())
//     , probs_()
//     , alpha_(alpha)
//     , beta_(beta)
//     , Q_(Q)
//     , pher_(0)
// {
// }

// void AntAlgorithm::Ant::run()
// {
//     while(!uncoverBases_.empty()) {
//         selectSet();
//     }
//     genPher();
// }

// void AntAlgorithm::Ant::selectSet()
// {
//     assert(probs_.empty());
    
//     for(Graph::NodeIndex sate : unusedSates_) {
//         probs_.push_back(make_pair(sate, calProb(sate)));
//     }
//     normProbs();
//     Graph::NodeIndex sate = roulette();
//     unusedSates_.erase(sate);
//     retSets_.insert(sate);
//     for(Graph::NodeIndex base : solu_->bGraph_.getAdjList()[sate]) {
//         if(uncoverBases_.find(base) != uncoverBases_.end()) {
//             uncoverBases_.erase(base);
//         }
//     }
//     probs_.clear();
// }

// double AntAlgorithm::Ant::calProb(Graph::NodeIndex sate) 
// {
//     assert(unusedSates_.find(sate) != unusedSates_.end());

//     Graph::Dist sumDist = 0;
//     size_t coverNum = 0;
//     vector<Graph::NodeIndex> coverBases;
//     const vector<Graph::NodeIndex>& adjBases = solu_->bGraph_.getAdjList()[sate];
//     for(Graph::NodeIndex base : adjBases) {
//         if(uncoverBases_.find(base) != uncoverBases_.end()) {
//             ++coverNum;
//             sumDist += solu_->bGraph_.getDist(sate, base);
//             coverBases.push_back(base);
//         }
//     }

//     float p = 0;    // FIXME 
//     if(coverNum != 0) {
//         double t = envPhers_[sate];
//         double n = (double) coverNum / (solu_->coeff_ * sumDist + solu_->site_);
//         p = pow(t, alpha_) + pow(n, beta_);
//     }

//     return p;
// }

// void AntAlgorithm::Ant::normProbs()
// {
//     float sum = 0;
//     for(PBP p : probs_) {
//         sum += p.second;
//     }
//     for(PBP &p : probs_) {
//         p.second /= sum;
//     }
// }

// Graph::NodeIndex AntAlgorithm::Ant::roulette() 
// {
//     assert(!probs_.empty());

//     float randNum = (double) rand() / RAND_MAX;

//     float cur = 0;
//     for(PBP prob : probs_) {
//         cur += prob.second;
//         if(cur >= randNum) {
//             return prob.first;
//         }
//     }
//     return (Graph::NodeIndex)solu_->bGraph_.getOrder();
// }

// void AntAlgorithm::Ant::genPher()
// {
//     uint32_t pher = 0;
    
//     const Graph::AdjList &bAdjList = solu_->bGraph_.getAdjList();
//     for(Graph::NodeIndex base : solu_->bases_) {
//         Graph::Dist bestDist = Graph::kInf;
//         for(Graph::NodeIndex sate : bAdjList[base]) {
//             if(retSets_.find(sate) != retSets_.end()) {
//                 Graph::Dist curDist = solu_->bGraph_.getDist(base, sate);
//                 if(curDist < bestDist) {
//                     bestDist = curDist;
//                 }
//             }
//         }
//         pher += bestDist;
//     }
//     pher *= solu_->coeff_;
//     pher += solu_->site_ * retSets_.size();
//     pher_ = (double) Q_ / pher;  // FIXME
// }

// } // namespace my