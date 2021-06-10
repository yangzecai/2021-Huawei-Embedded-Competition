#pragma once

#include "data.h"

#include <vector>
#include <memory>
#include <iostream>

class Graph {
public:
    using num_t = uint32_t;
    using id_t  = uint32_t;
    struct Node {
        id_t id;    // 节点id
        bool type;  // 节点类型
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
    
    void add_edge(const Edge &edge);
    void add_edge(Edge&& edge);

    static std::vector<Graph> getConnectedGraphVec(uint32_t N,
                                                   const vector<bool>& typeVec,
                                                   const vector<Edge>& edgeVec);

    void show_nodes() const;        //for debug
    void show_adjTable() const;     //for debug
    // void drawAdjMatrix() const;   //for debug

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_;
};

