import copy
from path import PathMgr
from res import ResMgr
from reader import IntReader
from config import Config

class Case:
    def GetNodePair(send, recv):
        if send < recv:
            return send * Config.NODE_MAX_ID + recv
        else:
            return recv * Config.NODE_MAX_ID + send

    def __init__(self, caseId):
        self.caseId = caseId
        self.ReadConfig(caseId)
        return

    def AddEdge(self, send, recv, dist):
        self.__edgeMgr[Case.GetNodePair(send, recv)] = dist
        return
    def DelEdge(self, send, recv):
        self.__edgeMgr.pop(Case.GetNodePair(send, recv), -1)
        return
    def GetDist(self, send, recv):
        return self.__edgeMgr.get(Case.GetNodePair(send, recv), -1)

    def IsSiteIdValid(self, siteId):
        return (siteId >= 0) and (siteId < self.__siteNum)
    
    def IsRouteDistValid(self, routeDist):
        return (routeDist >=0) and (routeDist <= self.__maxDist)

    def GetSendPwr(self, sendSiteId):
        return self.__sendPwrMgr[sendSiteId]

    def GetRecvPwr(self, recvSiteId):
        return self.__recvPwr

    def ReadConfig(self, caseId):
        intReader = IntReader(PathMgr.GetCasePath(caseId))
        # 固定数据
        self.__siteNum = intReader.Read()
        self.__edgeNum = intReader.Read()
        self.castPwr = intReader.Read()
        self.__maxDist = intReader.Read()
        self.__recvPwr = intReader.Read()
        self.__sendPwrMgr = []

        # 动态数据
        self.__edgeMgr = {}
        self.groundSiteSet = set()
        self.spaceSiteSet = set()

        # 读入站点身份
        for i in range(self.__siteNum):
            siteType = intReader.Read()
            if siteType == 0:
                self.groundSiteSet.add(i)
            elif siteType == 1:
                self.spaceSiteSet.add(i)

        # 读入发射基站的功耗系数
        for i in range(self.__siteNum):
            sitePwr = intReader.Read()
            self.__sendPwrMgr.append(sitePwr)

        # 读入边集
        for i in range(self.__edgeNum):
            send = intReader.Read()
            recv = intReader.Read()
            dist = intReader.Read()
            self.AddEdge(send, recv, dist)
        return

class CaseMgr:
    def Init():
        CaseMgr.__caseVec = []
        CaseMgr.LoadCases()
        return

    def LoadCases():
        for caseId in range(Config.CASE_NUM):
            CaseMgr.__caseVec.append(Case(caseId + 1))
        return

    def GetCommonCase(caseId):
        return CaseMgr.__caseVec[caseId - 1]
    def GetCloneCase(caseId):
        case = copy.deepcopy(CaseMgr.__caseVec[caseId - 1])
        return case