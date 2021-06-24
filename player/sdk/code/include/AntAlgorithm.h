// #pragma once

// #include "Solution.h"

// #include <vector>
// #include <set>

// namespace my {

// using namespace std;

// struct Node;
// using Graph = ::Graph<Node>;
// using Edge = Graph::Edge;
// using Power = uint32_t;
// class Solution;

// class AntAlgorithm {
// public:
//     class Ant {
//     public:
//         Ant(Solution *solu, const vector<double> &envPhers,
//             uint8_t alpha, uint8_t beta, uint8_t Q);
//         ~Ant();
//         Ant(const Ant&) = delete;
//         Ant& operator= (const Ant&) = delete;
//         void run();
//         void selectSet();
//         Graph::NodeIndex roulette();
//         double calProb(Graph::NodeIndex);
//         void normProbs();
//         void genPher();
//         double getPher() const
//         { return pher_; }
//         const set<Graph::NodeIndex>& getSets() const 
//         { return retSets_; }
//     private:
//         Solution *solu_;
//         const vector<double> &envPhers_;
//         set<Graph::NodeIndex> retSets_;
//         set<Graph::NodeIndex> unusedSates_;
//         set<Graph::NodeIndex> uncoverBases_;
//         using PBP = pair<Graph::NodeIndex, double>;
//         vector<PBP> probs_;
//         const uint8_t alpha_;
//         const uint8_t beta_;
//         const uint8_t Q_;
//         double pher_;
//     };  //class Ant

//     AntAlgorithm(Solution *solu, uint8_t alpha, uint8_t beta, 
//                  float rho, uint8_t Q, uint16_t antNum);
//     ~AntAlgorithm();

//     vector<Ant> createAnts(uint32_t num);
//     void iterate(uint16_t iterNum);
//     void setAntNum(uint16_t antNum)
//     { antNum_ = antNum; }
//     void updatePhers();
//     void updateDeltaPhers(const Ant &ant);
//     void displayCurValue(); // for debug

// private:
//     Solution *solu_;
//     vector<double> phers_;
//     vector<double> deltaPhers_;
//     uint8_t alpha_;
//     uint8_t beta_;
//     float rho_;
//     uint8_t Q_;
//     uint16_t antNum_;
// };  // class AntAlgorithm

// }   // namespace my

