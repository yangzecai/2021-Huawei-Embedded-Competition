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
    using Route = vector<NodeIndex>;
    using AdjList = vector<vector<NodeIndex>>;
    using AdjMatrix = vector<vector<Dist>>;
    using Color = uint32_t;

    static const Dist kInf;

    Graph(vector<NodeId> nodes = vector<NodeId>(),
          vector<Edge> edges = vector<Edge>());
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

    size_t getMaxDegree() const;

    const AdjList& getAdjList() const;
    const AdjList& getAdjListSorted() const;
    const AdjMatrix& getAdjMatrix() const;
    const vector<Color>& getColors() const;

    Graph getGraphBar() const;
    Graph getScopeGraph(Dist limit) const;

    void displayNodesId() const;    // for debug
    void displayAdjList() const;    // for debug
    void displayAdjListId() const;  // for debug
    void displayColors() const;     // for debug

private:
    vector<NodeId> nodes_;
    vector<Edge> edges_;

    mutable bool isAdjListNew_;
    mutable AdjList adjList_;

    mutable bool isAdjMatrixNew_;
    mutable AdjMatrix adjMatrix_;

    mutable bool isColorsNew_;
    mutable vector<Color> colors_;

    void updateAdjList() const;
    void updateAdjMatrix() const;
    void updateColors() const;

    void sortAdjList();
};

