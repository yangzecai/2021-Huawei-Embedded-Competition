#include "Graph.h"
#include "Dijkstra.h"

#include <vector>

class Scheme {
public:
    using GraphIndex = vector<Graph>::size_type;
    using RouteIndex = vector<Graph::Route>::size_type;
    using Power = uint32_t;

    Scheme(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
           const vector<bool>& typeVec, const vector<Edge>& edgeVec);
    ~Scheme();

    Scheme(const Scheme&) = delete;
    Scheme& operator= (const Scheme&) = delete;

    void solve();

    vector<::Route> getRouteVec();

private:
    vector<Graph> graphs_;
    vector<vector<Graph::NodeIndex>> stations_;
    vector<vector<Graph::NodeIndex>> satellites_;
    uint32_t coeff_;
    Graph::Dist limit_;
    Power site_;
    vector<vector<Graph::Route>> routes_;
    
    void solveConnected(GraphIndex graphIndex);
};