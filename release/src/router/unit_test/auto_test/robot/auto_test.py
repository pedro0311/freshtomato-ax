#-*- coding:utf-8 -*-
import getpass
import sys
import telnetlib
import time
try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO
import os
from paramiko import SSHClient
from scp import SCPClient
from shutil import copyfile
from colorama import Fore, Back, Style
import re
from datetime import datetime
import pycurl
from io import BytesIO
from urllib.parse import urlencode
import ast
import base64
import platform

devices = []
device_host = []
device_user = []
device_passwd = []
skipline = 0

def curl_exe(value):

    buffer = BytesIO()
    c = pycurl.Curl()

    value = iter(value.split(" "))
    for val in value:
        if val == "-A": # user agent
            c.setopt(c.USERAGENT, eval(next(value)))
            continue
        elif val == "-e": # Referer URL
            c.setopt(c.REFERER, eval(next(value)))
            continue
        elif val == "-b": # cookie STRING/FILE
            c.setopt(c.COOKIEFILE, next(value))
            continue
        elif val == "-d": # HTTP POST data
            rawdata = (next(value))
            data_dict = ast.literal_eval(rawdata)
            try:
                data_dict = ast.literal_eval(data_dict)
            except Exception:
                print("Not json format.")
                if isinstance(data_dict, dict) == True:
                    postfields = urlencode(data_dict)
                else:
                    postfields = data_dict
            else:
                postfields = urlencode(data_dict)
            c.setopt(c.POSTFIELDS, postfields)
            continue
        elif val == "-D": # Save cookie.
            c.setopt(c.COOKIEJAR, next(value))
            continue
        elif val == "curl" or val == "curl.exe": # Skip
            continue
        else: # URL
            c.setopt(c.URL, val)
            continue

    c.setopt(c.WRITEDATA, buffer)
    c.perform()
    c.close()
    body = buffer.getvalue()
    # print(body.decode('iso-8859-1'))

def clear_telnet_buf(tn):
    tn.write("clear".encode('utf-8') + b'\n')
    time.sleep(1)

def if_check(device1, cmd1, exp, device2, cmd2):

    print("if_check(%s, %s, %s, %s, %s)" %(device1, cmd1, exp, device2, cmd2))
    if len(device1) <= 7:
        print("Device1 is illegal. return 0")
        return

    val1 = ''
    val2 = ''
    global skipline
    global devices
    # switch to device1
    tn = devices[int(device1.split('_')[1])]
    clear_telnet_buf(tn)

    cmd1 = cmd1.strip()
    tn.write(cmd1.encode('utf-8') + b'\n')
    time.sleep(2)
    result1 = tn.read_very_eager().decode('utf-8')
    result1 = result1.strip()
    result1 = (iter)(result1.splitlines())
    for tmp in result1:
        if tmp.find(cmd1) > -1:
            val1 = next(result1)
            break

    if len(device2) <= 7:
        val2 = cmd2
    else:
        # switch to device2
        tn = devices[int(device2.split('_')[1])]
        clear_telnet_buf(tn)
        cmd2 = cmd2.strip()
        tn.write(cmd2.encode('utf-8') + b'\n')
        time.sleep(2)
        result1 = tn.read_very_eager().decode('utf-8')
        result1 = result1.strip()
        result1 = (iter)(result1.splitlines())
        for tmp in result1:
            if tmp.find(cmd2) > -1:
                val2 = next(result1)
                break

    if val1.isdigit() == True and val2.isdigit() == False and exp != '=':
        print("Interger and String not use %s. Skip" %exp)
        return
    elif val1.isdigit() == False and val2.isdigit() == True and exp != '=':
        print("Interger and String not use %s. Skip" %exp)
        return

    if exp == '=':
        if val1 != val2:
            skipline = 1
    elif exp == '!=':
        if val1 == val2:
            skipline = 1
    elif exp == '>':
        if int(val1) <= int(val2):
            skipline = 1
    elif exp == '<':
        if int(val1) >= int(val2):
            skipline = 1
    elif exp == '>=':
        if int(val1) < int(val2):
            skipline = 1
    elif exp == '<=':
        if int(val1) > int(val2):
            skipline = 1
    else:
        print("Not support %s. Skip.", exp)
    return

