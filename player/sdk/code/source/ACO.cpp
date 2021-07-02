#include "ACO.h"
#include <cmath>

Ant::Ant(const vector<double> &envPhers, uint8_t alpha, uint8_t beta, uint8_t Q)
    : envPhers_(envPhers)
    , retSets_()
    , unusedSates_(sateSubset.begin(), sateSubset.end())
    , uncoverBases_(baseSubset.begin(), baseSubset.end())
    , probs_()
    , alpha_(alpha)
    , beta_(beta)
    , Q_(Q)
    , pher_(0)
    , count_(0)
{
}

void Ant::run()
{
    assert(!uncoverBases_.empty());
    while (!uncoverBases_.empty()) {
        selectRandSet();
    }
    genPher();
}

void Ant::test()
{
    assert(!uncoverBases_.empty());
    while (!uncoverBases_.empty()) {
        selectBestSet();
    }
    genPher();
}

void Ant::selectRandSet() // FIXME
{
    assert(probs_.empty());

    size_t index = rand() % uncoverBases_.size();
    auto iter = uncoverBases_.begin();
    for (size_t i = 0; i < index; ++i) {
        ++iter;
    }
    SateGraph::NodeIndex base = *iter;

    for (SateGraph::NodeIndex sate : bGraph.getAdjList()[base]) {
        probs_.push_back(make_pair(sate, calProb(sate)));
    }
    normProbs();
    SateGraph::NodeIndex sate = roulette();
    unusedSates_.erase(sate);
    retSets_.insert(sate);
    for (SateGraph::NodeIndex base : bGraph.getAdjList()[sate]) {
        if (uncoverBases_.find(base) != uncoverBases_.end()) {
            uncoverBases_.erase(base);
        }
    }
    probs_.clear();
}

void Ant::selectBestSet() // FIXME
{
    assert(probs_.empty());
    SateGraph::NodeIndex bestSate;
    double bestProb = 0;
    for (SateGraph::NodeIndex sate : unusedSates_) {
        double curProb = calProb(sate);
        if (curProb > bestProb) {
            bestProb = curProb;
            bestSate = sate;
        }
    }
    unusedSates_.erase(bestSate);
    retSets_.insert(bestSate);
    for (SateGraph::NodeIndex base : bGraph.getAdjList()[bestSate]) {
        if (uncoverBases_.find(base) != uncoverBases_.end()) {
            uncoverBases_.erase(base);
        }
    }
}

double Ant::calProb(SateGraph::NodeIndex sate) // FIXME
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

    float p = 0; // FIXME
    if (coverNum != 0) {
        double t = envPhers_[sate];
        double n = (double)coverNum / (kPowerPerDist * sumDist + kPowerPerSite);
        p = pow(t, alpha_) + pow(n, beta_);
    }

    return p;
}

void Ant::normProbs()
{
    double sum = 0;
    for (PBP p : probs_) {
        sum += p.second;
    }
    for (PBP &p : probs_) {
        p.second /= sum;
    }
}

SateGraph::NodeIndex Ant::roulette()
{
    assert(!probs_.empty());

    double randNum = (double)rand() / RAND_MAX;

    double cur = 0;
    for (PBP prob : probs_) {
        cur += prob.second;
        if (cur >= randNum) {
            return prob.first;
        }
    }
    assert(0);
    return (SateGraph::NodeIndex)bGraph.getOrder();
}

void Ant::genPher() // FIXME
{
    uint32_t pher = 0;

    const SateGraph::AdjList &bAdjList = bGraph.getAdjList();
    for (SateGraph::NodeIndex base : baseSubset) {
        SateGraph::Dist bestDist = SateGraph::kInf;
        for (SateGraph::NodeIndex sate : bAdjList[base]) {
            if (retSets_.find(sate) != retSets_.end()) {
                SateGraph::Dist curDist = bGraph.getDist(base, sate);
                if (curDist < bestDist) {
                    bestDist = curDist;
                }
            }
        }
        pher += bestDist;
    }
    pher *= kPowerPerDist;
    pher += kPowerPerSite * retSets_.size();
    count_ = pher;
    pher_ = (double)Q_ / pher; // FIXME
}

void Ant::displaySets()
{
    cout << "-----------sets-----------" << endl;
    for (SateGraph::NodeIndex sate : getSets()) {
        cout << sate << endl;
    }
    cout << "--------------------------" << endl;
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
{
}

void ACO::iterate(uint16_t iterNum)
{
    for (int i = 0; i < iterNum; ++i) {
        for (int j = 0; j < antNum_; ++j) {
            Ant ant(phers_, alpha_, beta_, Q_);
            ant.run();
            updateDeltaPhers(ant);
        }
        updatePhers();
    }
}

void ACO::updateDeltaPhers(const Ant &ant)
{
    for (SateGraph::NodeIndex sate : ant.getSets()) {
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

void ACO::displayCurValue()
{
    Ant ant(phers_, 1, 0, Q_);
    ant.test();

    cout << ant.getCount() << endl;
}

set<SateGraph::NodeIndex> ACO::getBestSets()
{
    Ant ant(phers_, 1, 0, Q_);
    ant.test();
    set<SateGraph::NodeIndex> sets(ant.getSets());

    return sets;
}
