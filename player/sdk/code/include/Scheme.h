// #include "Graph.h"
// #include "Dijkstra.h"

// #include <vector>

// class Scheme {
// public:
//     struct SchemeGraph {
//         Graph graph;
//         vector<Graph::NodeIndex> stations;
//         vector<Graph::NodeIndex> satellites;
//         vector<Graph::Route>     routes;
//     };
//     using SGIndex = vector<SchemeGraph>::size_type;
//     using RouteIndex = vector<Graph::Route>::size_type;
//     using Power = uint32_t;

//     Scheme(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
//            const vector<bool>& typeVec, const vector<Edge>& edgeVec);
//     ~Scheme();

//     Scheme(const Scheme&) = delete;
//     Scheme& operator= (const Scheme&) = delete;

//     Graph& getGraph(SGIndex i)
//     { return schemeGraphs_[i].graph; }

//     vector<Graph::NodeIndex>& getStats(SGIndex i)
//     { return schemeGraphs_[i].stations; }

//     vector<Graph::NodeIndex>& getSates(SGIndex i)
//     { return schemeGraphs_[i].satellites; }

//     vector<Graph::Route>& getRoutes(SGIndex i)
//     { return schemeGraphs_[i].routes; }

//     void solve();

//     vector<::Route> parseRoutes();

// private:

//     vector<SchemeGraph> schemeGraphs_;
//     uint32_t coeff_;
//     Graph::Dist limit_;
//     Power site_;
    
//     void solveConnected(SGIndex graphIndex);
//     // void samllModify(Dijkstra &djks);
// };