#include "Scheme_II.h"

SchemeII::SchemeII(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                     const vector<bool>& typeVec, const vector<Edge>& edgeVec)
    
{
    
}

SchemeII::~SchemeII()
{
}


SchemeII::MyGraph::MyGraph(uint32_t C, uint32_t D, uint32_t PS,
                           const vector<bool>& typeVec,
                           const vector<NodeId>& nodes, 
                           const vector<Edge>& edges)
    : Graph(nodes, edges)
    , coeff_(C)
    , limit_(D)
    , site_(PS)
    , types_(typeVec)
{
    
}