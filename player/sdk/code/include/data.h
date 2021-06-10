#pragma once

#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    uint32_t send;  // 发送站点
    uint32_t recv;  // 接受站点
    uint32_t dist;  // 距离
};

using Route = vector<uint32_t>;