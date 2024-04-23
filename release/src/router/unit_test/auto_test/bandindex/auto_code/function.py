import base64
import shutil
import string
import re
import os
import itertools

class RE:
    def __init__(self, name, ip):
        self.name = name
        self.ip = ip
class Key_Value:
    def __init__(self, key, value):
        self.key = key
        self.value = value
class rename:
    def __init__(self, src, det):
        self.src = src
        self.det = det
class Arg:
    def __init__(self,arg0,arg1,arg2,arg3,arg4):
        self.arg0 = arg0
        self.arg1 = arg1
        self.arg2 = arg2
        self.arg3 = arg3
        self.arg4 = arg4

def Get_Arg(str,type):
    tmp = str.split()
    print("tmp:",tmp)
    arg = ''
    for t in tmp:
        print("t:",t)
        a1 = ""
        a2 = ""
        if "_" in t:
            t1 = t.split("_")[0]
            print("t1:",t1)
            if "DUALBAND" in t1: a1 = "0"
            elif "TRIBAND6G" in t1: a1 = "2"
            elif "TRIBAND" in t1: a1 = "1"
            elif "QUADBAND" in t1: a1 = "3"
            t2 = t.split("_")[1]
            if "v1" in t2: a2 = "1"
            elif "v2" in t2: a2 = "2"
            print(a1,",",a2)
            if type == "RE":
                arg = Arg("0",a1,a2,"0","1")
            else:
                arg = Arg("1", a1, a2, "0", "1")
    return arg
def Handle_Files_Original(original:str,end:str):
    original = original[:len(original)-len(end)]
    print(original)
    originals = original.split()
    print(originals)
    handle_folders = []
    if len(originals)>2:
        for folder in originals:
            if folder=="." or folder=="..":
                continue
            print("folder:",folder)
            print("**folder:",folder)
            handle_folders.append(folder)
    return handle_folders

def Change_File(files:list):
    rename_files = []
    for file in files:
        print("file:",file)
        pattern = r'^[A-Z0-9]{12}\..*$'
        match = re.match(pattern, file)
        if match:
            new_filename = f"{file[:2]}:{file[2:4]}:{file[4:6]}:{file[6:8]}:{file[8:10]}:{file[10:]}"
            #print("new_filename:", new_filename)
            rename_files.append(rename(file,new_filename))
    return rename_files
def Get_Folder(path):
    folders = []
    for item in os.listdir(path):
        item_path = os.path.join(path, item)
        if os.path.isdir(item_path):
            folders.append(item)
    return folders

def Get_Files(path):
    files = []
    for item in os.listdir(path):
        item_path = os.path.join(path, item)
        #print("item_path:",item_path)
        if item == "amas" and os.path.isdir(item_path):
            for amas_item in os.listdir(item_path):
                amas_item_path = os.path.join(item_path,amas_item)
                if os.path.isfile(amas_item_path):
                    #print("amas_item_path:",amas_item_path)
                    files.append("amas/"+amas_item)
            continue
        if os.path.isfile(item_path):
            files.append(item)
    #print("files:",files)
    return files


def Get_Result_File(message,path):
    '''file = open(os.path.normpath("C:\\Users\\Anne_Zheng\\Documents\\CAP\\FILE\\cfg.json"), "r")
    message = file.read()
    file.close()'''
    flag = 0
    message = message[2:]
    print("message:",message)
    if message.find("cfg_ver"):
        index = message.find("cfg_ver")
        print("index:",index)
        if index > 2:
            str1 = message[index - 2:]
            print("str1:", str1)
            pattern = r"\{(.*?)\}"
            matches = re.findall(pattern, str1)
            if matches:
                str2 = "{" + matches[0] + "}"
                print(str2)
                try:
                    file = open(os.path.normpath(path), "w")
                    file.write(str2)
                    file.close()
                    flag = 1
                except Exception as e:
                    print("*ERROR*" + str(e))
                    raise
            else:
                index_end = message.find("login")
                if index_end>0:
                    str1 = message[:index_end]
                    str1 = str1 + "}"
                    try:
                        file = open(os.path.normpath(path), "w")
                        file.write(str1)
                        file.close()
                        flag = 1
                    except Exception as e:
                        print("*ERROR*" + str(e))
                        raise
                else:
                    print("cfg_ver log 不全面")
        else:
            print("cfg_ver 出现错误")
    else:
        print("cfg不存在")
    return flag


def get_file_text(path):
    if len(path)>0:
        try:
            file = open(os.path.normpath(path),"r")
            file_text = file.read()
            file.close()
            return file_text
        except Exception as e:
            print("*ERROR*" + str(e))
            raise
def get_file_name(str:str,pattern):
    files = []
    tmp = str.split()
    for file_name in tmp:
        print(file_name)
        if re.match(pattern,file_name):
            if "ahsjson.json" not in file_name:
                print("****",file_name)
                files.append(file_name)
    print(files)
    return files

def Combination(options,m):
    combinations = list(itertools.combinations(options, m))
    return combinations

def Log_uncheckout_text(combination,td_text:str):
    input_tests = td_text.split()
    log_check = ""
    log_index = []
    print("combination:",combination)
    if type(combination) == str:
        log_index.append(int(combination[len(combination)-1]))
        print("index:",int(combination[len(combination)-1]))
    elif isinstance(combination,tuple):
        for tmp in combination:
            log_index.append(int(tmp[len(tmp) - 1]))
            print("for:",int(tmp[len(tmp) - 1]))
    else:
        print(type(combination))
    index = 0
    print("log_index:",log_index)
    for i in range(0,len(input_tests),2) :
        print("**:",input_tests[i]+input_tests[i+1])
        if index not in log_index:
            log_check = log_check + "_" + input_tests[i]+input_tests[i+1]
        index = index + 1
    print(log_check)
    return log_check
