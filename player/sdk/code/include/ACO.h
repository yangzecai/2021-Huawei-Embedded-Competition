#pragma once

#include "data.h"
#include <set>
#include <assert.h>

using std::set;

class Ant {
public:
    Ant(const vector<double> &envPhers, uint8_t alpha, uint8_t beta, uint8_t Q);
    ~Ant() = default;

    Ant(const Ant &) = delete;
    Ant &operator=(const Ant &) = delete;

    void run();
    void test();
    void selectRandSet();
    void selectBestSet();
    SateGraph::NodeIndex roulette();
    double calProb(SateGraph::NodeIndex);
    void normProbs();
    void genPher();
    double getPher() const { return pher_; }
    uint32_t getCount() const { return count_; }
    const set<SateGraph::NodeIndex> &getSets() const { return retSets_; }
    void displaySets(); // for debug

private:
    const vector<double> &envPhers_;
    set<SateGraph::NodeIndex> retSets_;
    set<SateGraph::NodeIndex> unusedSates_;
    set<SateGraph::NodeIndex> uncoverBases_;
    using PBP = pair<SateGraph::NodeIndex, double>;
    vector<PBP> probs_;
    const uint8_t alpha_;
    const uint8_t beta_;
    const uint8_t Q_;
    double pher_;
    uint32_t count_;
}; // class Ant

class ACO {
public:
    ACO(uint8_t alpha, uint8_t beta, float rho, uint8_t Q, uint16_t antNum);
    ~ACO() = default;

    vector<Ant> createAnts(uint32_t num);
    void iterate(uint16_t iterNum);
    void setAntNum(uint16_t antNum) { antNum_ = antNum; }
    void updatePhers();
    void updateDeltaPhers(const Ant &ant);
    void displayCurValue(); // for debug
    set<SateGraph::NodeIndex> getBestSets();

private:
    vector<double> phers_;
    vector<double> deltaPhers_;
    uint8_t alpha_;
    uint8_t beta_;
    float rho_;
    uint8_t Q_;
    uint16_t antNum_;
}; // class ACO
