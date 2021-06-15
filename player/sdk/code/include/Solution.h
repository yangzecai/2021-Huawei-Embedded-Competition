#pragma once

#include "Graph.h"
#include "data.h"

#include "Solution.h"
#include "DisJointSet.h"
#include "Dijkstra.h"

#include <unordered_map>

namespace my {

using namespace std;

struct Node {
    using ID = uint32_t;
    ID id;
    bool isSate;
    Graph<Node>::NodeIndex last;
};  // struct Node

class Solution {
public:

    using Graph = ::Graph<Node>;
    using Edge = Graph::Edge;
    using Power = uint32_t;

    Solution(uint32_t C, uint32_t D, uint32_t PS,
             const vector<Node> &nodes = vector<Node>(),
             const vector<Edge> &edges = vector<Edge>());
    Solution(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
             const vector<bool>& typeVec, const vector<::Edge>& edgeVec);

    ~Solution() = default;

    //拷贝构造 拷贝赋值

    void swap(Solution &&rhs);

    Solution(Solution &&rhs);
    Solution& operator= (Solution &&rhs);

    static vector<Solution> createGraphs(uint32_t N, uint32_t C, 
                                        uint32_t D, uint32_t PS,
                                        const vector<bool>& typeVec, 
                                        const vector<::Edge>& edgeVec);

    void test(void);

private:
    Graph graph_;

    const uint32_t coeff_;
    const Graph::Dist limit_;
    const Power site_;
    
    // for caching subset
    vector<Graph::NodeIndex> bases_;
    vector<Graph::NodeIndex> sates_;

};  // class Solution

}  // namespace my




namespace my {

Solution::Solution(uint32_t C, uint32_t D, uint32_t PS,
                 const vector<Node>& nodes, const vector<Edge>& edges)
    : graph_(nodes, edges)
    , coeff_(C)
    , limit_(D)
    , site_(PS)
    , bases_()
    , sates_()
{
    for(Graph::NodeIndex i = 0; i < nodes.size(); ++i) {
        if(nodes[i].isSate) {
            sates_.push_back(i);
        } else {
            bases_.push_back(i);
        }
    }
}

Solution::Solution(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                   const vector<bool>& typeVec, const vector<::Edge>& edgeVec)
    : graph_()
    , coeff_(C)
    , limit_(D)
    , site_(PS)
    , bases_()
    , sates_()
{
    Solution solu(C, D, PS);
    
    for(Graph::NodeIndex i = 0; i < N; ++i) {
        solu.graph_.addNode(Node{static_cast<Node::ID>(i), typeVec[i], i});
    }
    
    for(::Edge edge : edgeVec) {
        solu.graph_.addEdge(Edge{edge.send, edge.dist, edge.dist});
    }

    for(Graph::NodeIndex i = 0; i < N; ++i) {
        if(graph_.getNode(i).isSate) {
            sates_.push_back(i);
        } else {
            bases_.push_back(i);
        }
    }
}


void Solution::swap(Solution &&rhs)
{
    using std::swap;
    swap(this->graph_, rhs.graph_);
    swap(this->bases_, rhs.bases_);
    swap(this->sates_, rhs.sates_);
}

Solution::Solution(Solution &&rhs)
    : graph_(std::move(rhs.graph_))
    , coeff_(rhs.coeff_)
    , limit_(rhs.limit_)
    , site_(rhs.site_)
    , bases_(std::move(rhs.bases_))
    , sates_(std::move(rhs.sates_))
{
}

Solution& Solution::operator= (Solution &&rhs)
{
    swap(std::move(rhs));
    return *this;
}

void Solution::test(void)
{
    Dijkstra<Graph> shortPath(graph_);
    vector<Graph::Dist> dists (std::move(shortPath(0)));
    cout << "-----------------dist-----------------" << endl;
    for(auto dist : dists) {
        cout << dist << endl;
    }
    cout << "--------------------------------------" << endl;
}

}   // namespace my


