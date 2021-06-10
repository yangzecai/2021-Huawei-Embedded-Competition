#pragma once

#include "data.h"

#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Graph {
public:
    using NodeId = uint32_t;
    struct Node {
        NodeId id;   // 节点ID
        bool   type; // 节点类型
    };
    using NodeIndex = vector<Node>::size_type;
    using Dist = uint32_t;
    struct Edge {
        NodeIndex send;
        NodeIndex recv;
        Dist      dist;
    };
    using AdjList = vector<vector<NodeIndex>>;
    using AdjMatrix = vector<vector<Dist>>;

    static const Dist Inf;

    Graph();
    ~Graph();

    Graph(const Graph&) = delete;
    Graph& operator= (const Graph&) = delete;

    void swap(Graph&& rhs);

    Graph(Graph&& rhs);
    Graph& operator= (Graph&& rhs);

    static std::vector<Graph> getConnectedGraphVec(uint32_t N,
                                                   const vector<bool>& typeVec,
                                                   const vector<::Edge>& edgeVec);

    void add_node(const Node &node);
    void add_node(Node&& node);
    const vector<Node>& get_nodes() const 
    { return nodes_; }
    NodeId get_node_id(NodeIndex i) const 
    { return nodes_[i].id; }

    void add_edge(const Edge &edge);
    void add_edge(Edge&& edge);
    const vector<Edge>& get_edges() const
    { return edges_; }

    size_t get_order() const
    { return nodes_.size(); }

    const AdjList& get_adjlist();
    const AdjMatrix& get_adjmatrix();

    void display_ndoes_id() const;  // for debug
    void display_adjlist();         // for debug
    void display_adjlist_id();      // for debug
    // void drawAdjMatrix() const;   // for debug

private:
    vector<Node> nodes_;
    vector<Edge> edges_;

    bool is_adjlist_new_;
    AdjList adjlist_;

    bool is_adjmatrix_new_;
    AdjMatrix adjmatrix_;

    void update_adjlist();
    void update_adjmatrix();
};

