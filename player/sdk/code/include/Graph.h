#pragma once

#include "data.h"

#include <vector>
#include <memory>
#include <iostream>

class Graph {
public:
    using num_t = uint32_t;
    using id_t  = uint32_t;

    Graph() 
        : nodes_()
        , edges_() 
    {
    }

    ~Graph() 
    {
    }

    Graph(const Graph& graph) 
        : nodes_(graph.nodes_)
        , edges_(graph.edges_)
    {    
        std::cout << "copy con" << std::endl;
    }

    Graph& operator= (const Graph& rhs) {
        nodes_ = rhs.nodes_;
        edges_ = rhs.edges_;
        std::cout << "copy ass" << std::endl;
        return *this;
    }



    void add_node(const Node &node) 
    {
        nodes_.push_back(node);
    }

    void add_node(Node&& node) 
    {
        nodes_.push_back(std::move(node));
    }
    
    void add_edge(const Edge &edge) 
    {
        edges_.push_back(edge);
    }

    void add_edge(Edge&& edge) 
    {
        edges_.push_back(std::move(edge));
    }

    static std::vector<Graph> getConnectedGraphVec(uint32_t N,
                                                   const vector<bool>& typeVec,
                                                   const vector<Edge>& edgeVec);

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_;
};