def Set_RE_IP(cap_ip,cap_name,re_ip,re_name):
    my_re = []
    my_re.append(RE(cap_name,cap_ip))
    my_re.append(RE(re_name,re_ip))
    return my_re

def get_RE_Ip(str):
    tmp_str = str.split(">")
    my_re = []
    for i in range(len(tmp_str)):
        print("tmp:", tmp_str[i])
        if "<" in tmp_str[i]:
            name = tmp_str[i].split("<")[1]
            print("name:",name)
            i = i + 1
            ip = tmp_str[i]
            print("ip:", ip)
            my_re.append(RE(name,ip))
        #
    for re in my_re:
        print(re.name,re.ip)
    return my_re
def Copy_Files_Fun(src_file,dst_file):
    shutil.copy2(src_file, dst_file)

def Copy_File_Fun(log_path:str,name:str,context:str,end:str):
    print("log_path:",log_path,"name:",name,"context:",context,"end:",end)
    if os.path.exists(log_path):
        print('文件夹存在')
    else:
        os.mkdir(log_path)
    if "amas" in name:
        if os.path.exists(os.path.normpath(log_path+"\\amas")):
            print('文件夹存在')
        else:
            os.mkdir(os.path.normpath(log_path+"\\amas"))
        log_path = log_path
    file_name = name.replace(':', '')
    print(context)
    try:
        file = open(os.path.normpath(log_path+"/"+file_name),"w")
        length = len(context)
        length = length - len(end)
        print(length)
        context = context[0:length]
        file.write(context)
        file.close()
    except Exception as e:
        print("*ERROR*" + str(e))
        raise
#获取RE的band的对应ssid
def Get_RE_Band(band_original:str,end:str):
    bands = band_original.split()
    re_band = []
    band5_flag = band6_flag = 0
    for band in bands:
        if "_nband" in band:
            print("band:",band)
            if band[2].isdigit():
                index = band[10]
                print("index:", index)
                if index == '2':
                    re_band.append(Key_Value(band[2],"2G"))
                elif index == '1':
                    band5_flag = band5_flag + 1
                    if band5_flag == 1: re_band.append(Key_Value(band[2],"5GL"))
                    elif band5_flag == 2: re_band.append(Key_Value(band[2], "5GH"))
                elif index == '4':
                    band6_flag = band6_flag + 1
                    if band6_flag == 1: re_band.append(Key_Value(band[2],"6GL"))
                    elif band6_flag == 2: re_band.append(Key_Value(band[2], "6GH"))
    print("band5_flag:",band5_flag,",band6_flag:",band6_flag)
    if band5_flag == 1:
        re_band = [Key_Value(item.key,'5G') if item.value == '5GL' else item for item in re_band]
    if band6_flag == 1:
        re_band = [Key_Value(item.key,'6G') if item.value == '6GL' else item for item in re_band]
    for band in re_band:
        print("key:",band.key,",value:",band.value)
    re_order_band = []
    order_rule = ["2G", "5G", "5GL", "5GH", "6G", "6GL", "6GH"]
    for orfer_rule_index in order_rule:
        for re_name in re_band:
            if re_name.value == orfer_rule_index:
                re_order_band.append(re_name)
                break

    return re_order_band
def Get_String_Index(text:str,str1:str,value):
    tmp = text.find(str1)
    print("text:",text)
    if(tmp>0):
        print("tmp:",tmp)
        tmp_lists = text.split(",")
        for tmp_list in tmp_lists:
            if str1 in tmp_list:
                key_value = tmp_list.split(":")
                print("key_value:",key_value)
                if len(key_value) >= 2:
                    return key_value[1][1:len(key_value[1])-1]
    return value
def get_write_text(text,end):
    text_len = len(text)
    end_len = len(end)
    new_len = text_len-end_len
    if new_len-2 <= 0:
        return ""
    print("text_len:",text_len,",end_len:",end_len,",new_len:",new_len)
    return text[0:new_len-2]
if __name__ == "__main__":
    Get_RE_Band("wl0_nband=1\nwl1_nband=1\nwl2_nband=4\nwl3_nband=2\nwl_nband=1\n","gt-axe16000")
    #tmp = Get_String_Index("\"wireless\":{\"wl0_ssid\":\"admin\",\"wl0_closed\":\"0\",\"wl0_wpa_psk\":\"admin123\",\"wl0_auth_mode_x\":\"psk2\",\"wl0_crypto\":\"aes\",\"wl0_wep_x\":\"0\",\"wl0_key\":\"1\"","wl0_auth_mode_x","owe")
    #print("tmp:",tmp)
    #Log_uncheckout_text("smart_connect_check_0","2.4 GHz 5 GHz-1 5 GHz-2 6 GHz")
    #Get_Result_File("C:\\Users\\Anne_Zheng\\Documents\\CAP\\FILE\\cfg111.json")
    #get_RE_Ip("<GT-AXE16000>192.168.51.1>04:42:1A:6A:8E:C0>1<GT-AXE16000>192.168.51.181>04:42:1A:6A:90:90>0")
    #Copy_File("C:\\Users\\Anne_Zheng\\Documents\\CAP\\","04:42:1A:6A:8E:C0.cap","{\"0\":{\"unit\":0},\"1\":{\"unit\":1},\"4\":{\"unit\":2}}admin@GT-AXE16000-8EC0","admin@GT-AXE16000-8EC0")