def handle_general_key_word(deviceindex, line):
    global devices
    global device_host
    global device_user
    global device_passwd

    if line.find("UPLOAD=", 0, 7) > -1: # upload file to device. UPLOAD=./tmp.txt:/tmp/
        value = line.split('=')[1]
        scp_upload_file(device_host[deviceindex], device_user[deviceindex], device_passwd[deviceindex], value.split(':')[0], value.split(':')[1])
        return 1
    elif line.find("SLEEP=", 0, 6) > -1: # sleep seconds
        value = int(line.split('=')[1])
        print("SLEEP %s seconds" %value)
        time.sleep(value)
        return 1
    elif line.find("IFGO=", 0, 5) > -1: # condition for go next. eq: IFGO=30=nvram get wlc0_ssid=CHRIS_RTAC68U
        timeout = int(line.split('=')[1])
        cmd = line.split('=')[2]
        value = line.split('=')[3]
        tn = devices[deviceindex]
        matched = 0
        print("IFGO Timeout: %s CMD: %s Value: %s" %(timeout, cmd, value))
        if timeout == 0:
            timeout = 86400
        for i in range(timeout):
            if matched == 1:
                break
            cmd = cmd.strip()
            tn.write(cmd.encode('utf-8') + b"\n")
            time.sleep(1)
            ret = tn.read_very_eager().decode('utf-8')
            for line in ret.splitlines():
                if line.find(value) > -1:
                    matched = 1
        if matched == 1:
            print("IFGO matched.")
        else:
            print("IFGO timeout.")
        return 1
    elif line.find("CURL=", 0, 5) > -1: # Execute curl.
        url = line.split('=', 1)[1]
        curl_exe(url)
        return 1
    else:
        return 0

# example: sftp_upload_file("192.168.1.1", "admin", "123456789", "./test.txt", "/jffs/")
def scp_upload_file(host, user, passwd, local_path, server_path):
    print("UPLOAD FILE: %s to SERVER(%s) %s" %(local_path, host, server_path))
    ssh = SSHClient()
    ssh.load_system_host_keys()
    ssh.connect(host, username=user, password=passwd)

    scp = SCPClient(ssh.get_transport())
    scp.put(local_path, recursive=True, remote_path=server_path)
    scp.close()

def scp_download_file(host, user, passwd, server_path, local_path):
    print("DOWNLOAD FILE: %s from SERVER(%s) %s" %(local_path, host, server_path))
    ssh = SSHClient()
    ssh.load_system_host_keys()
    ssh.connect(host, username=user, password=passwd)

    scp = SCPClient(ssh.get_transport())
    scp.get(server_path, local_path=local_path,recursive=True)
    scp.close()

def telnet_connect_to_devices():
    global devices
    global device_host
    global device_passwd
    global device_user

    for host in device_host:
        print("Connect to host: %s" %host)
        tn = telnetlib.Telnet(host)
        tn.read_until(b"login:")
        tn.write(device_user[device_host.index(host)].encode('utf-8') + b"\n")
        tn.read_until(b"Password: ")
        tn.write(device_passwd[device_host.index(host)].encode('utf-8') + b"\n")
        devices.insert(device_host.index(host), tn)

def telnet_exit_devices():
    global devices
    for tn in devices:
        tn.write("exit".encode('utf-8') + b"\n")
        tn.close()

def load_device_config():
    global device_host
    global device_passwd
    global device_user
    device_host = []
    device_user = []
    device_passwd = []
    fo = open("config", "r", encoding="utf-8")
    for line in fo.readlines():
        line = line.strip()
        if line.find("IP") > -1:
            index = int(line.split('_')[1])
            host = line.split('=')[1]
            device_host.insert(index, host)
        elif line.find("USER") > -1:
            index = int(line.split('_')[1])
            user = line.split('=')[1]
            device_user.insert(index, user)
        elif line.find("PASSWD") > -1:
            index = int(line.split('_')[1])
            passwd = line.split('=')[1]
            device_passwd.insert(index, passwd)
    fo.close()
    print(device_host)
    print(device_user)
    print(device_passwd)

