import os
import time
import subprocess
from path import PathMgr
from config import Config

class ResMgr:
    def ClearSolu():
        path = PathMgr.GetSoluFolderPath()
        os.system("rm -rf " + path + "*")
        return

    def WaitProc(proc, maxTime):
        try:
            startTime = time.time()
            proc.wait(maxTime)
            endTime = time.time()
            return endTime - startTime
        except:
            return -1

    def WaitProcWithLog(caseName, proc, maxTime):
        try:
            cnt = 0
            while True:
                cnt = cnt + 1
                if proc.poll() == None :
                    if cnt > maxTime:
                        return -1
                    print("[用户程序-进行] 用例：%s 耗时=%ds" % (caseName, cnt))
                else :
                    break
                time.sleep(1)
            return cnt
        except:
            return -1
        return

    def GenProc():
        print("[编译-开始]")
        logFile = open(PathMgr.GetBuildLogPath(), "w")
        proc = subprocess.Popen("sh build.sh", cwd=PathMgr.GetShellFolderPath(), stdout=logFile, stderr=logFile, shell=True)
        detaTime = ResMgr.WaitProc(proc, Config.TIME_MAX_BUILD)
        logFile.close()

        logFile = open(PathMgr.GetBuildLogPath(), "r", encoding='utf-8')
        try:
            logStr = logFile.read()
        except:
            return Config.ERR_CODE_BUILD_FAIL
        logLines = logStr.split('\n')
        logFile.close()
        print(logStr)
        print("[编译-结束]")
        if logLines[-2]=="make return: 0":            # 编译成功
            return detaTime
        elif detaTime == -1:                            # 编译超时
            return Config.ERR_CODE_BUILD_TIMEOUT
        else:                                           # 编译失败
            return Config.ERR_CODE_BUILD_FAIL

    def RunProc(caseId):
        caseName = Config.CASE_NAME_VEC[caseId - 1]
        print("[用户程序-开始] 用例：" + caseName)
        casePath = PathMgr.GetCasePath(caseId)
        soluPath = PathMgr.GetSoluPath(caseId)
        logFile = open(PathMgr.GetRunLogPath(caseId),"w")
        args =  "sh run.sh " + casePath + " " + soluPath
        proc = subprocess.Popen(args, cwd=PathMgr.GetShellFolderPath(), stderr=logFile, shell=True)
        detaTime = ResMgr.WaitProcWithLog(caseName, proc, Config.TIME_MAX_RUN)
        print("[用户程序-结束] 用例：" + caseName)
        logFile = open(PathMgr.GetRunLogPath(caseId), "r", encoding='utf-8')
        try:
            logStr = logFile.read()
        except:
            return Config.ERR_CODE_RUN_FAIL
        logFile.close()
        print(logStr)
        if logStr != "":
            return Config.ERR_CODE_RUN_FAIL
        elif detaTime == -1:
            return Config.ERR_CODE_RUN_TIMEOUT
        else:
            return detaTime