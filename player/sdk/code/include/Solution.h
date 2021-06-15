#pragma once

#include "Graph.h"
#include "data.h"

#include <unordered_map>

namespace my {

using namespace std;

struct Node {
    using ID = uint32_t;
    ID id;
    bool isSate;
};  // struct Node

class Solution {
public:

    using Graph = ::Graph<Node>;
    using Edge = Graph::Edge;
    using Power = uint32_t;

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

    /***********************************PlanA*************************************/
    vector<::Route> PlanA();


private:
    Graph graph_;

    const uint32_t coeff_;
    const Graph::Dist limit_;
    const Power site_;
    
    vector<Graph::NodeIndex> lasts_;

    // for caching subset
    vector<Graph::NodeIndex> bases_;
    vector<Graph::NodeIndex> sates_;

};  // class Solution

}  // namespace my







