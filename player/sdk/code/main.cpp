#include <iostream>
#include <vector>

#include "data.h"
#include "Solution.h"

using namespace std;

int main(int argc, char *argv[])
{
    uint32_t N;             // 表示所有卫星和发射基站的总数
    uint32_t E;             // 表示网络中可使用的路径总数
    uint32_t C;             // 表示路径距离L与功耗P的系数，以编号为i的发射基站为起点的传输功耗计算公式为：P=C*L*pwrVec[i]
    uint32_t D;             // 表示全网最大路径长度限制。
    uint32_t PS;            // 表示接收卫星所需要的站点功耗
    vector<bool> typeVec;   // 下标为i的值代表ID为i的站点身份，卫星为true，发射基站为false
    vector<int> pwrVec;     // 下标为i的值代表ID为i的发射基站的功耗系数，如果不是发射基站，则为10000
    vector<Edge> edgeVec;   // 包含E条边

    cin >> N >> E >> C >> D >> PS;

    // 输入站点身份
    typeVec = vector<bool>(N);
    for (uint32_t i = 0; i < N; i++) {
        bool type;
        cin >> type;
        typeVec[i] = type;
    }

    // 输入发射基站的额功耗系数
    pwrVec = vector<int>(N);
    for (uint32_t i = 0; i < N; i++) {
        cin >> pwrVec[i];
    }

    // 输入边集
    edgeVec = vector<Edge>(E);
    for (auto& edge : edgeVec) {
        cin >> edge.send >> edge.recv >> edge.dist;
    }

    Solution solution;
    vector<Route> retRouteVec = solution.Plan(N, C, D, PS, typeVec, pwrVec, edgeVec);
    for (const auto& route : retRouteVec) {
        for (const auto& siteId : route) {
            cout << siteId << " ";
        }
        cout << "\n";
    }
    return 0;
}