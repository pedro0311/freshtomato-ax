import datetime
import os.path
import tempfile
import copy

class PythonListener:
    ROBOT_LISTENER_API_VERSION = 3

    def __init__(self,path=""):
        # Mark whether build FW succeeds. 1 indicates success and 2 indicates failure.
        self.flag = 0
        # Flags whether an error occurred in testcase.
        self.fail = 0
        if path == "":
            try:
                with open("./TEST_DUT.txt",encoding='utf-8', errors='ignore') as file:
                    line = file.readline()
                    while line:
                        tmp = line.split()
                        if "${VAR_PATH}" in tmp[0]:
                            path = tmp[1]
                        line = file.readline()
            except Exception as e:
                self.flag = 2
                print(e)

        if len(path) > 0:
            try:
                with open(path+".resource", encoding='utf-8', errors='ignore') as file:
                    line = file.readline()
                    self.tags = []
                    while line:
                        tmp = line.split()
                        if "${DUT_NAME}" in tmp[0]:
                            self.tags.append(tmp[1])
                        elif "${DUT_PARA}" in tmp[0]:
                            for tmp_tmp in tmp[1:]:
                                if tmp_tmp[0] == '#':
                                    break
                                self.tags.append(tmp_tmp.split("=y")[0])
                        line = file.readline()
            except Exception as e:
                print("*ERROR*" + str(e))
                self.flag = 2
        else:
            self.flag = 2
            print("*ERROR* The DUT name is empty!")

    def start_suite(self, data, suite):
        if self.flag == 1:
            print("Start")
            suite_tmp = copy.deepcopy(data)
            data.tests.clear()
            for t in suite_tmp.tests:
                for j in t.tags:
                    if j in self.tags:
                        print("TAG is:", t.tags)
                        data.tests.append(t)
                        break
        elif self.flag == 2:
            data.tests.clear()

    def end_suite(self, data, suite):
        if suite.status == "PASS" and self.flag == 0:
            self.flag = 1
            print("RUN ", self.tags)
        elif suite.status == "FAIL" and self.flag == 0:
            self.flag = 2
            self.fail = 1
            print("DEMO FAIL")
        elif suite.status == "FAIL":
            self.fail = 1

    def close(self):
        if self.fail == 1:
            file = open("./fail.txt", "a")
            now = datetime.datetime.now().strftime('%Y-%m-%d_%H:%M:%S')
            file.write(now)
            file.write("----------------fail\n")
            file.close()
