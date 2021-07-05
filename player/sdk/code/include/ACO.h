#pragma once

#include "data.h"
#include <assert.h>
#include <set>

using std::set;

class Ant {
public:
    Ant(const vector<double> &envPhers, double alpha, double beta);
    ~Ant() = default;

    Ant(const Ant &) = delete;
    Ant &operator=(const Ant &) = delete;

    void run();

    const set<SateGraph::NodeIndex> &getRecvSateSet() const
    {
        return recvSateSet_;
    }

private:
    const vector<double> &envPhers_;
    set<SateGraph::NodeIndex> recvSateSet_;
    set<SateGraph::NodeIndex> unusedSates_;
    set<SateGraph::NodeIndex> uncoverBases_;
    const uint8_t alpha_;
    const uint8_t beta_;

    struct Choice {
        SateGraph::NodeIndex sate;
        double probability;
        size_t newCoverNum;
        Power powerSum;
    };

    void selectRecvSate(SateGraph::NodeIndex base);
    SateGraph::NodeIndex getRandUncoverBase()
    {
        return getRandElemFromSet<SateGraph::NodeIndex>(uncoverBases_);
    }
    Choice getChoice(SateGraph::NodeIndex sate);
    void normProbs(vector<Choice> &choices);
    const Choice &roulette(const vector<Choice> &choices);
    void determineChoice(const Choice &choice);
    void producePher();
    void localSearch();
}; // class Ant

class ACO {
public:
    ACO(double alpha, double beta, double rho, double epsilon, uint16_t antNum);
    ~ACO() = default;

    void iterate(uint16_t iterNum);
    Power getMinPowerSum() const { return minPowerSum_; }
    const set<SateGraph::NodeIndex> &getMinRecvSateSet() const
    {
        return minRecvSateSet_;
    }

private:
    const double alpha_;
    const double beta_;
    const double rho_;
    const double epsilon_;
    const uint8_t antNum_;
    double maxPher_;
    double minPher_;
    vector<double> phers_;
    vector<double> deltaPhers_;
    Power minPowerSum_;
    set<SateGraph::NodeIndex> minRecvSateSet_;

    void updatePhers();
    void updateDeltaPhers();
}; // class ACO
