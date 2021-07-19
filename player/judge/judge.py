from res import ResMgr
from config import Config
from case import CaseMgr
from solution import Solution

class JudgeResult:
    def __init__(self, errCode = Config.ERR_CODE_SUCCESS, detailMsg = ""):
        self.errCode = errCode
        self.detailMsg = detailMsg
        return
    
    def GetPrintStr(self):
        str = Config.ERR_MSG_MGR[self.errCode]
        str += " "
        str += self.detailMsg
        return str

class Judge():
    def __init__(self, work, caseId):
        self.__work = work
        self.__caseName = Config.CASE_NAME_VEC[caseId - 1]
        self.__commonCase = CaseMgr.GetCommonCase(caseId)
        self.__cloneCase = CaseMgr.GetCloneCase(caseId)
        self.__pwr = 0
        return

    def Solve(self):
        status = ResMgr.RunProc(self.__cloneCase.caseId)
        if status == Config.ERR_CODE_RUN_TIMEOUT:
            self.__work.NotifyJudgeDone(self.__caseName, -1, 0, JudgeResult(Config.ERR_CODE_RUN_TIMEOUT))
            return
        if status == Config.ERR_CODE_RUN_FAIL:
            self.__work.NotifyJudgeDone(self.__caseName, -1, 0, JudgeResult(Config.ERR_CODE_RUN_FAIL))
            return
        self.__runtime = status
        print("[判题程序-开始] 用例：" + self.__caseName)
        self.__CheckSolution()
        print("[判题程序-结束] 用例：" + self.__caseName)
        return

    def __CheckRecvSite(self, recvSite):
        # 校验接收站点是否是一个未出现过的卫星
        cloneCase = self.__cloneCase
        commonCase = self.__commonCase
        if recvSite.siteId not in cloneCase.spaceSiteSet:
            if recvSite.siteId in commonCase.spaceSiteSet:
                return JudgeResult(Config.ERR_CODE_JUDGE_REPEAT_RECV, "站点=%d" % recvSite.siteId)
            else:
                return JudgeResult(Config.ERR_CODE_JUDGE_INVALID_RECV, "站点=%d" % recvSite.siteId)
        cloneCase.spaceSiteSet.remove(recvSite.siteId)

        # 累计接收站点的功耗
        self.__pwr = self.__pwr + cloneCase.GetRecvPwr(recvSite.siteId)

        for initRoute in recvSite.routeVec:
            routeStr = str(initRoute)
            route = initRoute[0:-1]
            route.reverse()
            # 校验（翻转前的）起始站点是否是一个未出现过的地面站点
            groundSiteId = route[-1]
            if groundSiteId not in cloneCase.groundSiteSet:
                if groundSiteId in commonCase.groundSiteSet:
                    return JudgeResult(Config.ERR_CODE_JUDGE_REPEAT_SEND, "站点=%d 路径=%s" % (groundSiteId, routeStr))
                else:
                    return JudgeResult(Config.ERR_CODE_JUDGE_INVALID_SEND, "站点=%d 路径=%s" % (groundSiteId, routeStr))
            cloneCase.groundSiteSet.remove(groundSiteId)

            routeDist = 0
            preSiteId = recvSite.siteId
            for curSiteId in route:
                if curSiteId in self.__solution.recvSiteMgr:
                    return JudgeResult(Config.ERR_CODE_JUDGE_RECV_ALSO_MID, "站点=%d 路径=%s" % (curSiteId, routeStr))

                # 校验站点编号是否在区间范围内
                if not cloneCase.IsSiteIdValid(curSiteId):
                    return JudgeResult(Config.ERR_CODE_JUDGE_INVALID_SITE, "站点=%d 路径=%s" % (curSiteId, routeStr))
                # 校验边是否存在，且没有被之前的接收站点使用
                edgeDist = cloneCase.GetDist(preSiteId, curSiteId)
                if edgeDist < 0:
                    if commonCase.GetDist(preSiteId, curSiteId) < 0:
                        return JudgeResult(Config.ERR_CODE_JUDGE_EDGE_NONE, "边=(%d->%d) 路径=%s" % (preSiteId, curSiteId, routeStr))
                    else:
                        return JudgeResult(Config.ERR_CODE_JUDGE_EDGE_CONFLICT, "边=(%d->%d) 路径=%s" % (preSiteId, curSiteId, routeStr))
                preSiteId = curSiteId
                # 校验路径总长度是否超出限制
                routeDist = routeDist + edgeDist
                if not cloneCase.IsRouteDistValid(routeDist):
                    return JudgeResult(Config.ERR_CODE_JUDGE_ROUTE_LENOUT, "路径=%s" % routeStr)
                # 累计发射的功耗
                self.__pwr = self.__pwr + edgeDist * cloneCase.castPwr * commonCase.GetSendPwr(groundSiteId)


        for route in recvSite.routeVec:
            preSiteId = recvSite.siteId
            for curSiteId in route:
                cloneCase.DelEdge(preSiteId, curSiteId)
                preSiteId = curSiteId
        return JudgeResult()

    def __CheckRecvSites(self):
        for recvSite in self.__solution.recvSiteMgr.values():
            judgeResult = self.__CheckRecvSite(recvSite)
            if judgeResult.errCode != Config.ERR_CODE_SUCCESS:
                return judgeResult
        if len(self.__cloneCase.groundSiteSet) > 0:
            return JudgeResult(Config.ERR_CODE_JUDGE_SEND_LEFT, "未规划站点=%s" % str(self.__cloneCase.groundSiteSet))
        return JudgeResult()

    def __CheckSolution(self):
        solution = Solution()
        print("[解析输出-开始] 用例：" + self.__caseName)
        errCode = solution.ReadConfig(self.__cloneCase.caseId)
        print("[解析输出-结束] 用例：" + self.__caseName)
        if errCode != Config.ERR_CODE_SUCCESS:
            self.__work.NotifyJudgeDone(self.__caseName, -1, -1, JudgeResult(errCode))
            return False

        self.__solution = solution
        judgeResult = self.__CheckRecvSites()
        if judgeResult.errCode != Config.ERR_CODE_SUCCESS:
            self.__work.NotifyJudgeDone(self.__caseName, -1, -1, judgeResult)
            return False
        self.__work.NotifyJudgeDone(self.__caseName, self.__pwr, self.__runtime, judgeResult)
        return True
