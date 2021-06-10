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
        NodeId id;   //节点ID
        bool   type; //节点类型
    };
    using NodeIndex = vector<Node>::size_type;
    struct Edge {
        NodeIndex send;
        NodeIndex recv;
        size_t    dist;
    };

    Graph();
    ~Graph();

    Graph(const Graph&) = delete;
    Graph& operator= (const Graph&) = delete;

    void swap(Graph&& rhs);

    Graph(Graph&& rhs);
    Graph& operator= (Graph&& rhs);

    void add_node(const Node &node);
    void add_node(Node&& node);
    const vector<Node>& get_nodes() const;

    void add_edge(const Edge &edge);
    void add_edge(Edge&& edge);
    const vector<Edge>& get_edges() const;

    static std::vector<Graph> getConnectedGraphVec(uint32_t N,
                                                   const vector<bool>& typeVec,
                                                   const vector<::Edge>& edgeVec);

    void show_nodes() const;        //for debug
    void show_adjTable() const;     //for debug
    // void drawAdjMatrix() const;   //for debug

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_;
};

