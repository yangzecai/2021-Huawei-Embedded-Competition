#include <iostream>
#include <vector>

#include "data.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "Solution.h"

using namespace std;

class Solution {
public:
    vector<Route> Plan(uint32_t N, uint32_t C, uint32_t D, uint32_t PS,
                       const vector<bool>& typeVec, const vector<Edge>& edgeVec)
    {

        my::Solution solu(N, C, D, PS, typeVec, edgeVec);
        return solu.PlanB();

        // vector<Route> retRouteVec;
        // return retRouteVec;
    }
};

int main(int argc, char *argv[])
{
    uint32_t N;             // 表示所有卫星和发射基站的总数
    uint32_t E;             // 表示网络中可使用的路径总数
    uint32_t C;             // 表示路径距离L与功耗P的系数，传输功耗计算公式为：P=C*L
    uint32_t D;             // 表示全网最大路径长度限制。
    uint32_t PS;            // 表示接收卫星所需要的站点功耗
    vector<bool> typeVec;   // 下标为i的值代表ID为i的站点身份，卫星为true，发射基站为false
    vector<Edge> edgeVec;   // 包含E条边

    cin >> N >> E >> C >> D >> PS;
    typeVec = vector<bool>(N);
    for (uint32_t i = 0; i < N; i++) {
        bool type;
        cin >> type;
        typeVec[i] = type;
    }
    edgeVec = vector<Edge>(E);
    for (auto& edge : edgeVec) {
        cin >> edge.send >> edge.recv >> edge.dist;
    }

    Solution solution;
    vector<Route> retRouteVec = solution.Plan(N, C, D, PS, typeVec, edgeVec);
    for (const auto& route : retRouteVec) {
        for (const auto& siteId : route) {
            cout << siteId << " ";
        }
        cout << "\n";
    }
    return 0;
}