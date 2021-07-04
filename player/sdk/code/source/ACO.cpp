#include "ACO.h"
#include <cmath>

Ant::Ant(const vector<double> &envPhers, double alpha, double beta)
    : envPhers_(envPhers)
    , recvSateSet_()
    , unusedSates_(sateSubset.begin(), sateSubset.end())
    , uncoverBases_(baseSubset.begin(), baseSubset.end())
    , alpha_(alpha)
    , beta_(beta)
    , pher_(0)
{
}

void Ant::run()
{
    assert(!uncoverBases_.empty());
    while (!uncoverBases_.empty()) {
        SateGraph::NodeIndex base = getRandUncoverBase();
        selectRandRecvSate(base);
    }
    localSearch();
    producePher();
}

void Ant::localSearch() { removeRedundantSate(recvSateSet_); }

SateGraph::NodeIndex Ant::getRandUncoverBase()
{
    size_t index = rand() % uncoverBases_.size();
    auto iter = uncoverBases_.begin();
    for (size_t i = 0; i < index; ++i) {
        ++iter;
    }
    return *iter;
}

void Ant::selectRandRecvSate(SateGraph::NodeIndex base) // FIXME
{
    vector<Choice> choices;
    for (SateGraph::NodeIndex sate : bGraph.getAdjList()[base]) {
        choices.push_back(getChoice(sate));
    }
    normProbs(choices);
    const Choice &choice = roulette(choices);
    determineChoice(choice);
}

void Ant::determineChoice(const Choice &choice)
{
    unusedSates_.erase(choice.sate);
    recvSateSet_.insert(choice.sate);
    for (SateGraph::NodeIndex base : bGraph.getAdjList()[choice.sate]) {
        if (uncoverBases_.find(base) != uncoverBases_.end()) {
            uncoverBases_.erase(base);
        }
    }
}

Ant::Choice Ant::getChoice(SateGraph::NodeIndex sate) // FIXME
{
    assert(unusedSates_.find(sate) != unusedSates_.end());

    SateGraph::Dist sumDist = 0;
    size_t newCoverNum = 0;
    const vector<SateGraph::NodeIndex> &adjBases = bGraph.getAdjList()[sate];
    for (SateGraph::NodeIndex base : adjBases) {
        if (uncoverBases_.find(base) != uncoverBases_.end()) {
            ++newCoverNum;
            sumDist += bGraph.getDist(sate, base);
        }
    }
    Power powerSum = kPowerPerDist * sumDist + kPowerPerSite;
    double probability = 0;
    if (newCoverNum != 0) {
        double t = envPhers_[sate];
        double n = (double)newCoverNum; // / kPowerPerSite / powerSum;
        probability = pow(t, alpha_) * pow(n, beta_); // 初始时 0 乘任意数均为 0
    }
    return Choice{sate, probability, newCoverNum, powerSum};
}

void Ant::normProbs(vector<Choice> &choices)
{
    double sum = 0;
    for (Choice &choice : choices) {
        sum += choice.probability;
    }
    for (Choice &choice : choices) {
        choice.probability /= sum;
    }
}

const Ant::Choice &Ant::roulette(const vector<Choice> &choices)
{
    double randNum = (double)rand() / RAND_MAX;

    double cur = 0;
    for (const Choice &choice : choices) {
        cur += choice.probability;
        if (cur >= randNum) {
            return choice;
        }
    }
    assert(0);
}

void Ant::producePher() // FIXME
{
    pher_ = (double)1 / recvSateSet_.size();
}

ACO::ACO(double alpha, double beta, double rho, double epsilon, uint16_t antNum)
    : alpha_(alpha)
    , beta_(beta)
    , rho_(rho)
    , epsilon_(epsilon)
    , antNum_(antNum)
    , maxPher_((double)1 / (1 - rho_) / sateSubset.size())
    , minPher_(epsilon_ * maxPher_)
    , phers_(bGraph.getOrder(), (maxPher_ + minPher_) / 2)
    , deltaPhers_(bGraph.getOrder(), 0)
    , minPowerSum_(SateGraph::kInf)
    , minRecvSateSet_(sateSubset.begin(), sateSubset.end())
{
}

void ACO::iterate(uint16_t iterNum)
{
    for (int i = 0; i < iterNum; ++i) {
        for (int j = 0; j < antNum_; ++j) {
            Ant ant(phers_, alpha_, beta_);
            ant.run();
            if (ant.getRecvSateSet().size() <= minRecvSateSet_.size()) {
                Power curPowerSum = getPowerSum(ant.getRecvSateSet());
                if (curPowerSum < minPowerSum_) {
                    minPowerSum_ = curPowerSum;
                    minRecvSateSet_ = ant.getRecvSateSet();
                    maxPher_ =
                        (double)1 / (1 - rho_) / (ant.getRecvSateSet().size());
                    minPher_ = epsilon_ * maxPher_;
                }
            }
        }
        updateDeltaPhers();
        updatePhers();
    }
}

void ACO::updateDeltaPhers()
{
    for (SateGraph::NodeIndex sate : minRecvSateSet_) {
        deltaPhers_[sate] = 1.0 / minRecvSateSet_.size();
    }
}

void ACO::updatePhers()
{
    for (SateGraph::NodeIndex sate : sateSubset) {
        phers_[sate] *= rho_;
        phers_[sate] += deltaPhers_[sate];
        deltaPhers_[sate] = 0;
        if (phers_[sate] > maxPher_) {
            phers_[sate] = maxPher_;
        } else if (phers_[sate] < minPher_) {
            phers_[sate] = minPher_;
        }
    }
}