def enable_telnet():
    print("Enable telnet daemon.")
    global device_host
    global device_passwd
    global device_user

    for ip in device_host:
        # Login
        autho = device_user[device_host.index(ip)]+':'+ device_passwd[device_host.index(ip)]
        autho = base64.b64encode(autho.encode(encoding="utf-8"))
        cmd = 'curl -A "asusrouter-Windows-DUTUtil-1.0" ' +  'http://' + ip + '/login.cgi?login_authorization=' + autho.decode('utf-8') +' -D savecookie.txt'
        curl_exe(cmd)
        cmd = 'curl -A "asusrouter-Windows-DUTUtil-1.0" -e "' + ip + '" -b savecookie.txt -d {\"action_mode\":\"apply\",\"rc_service\":\"restart_time;restart_upnp\",\"telnetd_enable\":\"1\",\"sshd_enable\":\"1\"} http://' + ip + '/applyapp.cgi'
        curl_exe(cmd)
    time.sleep(5)


def selectTestCaseScript():
    script = ""
    testcase_script = input("Enter test case script name or skip the request: ")
    if len(testcase_script) > 0:
        if platform.system() == 'Windows':
            dst = os.path.abspath("testcase") + '\\' + testcase_script
            print("dst:%s."%dst)
        else:    
            dst = "./testcase/" + testcase_script
        if os.path.exists(dst):
            fo = open(dst, "r", encoding="utf-8")
            script = fo.readlines()
            fo.close()
        else:
            print("[WARRING] Not found test case script %s." %testcase_script)
    return script

def selectTestCase(testcase_name):  

    #find folder by test case name
    found_dir = 0
    dirpath = ''
    dirs = []
    files = []
    for (dirpath, dirs, files) in os.walk("testcase"):
        for t in dirs:
            if t == testcase_name:
                found_dir = 1
                break
        if found_dir == 1:
            break

    src = os.path.join(dirpath, testcase_name, "setup.txt")
    dst = "setup.txt"
    copyfile(src, dst)
    src = os.path.join(dirpath, testcase_name, "run.txt")
    dst = "run.txt"
    copyfile(src, dst)
    src = os.path.join(dirpath, testcase_name, "chk.txt")
    dst = "chk.txt"
    copyfile(src, dst)
    src = os.path.join(dirpath, testcase_name, "config")
    dst = "config"
    copyfile(src, dst)
    src = os.path.join(dirpath, testcase_name, "recover.txt")
    dst = "recover.txt"
    if os.path.exists(src):
        copyfile(src, dst)
    print("\n[MAIN] READY RUN TEST CASE NAME: %s" %testcase_name)
    src = os.path.join(dirpath, testcase_name, "README")

    if os.path.exists(src):
        fo = open(src, "r", encoding="utf-8")
        print("")
        for line in fo.readlines():
            line = line.strip()
            print(line)
        print("")
        fo.close()

def SetupEnviConfig():
    print("[SETUP] Setup Python Starting")

    global devices
    global skipline
    tn = devices[0]
    fo = open("setup.txt", "r", encoding="utf-8")
    index = 0
    for line in fo.readlines():
        line = line.strip()

        if line.find("ENDIF", 0, 5) > -1:
            skipline = 0
            continue
        elif line.find("ELSE", 0, 4) > -1:
            if skipline == 0:
                skipline = 1
            else:
                skipline = 0
            continue

        if skipline  == 1:
            print("Skip line %s" %line)
            continue

        if line.find("SWITCH=", 0, 7) > -1:
            print(line)
            index = int(line.split('_')[1])
            print("SWITCH to device %s" %index)
            tn = devices[index]
            continue
        elif line.find("IF=", 0, 3) > -1:
            val = line.split('=', 1)[1]
            if_check(val.split(',')[0], val.split(',')[1], val.split(',')[2], val.split(',')[3], val.split(',')[4])
            continue
        else:
            if handle_general_key_word(index, line) != 1:
                tn.write(line.encode('utf-8')+ b"\n")
                time.sleep(1)
                ret = tn.read_very_eager().decode('utf-8')
                print(ret)

    fo.close()

