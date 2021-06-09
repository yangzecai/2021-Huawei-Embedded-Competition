from config import Config

class IntReader:
    def IsIntStr(str):
        try:
            int(str)
            return True
        except ValueError:
            return False

    def __init__(self, fileAddr):
        file = open(fileAddr)
        lines = file.readlines()
        file.close()
        self.valueMgr = []
        self.__curRowId = 0
        self.__curColId = 0
        self.errCode = Config.ERR_CODE_SUCCESS
        self.isLineEnd = False
        for line in lines:
            itemVec = line.split(' ')
            valueVec = []
            for item in itemVec:
                if IntReader.IsIntStr(item):
                    valueVec.append(int(item))
                elif item != "\n":
                    self.errCode = Config.ERR_CODE_OUTPUT_INVALID
                    return
            self.valueMgr.append(valueVec)
        if len(self.valueMgr) == 0:
            self.errCode = Config.ERR_CODE_OUTPUT_EMPTY
        return

    def Read(self):
        if self.__curRowId >= len(self.valueMgr):
            self.errCode == Config.ERR_CODE_OUTPUT_FEW
            return -1
        if self.__curColId >= len(self.valueMgr[self.__curRowId]) :
            self.__curRowId = self.__curRowId + 1
            self.__curColId = 0
        if self.__curRowId >= len(self.valueMgr):
            self.errCode == Config.ERR_CODE_OUTPUT_INVALID
            return -1
        value = self.valueMgr[self.__curRowId][self.__curColId]
        self.__curColId = self.__curColId + 1
        if self.__curColId == len(self.valueMgr[self.__curRowId]) :
            self.isLineEnd = True
        else :
            self.isLineEnd = False
        return value