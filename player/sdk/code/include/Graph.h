#pragma once

#include "data.h"

#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Graph {
public:
    using NodeId = uint32_t;
    using NodeIndex = vector<NodeId>::size_type;
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
                                                   const vector<::Edge>& edgeVec);

    void addNode(const NodeId &node);
    void addNode(NodeId&& node);
    const vector<NodeId>& getNodes() const 
    { return nodes_; }
    NodeId getNodeId(NodeIndex i) const 
    { return nodes_[i]; }

    void addEdge(const Edge &edge);
    void addEdge(Edge&& edge);
    const vector<Edge>& getEdges() const
    { return edges_; }

    size_t getOrder() const
    { return nodes_.size(); }

    const AdjList& getAdjList();
    const AdjMatrix& getAdjMatrix();

    void displayNodesId() const;  // for debug
    void displayAdjList();         // for debug
    void displayAdjListId();      // for debug
    // void drawAdjMatrix() const;   // for debug

private:
    vector<NodeId> nodes_;
    vector<Edge> edges_;

    bool isAdjListNew_;
    AdjList adjList_;

    bool isAdjMatrixNew_;
    AdjMatrix adjMatrix_;

    void updateAdjList();
    void updateAdjMatrix();
};

