#pragma once

#include "data.h"
#include <assert.h>
#include <set>

using std::set;

class Ant {
public:
    Ant(const vector<double> &envPhers, uint8_t alpha, uint8_t beta, uint8_t Q);
    ~Ant() = default;

    Ant(const Ant &) = delete;
    Ant &operator=(const Ant &) = delete;

    void run();

    double getPher() const { return pher_; }
    uint32_t getPowerSum() const { return powerSum_; }
    const vector<SateGraph::NodeIndex> &getMinRecvSateSet() const
    {
        return minRecvSateSet_;
    }

private:
    const vector<double> &envPhers_;
    vector<SateGraph::NodeIndex> minRecvSateSet_;
    set<SateGraph::NodeIndex> unusedSates_;
    set<SateGraph::NodeIndex> uncoverBases_;
    const uint8_t alpha_;
    const uint8_t beta_;
    const uint8_t Q_;
    double pher_;
    Power powerSum_;

    struct Choice {
        SateGraph::NodeIndex sate;
        double probability;
        size_t coverNum;
        Power powerSum;
    };

    void selectRandRecvSate(SateGraph::NodeIndex base);
    SateGraph::NodeIndex getRandUncoverBase();
    Choice getChoice(SateGraph::NodeIndex sate);
    void normProbs(vector<Choice> &choices);
    const Choice &roulette(const vector<Choice> &choices);
    void determineChoice(const Choice &choice);
    void producePher();
}; // class Ant

class ACO {
public:
    ACO(uint8_t alpha, uint8_t beta, float rho, uint8_t Q, uint16_t antNum);
    ~ACO() = default;

    void iterate(uint16_t iterNum);
    Power getMinPowerSum() const { return minPowerSum_; }
    const vector<SateGraph::NodeIndex> &getMinRecvSateSet() const
    {
        return minRecvSateSet_;
    }

private:
    vector<double> phers_;
    vector<double> deltaPhers_;
    uint8_t alpha_;
    uint8_t beta_;
    float rho_;
    uint8_t Q_;
    uint16_t antNum_;
    Power minPowerSum_;
    vector<SateGraph::NodeIndex> minRecvSateSet_;

    void setAntNum(uint16_t antNum) { antNum_ = antNum; }
    void updatePhers();
    void updateDeltaPhers(const Ant &ant);
}; // class ACO
