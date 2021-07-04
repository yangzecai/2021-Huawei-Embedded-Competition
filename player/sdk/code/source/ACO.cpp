#include "ACO.h"
#include <cmath>

Ant::Ant(const vector<double> &envPhers, uint8_t alpha, uint8_t beta, uint8_t Q)
    : envPhers_(envPhers)
    , minRecvSateSet_()
    , unusedSates_(sateSubset.begin(), sateSubset.end())
    , uncoverBases_(baseSubset.begin(), baseSubset.end())
    , alpha_(alpha)
    , beta_(beta)
    , Q_(Q)
    , pher_(0)
    , powerSum_(0)
{
}

void Ant::run()
{
    assert(!uncoverBases_.empty());
    while (!uncoverBases_.empty()) {
        SateGraph::NodeIndex base = getRandUncoverBase();
        selectRandRecvSate(base);
    }
    producePher();
}

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
    minRecvSateSet_.push_back(choice.sate);
    for (SateGraph::NodeIndex base : bGraph.getAdjList()[choice.sate]) {
        if (uncoverBases_.find(base) != uncoverBases_.end()) {
            uncoverBases_.erase(base);
        }
    }
    powerSum_ += choice.powerSum;
}

Ant::Choice Ant::getChoice(SateGraph::NodeIndex sate) // FIXME
{
    assert(unusedSates_.find(sate) != unusedSates_.end());

    SateGraph::Dist sumDist = 0;
    size_t coverNum = 0;
    const vector<SateGraph::NodeIndex> &adjBases = bGraph.getAdjList()[sate];
    for (SateGraph::NodeIndex base : adjBases) {
        if (uncoverBases_.find(base) != uncoverBases_.end()) {
            ++coverNum;
            sumDist += bGraph.getDist(sate, base);
        }
    }
    Power powerSum = kPowerPerDist * sumDist + kPowerPerSite;
    double probability = 0;
    if (coverNum != 0) {
        double t = envPhers_[sate];
        double n = (double)coverNum / powerSum;
        probability = pow(t, alpha_) + pow(n, beta_);
    }
    return Choice{sate, probability, coverNum, powerSum};
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
    pher_ = (double) Q_ / powerSum_;
    // uint32_t pher = 0;
    // const SateGraph::AdjList &bAdjList = bGraph.getAdjList();
    // for (SateGraph::NodeIndex base : baseSubset) {
    //     SateGraph::Dist bestDist = SateGraph::kInf;
    //     for (SateGraph::NodeIndex sate : bAdjList[base]) {
    //         if (minRecvSateSet_.find(sate) != minRecvSateSet_.end()) {
    //             SateGraph::Dist curDist = bGraph.getDist(base, sate);
    //             if (curDist < bestDist) {
    //                 bestDist = curDist;
    //             }
    //         }
    //     }
    //     pher += bestDist;
    // }
    // pher *= kPowerPerDist;
    // pher += kPowerPerSite * minRecvSateSet_.size();
    // pher_ = (double)Q_ / pher; // FIXME
}

ACO::ACO(uint8_t alpha, uint8_t beta, float rho, uint8_t Q,
         uint16_t antNum = 100)
    : phers_(bGraph.getOrder(), 0) // FIXME
    , deltaPhers_(bGraph.getOrder(), 0)
    , alpha_(alpha)
    , beta_(beta)
    , rho_(rho)
    , Q_(Q)
    , antNum_(antNum)
    , minPowerSum_(SateGraph::kInf)
    , minRecvSateSet_()
{
}

void ACO::iterate(uint16_t iterNum)
{
    for (int i = 0; i < iterNum; ++i) {
        for (int j = 0; j < antNum_; ++j) {
            Ant ant(phers_, alpha_, beta_, Q_);
            ant.run();
            updateDeltaPhers(ant);
            if(ant.getPowerSum() < minPowerSum_) {
                minRecvSateSet_ = ant.getMinRecvSateSet();
                minPowerSum_ = ant.getPowerSum();
            }
        }
        updatePhers();
    }
}

void ACO::updateDeltaPhers(const Ant &ant)
{
    for (SateGraph::NodeIndex sate : ant.getMinRecvSateSet()) {
        deltaPhers_[sate] += ant.getPher();
    }
}

void ACO::updatePhers()
{
    for (SateGraph::NodeIndex sate : sateSubset) {
        phers_[sate] *= rho_;
        phers_[sate] += deltaPhers_[sate];
    }
    deltaPhers_ = vector<double>(bGraph.getOrder(), 0);
}
