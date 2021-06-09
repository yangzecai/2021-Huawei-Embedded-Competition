#include "Graph.h"
#include "UnionFindSet.h"

#include <unordered_map>

std::vector<Graph> Graph::getConnectedGraphVec(uint32_t N,
                                               const vector<bool>& typeVec,
                                               const vector<Edge>& edgeVec)
{
    vector<Graph> retGraphVec;
    unordered_map<id_t, id_t> graphIdMap;
    UnionFindSet ufs(N);
    
    // 求连通节点集合
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        ufs.merge(iter->dist, iter->recv);
    }
    
    // 将节点添加到相应连通图中
    for(id_t i = 0; i < N; ++i) {
        if(graphIdMap.find(ufs.find(i)) == graphIdMap.end()) {
            graphIdMap[i] = retGraphVec.size();
            retGraphVec.push_back(Graph());
        }
        id_t graphId = graphIdMap[i];
        retGraphVec[graphId].add_node(Node{i, typeVec[i]});
    }

    // 将边添加到相应的连通图中
    for(auto iter = edgeVec.cbegin(); iter != edgeVec.cend(); ++iter) {
        id_t graphId = graphIdMap[iter->send];
        retGraphVec[graphId].add_edge(*iter);
    }

    return retGraphVec;
}