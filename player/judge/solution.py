from path import PathMgr
from config import Config
from res import ResMgr
from reader import IntReader
from case import CaseMgr

class RecvSite:
    def __init__(self, siteId):
        self.siteId = siteId
        self.routeVec = []
        return

    def AddRoute(self, route):
        self.routeVec.append(route)
        return

    def ReadRoute(self, intReader):
        route = []
        while True:
            siteId = intReader.Read()
            if siteId < 0:
                return -1
            if siteId == self.siteId:
                route.reverse()
                self.routeVec.append(route)
                return len(route)
            route.append(siteId)

class Solution:
    def ReadConfig(self, caseId):
        intReader = IntReader(PathMgr.GetSoluPath(caseId))
        if intReader.errCode != Config.ERR_CODE_SUCCESS:
            return intReader.errCode

        self.recvSiteMgr = {}
        for row in intReader.valueMgr :
            if len(row) < 2 :
                continue
            recvSiteId = row[-1]
            if recvSiteId not in self.recvSiteMgr :
                self.recvSiteMgr[recvSiteId] = RecvSite(recvSiteId)
            self.recvSiteMgr[recvSiteId].AddRoute(row)
        return Config.ERR_CODE_SUCCESS

