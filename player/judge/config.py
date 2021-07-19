import os

def GetParentPath(path, layer):
    for i in range(layer):
        path = os.path.dirname(path)
    return path

class Config:
    PATH_ROOT = GetParentPath(os.getcwd(), 1) + "/"

    TIME_MAX_RUN = 300
    TIME_MAX_BUILD = 60

    NODE_MAX_ID = 10000

    PWR_MAXN                        = 2999999999
    TIME_MAXN                       = 30
    TIME_UNIT                       = TIME_MAX_RUN / TIME_MAXN
    SCORE_MAXN                      = PWR_MAXN * TIME_MAXN + TIME_MAXN - 1
    def GetFinalScore(sumPwr, avgTime):
        if sumPwr > Config.PWR_MAXN:
            sumPwr = Config.PWR_MAXN
        avgTime = avgTime / Config.TIME_UNIT
        if avgTime >= Config.TIME_MAXN:
            avgTime = Config.TIME_MAXN - 1
        return sumPwr * Config.TIME_MAXN + avgTime

    ERR_CODE_SUCCESS                = 90000000000
    ERR_CODE_BUILD_FAIL             = 90000000101
    ERR_CODE_BUILD_TIMEOUT          = 90000000102
    ERR_CODE_RUN_FAIL               = 90000000201
    ERR_CODE_RUN_MEMOUT             = 90000000202
    ERR_CODE_RUN_TIMEOUT            = 90000000203
    ERR_CODE_OUTPUT_EMPTY           = 90000000301
    ERR_CODE_OUTPUT_INVALID         = 90000000302
    ERR_CODE_OUTPUT_FEW             = 90000000303
    ERR_CODE_JUDGE_REPEAT_SEND      = 90000000401
    ERR_CODE_JUDGE_REPEAT_RECV      = 90000000402
    ERR_CODE_JUDGE_INVALID_SITE     = 90000000403
    ERR_CODE_JUDGE_INVALID_SEND     = 90000000404
    ERR_CODE_JUDGE_INVALID_RECV     = 90000000405
    ERR_CODE_JUDGE_RECV_ALSO_MID    = 90000000406
    ERR_CODE_JUDGE_EDGE_NONE        = 90000000407
    ERR_CODE_JUDGE_EDGE_CONFLICT    = 90000000408
    ERR_CODE_JUDGE_ROUTE_LENOUT     = 90000000409
    ERR_CODE_JUDGE_SEND_LEFT        = 90000000410

    ERR_MSG_MGR = {
        ERR_CODE_BUILD_FAIL             : "编译错误",
        ERR_CODE_BUILD_TIMEOUT          : "编译超时",
        ERR_CODE_RUN_FAIL               : "运行错误",
        ERR_CODE_RUN_MEMOUT             : "运行错误-内存溢出",
        ERR_CODE_RUN_TIMEOUT            : "运行超时",
        ERR_CODE_OUTPUT_EMPTY           : "输出错误-无输出",
        ERR_CODE_OUTPUT_INVALID         : "输出错误-非法符号",
        ERR_CODE_OUTPUT_FEW             : "输出错误-提前结束",
        ERR_CODE_JUDGE_REPEAT_SEND      : "方案错误-重复规划了同一个发射基站",
        ERR_CODE_JUDGE_REPEAT_RECV      : "方案错误-重复规划了同一个接收卫星",
        ERR_CODE_JUDGE_INVALID_SITE     : "方案错误-规划了非法的站点",
        ERR_CODE_JUDGE_INVALID_SEND     : "方案错误-规划了非法的发射基站",
        ERR_CODE_JUDGE_INVALID_RECV     : "方案错误-规划了非法的接收卫星",
        ERR_CODE_JUDGE_RECV_ALSO_MID    : "方案错误-接收卫星同时规划成了中转卫星",
        ERR_CODE_JUDGE_EDGE_NONE        : "方案错误-规划了不存在的通信通道",
        ERR_CODE_JUDGE_EDGE_CONFLICT    : "方案错误-规划了冲突的通信通道",
        ERR_CODE_JUDGE_ROUTE_LENOUT     : "方案错误-规划了过长的通信路径",
        ERR_CODE_JUDGE_SEND_LEFT        : "方案错误-未规划所有的发射基站",
    }

    CASE_NAME_VEC = [
        "Example",
        "TestData_24",
    ]
    CASE_NUM = len(CASE_NAME_VEC)