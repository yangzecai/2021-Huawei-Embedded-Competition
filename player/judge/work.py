from config import Config
from res import ResMgr
from judge import Judge

class Work:
    def __init__(self):
        self.__judgeCnt = Config.CASE_NUM
        self.__score = 0
        self.__runtime = 0
        buildTime = ResMgr.GenProc()
        if buildTime == Config.ERR_CODE_BUILD_TIMEOUT:
            print("-----------------------------------------")
            print("[编译-超时] 错误编码：%d" % Config.ERR_CODE_BUILD_TIMEOUT)
            return
        if buildTime == Config.ERR_CODE_BUILD_FAIL:
            print("-----------------------------------------")
            print("[编译-错误] 错误编码：%d" % Config.ERR_CODE_BUILD_FAIL)
            return
        for caseId in range(Config.CASE_NUM):
            print("-----------------------------------------")
            judge = Judge(self, caseId + 1)
            judge.Solve()
            if self.__errCode != Config.ERR_CODE_SUCCESS:    # 有一组失败，则直接结束
                print("-----------------------------------------")
                print("[判题-失败] 错误编码：%d" % self.__errCode)
                return
        print("-----------------------------------------")
        print("[判题-成功] 最终分数：%d" % self.__finalScore)
        return

    def NotifyJudgeDone(self, caseName, score, runtime, judgeResult):
        self.__judgeCnt = self.__judgeCnt - 1

        if judgeResult.errCode == Config.ERR_CODE_SUCCESS:
            print("[判题程序-结果] 用例：%s  功耗：%d  时间：%d秒" % (caseName, score, runtime))
        else:
            print("[判题程序-结果] 用例：%s  错误信息：%s" % (caseName, judgeResult.GetPrintStr()))
        
        self.__errCode = judgeResult.errCode
        if self.__errCode != Config.ERR_CODE_SUCCESS:
            return

        self.__score = self.__score + score
        self.__runtime = self.__runtime + runtime
        if self.__judgeCnt == 0:
            self.__finalScore = Config.GetFinalScore(self.__score, self.__runtime / Config.CASE_NUM)
        return