def runTestConfig():
    print("[RUN] Trigger test daemon to start testing.")

    global devices
    global skipline
    index = 0
    tn = devices[index]
    fo = open("run.txt", "r", encoding="utf-8")

    startcmd = ""
    donecmd = ""
    timeout = 120 # default 120 seconds

    for line in fo.readlines():
        line = line.strip()

        if line.find("ENDIF", 0, 5) > -1:
            skipline = 0
            continue
        elif line.find("ELSE", 0, 4) > -1:
            if skipline == 0:
                skipline = 1
            else:
                skipline = 0
            continue

        if skipline  == 1:
            print("Skip line %s" %line)
            continue

        if line.find("SWITCH=", 0, 7) > -1:
            index = int(line.split('_')[1])
            print("[RUN] SWITCH to device %s" %index)
            tn = devices[index]
            continue
        elif line.find("TIMEOUT") > -1:
            timeout = int(line.split('=')[-1])
        elif line.find("START=", 0, 6) > -1:
            startcmd = line.split('=')[-2]
            startval = line.split('=')[-1]
            print("[RUN] START check command: %s = %s" %(startcmd, startval))
        elif line.find("DONE", 0, 5) > -1:
            donecmd = line.split('=')[-2]
            doneval = line.split('=')[-1]
            print("[RUN] DONE check command: %s = %s" %(donecmd, doneval))
        elif line.find("IF=", 0, 3) > -1:
            val = line.split('=', 1)[1]
            if_check(val.split(',')[0], val.split(',')[1], val.split(',')[2], val.split(',')[3], val.split(',')[4])
            continue
        else:
            if handle_general_key_word(index, line) != 1:
                tn.write(line.encode('utf-8')+ b"\n")
                time.sleep(1)

    i = 0
    print("[RUN] Timeout setting is %s" % timeout)

    if len(startcmd) > 0:
        startcase = 0
    else:
        startcase = 1
    if len(donecmd) > 0:
        donecase = 0
    else:
        donecase = 1
    while i < timeout:
        if startcase == 0:
            startcmd = startcmd.strip()
            tn.write(startcmd.encode('utf-8') + b"\n")
            time.sleep(1)
            ret = tn.read_very_eager().decode('utf-8')
            print(ret)
            for line in ret.splitlines():
                if line == startval:
                    startcase = 1
                    print("[RUN] START key val found.")
        if donecase == 0 and startcase == 1:
            tn.write(donecmd.encode('utf-8') + b"\n")
            time.sleep(1)
            ret = tn.read_very_eager().decode('utf-8')
            for line in ret.splitlines():
                if line == doneval:
                    donecase = 1
                    print("[RUN] DONE key val found.")

        if startcase == 1 and donecase == 1:
            break
        i += 1

    if donecase == 0:
        print("*ERROR*[RUN] Run test case fail or timeout.")

    fo.close()

