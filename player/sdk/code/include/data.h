#pragma once

#include <iostream>
#include <vector>
using namespace std;

struct Node {
    uint32_t id;    // 节点id
    bool     type;  // 节点类型
};

struct Edge {
    uint32_t send;  // 发送站点
    uint32_t recv;  // 接受站点
    uint32_t dist;  // 距离
};

using Route = vector<uint32_t>;