#pragma once

#include "Graph.h"
#include "data.h"
// #include "AntAlgorithm.h"

#include <unordered_map>
#include <cmath>
#include <set>
#include <vector>

#include <assert.h>

namespace my {

using namespace std;

struct Node {
    using ID = uint32_t;
    ID id;
    bool isSate;
};  // struct Node

using Graph = ::Graph<Node>;
using Edge = Graph::Edge;
using Power = uint32_t;

class Solution {
friend class AntAlgorithm;
public:
    Solution(const uint32_t coeff, const Graph::Dist limit, 
             const Power site,
             const vector<Node> &nodes = vector<Node>(),
             const vector<Edge> &edges = vector<Edge>());
    Solution(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
             const vector<bool>& typeVec, 
             const vector<::Edge>& edgeVec);

    ~Solution() = default;

    Solution(const Solution &rhs) = delete;
    Solution& operator= (const Solution &rhs) = delete;

    void swap(Solution &&rhs);

    Solution(Solution &&rhs);
    Solution& operator= (Solution &&rhs);

    void meetCondition();
    vector<::Route> parseLasts();

    void test(void);

    void genPath(Graph::NodeIndex source, vector<Graph::NodeIndex> dests);
    void initBGraph();
    
    
    /********************PlanA*********************/
    vector<::Route> PlanA();
    /**********************************************/



    /********************PlanB*********************/
    vector<::Route> PlanB();
    /**********************************************/



    /********************PlanC*********************/
    vector<::Route> PlanC();
    /**********************************************/
private:
    Graph graph_;
    
    // bGraph : 当且仅当卫星和基站存在 长度小于最大路径的路 时 之间有连线
    Graph bGraph_;

    const uint32_t coeff_;
    const Graph::Dist limit_;
    const Power site_;
    
    vector<Graph::NodeIndex> lasts_;

    // for caching subset
    vector<Graph::NodeIndex> bases_;
    vector<Graph::NodeIndex> sates_;

};  // class Solution


/**************************************************/
class AntAlgorithm {
public:
    class Ant {
    public:
        Ant(Solution *solu, const vector<double> &envPhers,
            uint8_t alpha, uint8_t beta, uint8_t Q);
        ~Ant() = default;
        Ant(const Ant&) = delete;
        Ant& operator= (const Ant&) = delete;
        void run();
        void test();
        void selectRandSet();
        void selectBestSet();
        Graph::NodeIndex roulette();
        double calProb(Graph::NodeIndex);
        void normProbs();
        void genPher();
        double getPher() const
        { return pher_; }
        uint32_t getCount() const
        { return count_; }
        const set<Graph::NodeIndex>& getSets() const 
        { return retSets_; }
        void displaySets(); // for debug
    private:
        Solution *solu_;
        const vector<double> &envPhers_;
        set<Graph::NodeIndex> retSets_;
        set<Graph::NodeIndex> unusedSates_;
        set<Graph::NodeIndex> uncoverBases_;
        using PBP = pair<Graph::NodeIndex, double>;
        vector<PBP> probs_;
        const uint8_t alpha_;
        const uint8_t beta_;
        const uint8_t Q_;
        double pher_;
        uint32_t count_;
    };  //class Ant

    AntAlgorithm(Solution *solu, uint8_t alpha, uint8_t beta, 
                 float rho, uint8_t Q, uint16_t antNum);
    ~AntAlgorithm() = default;

    vector<Ant> createAnts(uint32_t num);
    void iterate(uint16_t iterNum);
    void setAntNum(uint16_t antNum)
    { antNum_ = antNum; }
    void updatePhers();
    void updateDeltaPhers(const Ant &ant);
    void displayCurValue(); // for debug
    set<Graph::NodeIndex> getBestSets();

private:
    Solution *solu_;
    vector<double> phers_;
    vector<double> deltaPhers_;
    uint8_t alpha_;
    uint8_t beta_;
    float rho_;
    uint8_t Q_;
    uint16_t antNum_;
};  // class AntAlgorithm



}  // namespace my