def chkResult():
    print("[CHECK] Check test case result")

    global devices
    global skipline
    tn = devices[0]

    filelist = [] # [chkfile1, chkfile2, ...]
    chkfile = [] # [device index, file location, check pattern, time check pattern]
    pattern = [] # [string1, string2, ...]
    time_pattern = [] # [string1, time1, string2, time2, ...]
    compare_list = [] # [[compare data], ...]

    fo = open("chk.txt", "r", encoding="utf-8")
    found = 0
    for line in fo.readlines():
        line = line.strip()

        if line.find("ENDIF", 0, 5) > -1:
            skipline = 0
            continue
        elif line.find("ELSE", 0, 4) > -1:
            if skipline == 0:
                skipline = 1
            else:
                skipline = 0
            continue

        if skipline  == 1:
            print("Skip line %s" %line)
            continue

        if line.find("SWITCH=", 0, 7) > -1:
            if len(chkfile) > 0:
                chkfile.insert(2, pattern.copy())
                chkfile.insert(3, time_pattern.copy())
                filelist.append(chkfile.copy())
                chkfile.clear()
                pattern.clear()
                time_pattern.clear()
            switch_index = int(line.split('_')[1])
            chkfile.insert(0, switch_index)
            continue
        elif line.find("CHECKFILE=", 0, 10) > -1:
            chkfile.insert(1, line.split('=')[1])
            continue
        elif line.find("TIMECHECK=", 0, 10) > -1:
            time_pattern.append(line.split('=')[1])
            time_pattern.append(line.split('=')[2])
        elif line.find("PATTERN=", 0, 8) > -1:
            pattern.append(line.split('=', 1)[1])
        elif line.find("COMPARE=", 0, 8) > -1:
            rawdata = line.split('=', 1)[1] # device index,cmd,<>=,device index,cmd
            rawdata = rawdata.split(',')
            compare_list.append(rawdata.copy())
        elif line.find("IF=", 0, 3) > -1:
            val = line.split('=', 1)[1]
            if_check(val.split(',')[0], val.split(',')[1], val.split(',')[2], val.split(',')[3], val.split(',')[4])
            continue
        else:
            pattern.append(line)

    if len(chkfile) > 0: # Processing for last chkfile list
        chkfile.insert(2, pattern.copy())
        chkfile.insert(3, time_pattern.copy())
        filelist.append(chkfile.copy())
        chkfile.clear()
        pattern.clear()
        time_pattern.clear()

    print("[CHECK] filelist", filelist)

    # Process filelist
    check_result = 1
    for chkfile_tmp in filelist:
        tn = devices[chkfile_tmp[0]]
        cmd = "cat" + " " + chkfile_tmp[1]
        tn.write(cmd.encode('utf-8') + b'\n')
        time.sleep(2)
        result = tn.read_very_eager().decode('utf-8')
        print("\n[CHECK] SHOW DEVICE_%s %s LOG:\n" %(chkfile_tmp[0], chkfile_tmp[1]))
        print(result)
        for pattern_tmp in chkfile_tmp[2]: # Check pattern
            found = 0
            re_pattern = re.compile(pattern_tmp)
            for line in result.splitlines():
                if re_pattern.search(line) != None:
                    found = 1
            if found == 0:
                print("[CHECK] Not found %s" %pattern_tmp)
                check_result = 0

        pattern_value = ""
        time_value = 0
        for time_pattern_tmp in chkfile_tmp[3]: # Check time pattern
            if chkfile_tmp[3].index(time_pattern_tmp) % 2 == 0:
                pattern_value = time_pattern_tmp
                continue

            time_value = int(time_pattern_tmp)
            timelist = []
            print("Pattern: %s Time: %s" %(pattern_value, time_value))
            for line in result.splitlines():
                re_pattern = re.compile(pattern_value)
                if re_pattern.search(line) != None:
                    datetmp = line.split('[')[0]
                    datetmp = datetmp.strip()
                    t = datetime.strptime(datetmp, "%a %b  %d %H:%M:%S %Y") # Wed Aug  7 16:20:33 2019
                    timelist.append(time.mktime(t.timetuple()))
            print(timelist)
            if len(timelist) > 1:
                i = 0
                for timelist_tmp in timelist:
                    if i == 0:
                        i += 1
                        continue
                    if (timelist_tmp - timelist[i-1]) < (time_value - 1) or (timelist_tmp - timelist[i-1]) > (time_value + 1):
                        check_result = 0
                        print("*ERROR*[CHECK] TIMECHECK fail. Pattern: %s TIME: %s DIFFTIME: %s" %(pattern_value, time_value, timelist_tmp - timelist[i-1]))

    # Process compare list
    for comp in compare_list:
        val1 = ''
        val2 = ''
        tn = devices[(int)(comp[0].split('_')[1])]
        comp[1] = comp[1].strip()
        tn.write(comp[1].encode('utf-8') + b'\n')
        time.sleep(2)
        result1 = tn.read_very_eager().decode('utf-8')
        result1 = result1.strip()
        result1 = (iter)(result1.splitlines())
        for tmp in result1:
            if tmp.find(comp[1]) > -1:
                val1 = next(result1)
                break
        if len(comp[3]) <= 7:
            val2 = comp[4]
        else:
            tn = devices[(int)(comp[3].split('_')[1])]
            comp[4] = comp[4].strip()
            tn.write(comp[4].encode('utf-8') + b'\n')
            time.sleep(2)
            result2 = tn.read_very_eager().decode('utf-8')
            result2 = result2.strip()
            result2 = (iter)(result2.splitlines())
            for tmp in result2:
                if tmp.find(comp[4]) > -1:
                    val2 = next(result2)
                    break
        if val1.isdigit() == True and val2.isdigit() == False:
            if comp[2] != '=' and comp[2] != '!=':
                print("Interger and String not use %s. Skip" %comp[2])
                continue
        elif val1.isdigit() == False and val2.isdigit() == True:
            if comp[2] != '=' and comp[2] != '!=':
                print("Interger and String not use %s. Skip" %comp[2])
                continue

        if comp[2] == '=':
            if val1 != val2:
                print("*ERROR*[CHECK] %s != %s. Fail." %(val1, val2))
                check_result = 0
        elif comp[2] == '!=':
            if val1 == val2:
                print("*ERROR*[CHECK] %s = %s. Fail." %(val1, val2))
                check_result = 0
        elif comp[2] == '>':
            if int(val1) <= int(val2):
                print("*ERROR*[CHECK] %s > %s. Fail." %(val1, val2))
                check_result = 0
        elif comp[2] == '<':
            if int(val1) >= int(val2):
                print("*ERROR*[CHECK] %s < %s. Fail." %(val1, val2))
                check_result = 0
        elif comp[2] == '>=':
            if int(val1) < int(val2):
                print("*ERROR*[CHECK] %s >= %s. Fail." %(val1, val2))
                check_result = 0
        elif comp[2] == '<=':
            if int(val1) > int(val2):
                print("*ERROR*[CHECK] %s <= %s. Fail." %(val1, val2))
                check_result = 0
        else:
            print("Not support %s. Skip.", comp[2])
    flag = 1
    if check_result == 1:
        print(Fore.GREEN + "\n[CHECK] PASS" + Style.RESET_ALL)
        flag = 0
    else:
        print("*ERROR*" + Fore.RED + "\n[CHECK] FAIL" + Style.RESET_ALL)

    fo.close()
    return flag
