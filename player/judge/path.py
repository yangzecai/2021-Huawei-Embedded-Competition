from config import Config

class PathMgr:
    def GetCaseFolderPath():
        return Config.PATH_ROOT + "judge/cases/"
    def GetSdkFolderPath():
        return Config.PATH_ROOT + "sdk/"
    def GetShellFolderPath():
        return Config.PATH_ROOT + "sdk/shell"
    def GetSoluFolderPath():
        return Config.PATH_ROOT + "judge/solu/"
    def GetLogFolderPath():
        return Config.PATH_ROOT + "judge/log/"


    def GetCasePath(caseId):
        return PathMgr.GetCaseFolderPath() + Config.CASE_NAME_VEC[caseId-1] + ".case"
    def GetBuildPath():
        return PathMgr.GetShellFolderPath() + "shell/build.sh"
    def GetRunPath():
        return PathMgr.GetShellFolderPath() + "shell/run.sh"
    def GetSoluPath(caseId):
        return PathMgr.GetSoluFolderPath() + Config.CASE_NAME_VEC[caseId-1] + ".solu"
    def GetBuildLogPath():
        return PathMgr.GetLogFolderPath() + "build.log"
    def GetRunLogPath(caseId):
        return PathMgr.GetLogFolderPath() + Config.CASE_NAME_VEC[caseId-1] + ".log"