def recover_settings():
    print("[RECV] Recover devices setting.")
    global devices
    global skipline
    tn = devices[0]
    index = 0
    try:
        fo = open("recover.txt", "r", encoding="utf-8")

        for line in fo.readlines():
            line = line.strip()

            if line.find("ENDIF", 0, 5) > -1:
                skipline = 0
                continue
            elif line.find("ELSE", 0, 4) > -1:
                if skipline == 0:
                    skipline = 1
                else:
                    skipline = 0
                continue

            if skipline  == 1:
                print("Skip line %s" %line)
                continue

            if line.find("SWITCH=", 0, 7) > -1:
                print(line)
                index = int(line.split('_')[1])
                print("SWITCH to device %s" %index)
                tn = devices[index]
                continue
            elif line.find("IF=", 0, 3) > -1:
                val = line.split('=', 1)[1]
                if_check(val.split(',')[0], val.split(',')[1], val.split(',')[2], val.split(',')[3], val.split(',')[4])
                continue
            else:
                if handle_general_key_word(index, line) != 1:
                    tn.write(line.encode('utf-8') + b"\n")
                    time.sleep(1)
                    ret = tn.read_very_eager().decode('utf-8')
                    print(ret)

        fo.close()
    except:
        print("[RECV] Recover process error. Skip.")
    else:
        print("[RECV] Recover process success.")