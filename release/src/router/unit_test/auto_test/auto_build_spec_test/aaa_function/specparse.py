import copy

import requests
from lxml.html import etree
import csv
import sys
import getopt
from selenium import webdriver
import os
import time
import shutil
import os.path
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service
import re


# branch_Path = "C:\\Users\\binbin_ni\\Desktop\\3.0.0.4.386.45892\\3.0.0.4.386.45892\\branch"
# models_Path = 'C:\\Users\\binbin_ni\\Desktop\\3.0.0.4.386.45892\\3.0.0.4.386.45892\\models'
# base_dir="C:\\Users\\binbin_ni\\Downloads\\"   #chrome download path

'''
def Sys_getopt(argv):
    source_file = compare_file = output_file = password = username = ""
    try:
        opts, args = getopt.getopt(argv, "hs:c:o:u:p:", [
                                   "help", "source file=", "compare file directory=", "outputfile=", "username=", "password="])
    except getopt.GetoptError:
        print('Error: new.py -s <source file> -c <compare file directory> -o <outputfile> -u <username> -p <password>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print('new.py -s <source file> -c <compare file directory> -o <outputfile> -u <username> -p <password>')
            sys.exit()
        elif opt in ("-s", "--source file"):
            source_file = arg
        elif opt in ("-c", "--compare file directory"):
            compare_file = arg
        elif opt in ("-o", "--output file"):
            output_file = arg
        elif opt in ("-p", "--password"):
            password = arg
        elif opt in ("-u", "--username"):
            username = arg
    return source_file, compare_file, output_file, username, password,

'''
def GetCookie(username, password):
    options = Options()
    options.add_argument('--no-sandbox')
    options.add_argument('--headless')
    s = Service(executable_path=r'C:\\Users\\anne_zheng\\AppData\Local\\Microsoft\\WindowsApps\\chromedriver')
    
    #chromedriver = "C:\\Users\\anne_zheng\\AppData\Local\\Microsoft\\WindowsApps\\chromedriver"  # 这里写本地的chromedriver 的所在路径
    # chromedriver = "C:\\Users\\binbin_ni\\AppData\\Local\\Programs\\Python\\Python38\\chromedriver"  # 这里写本地的chromedriver 的所在路径
    #driver = webdriver.Chrome(chromedriver, options=options)
    driver = webdriver.Chrome(service=s, options=options)
    driver.get(url)
    driver.refresh()  # 刷新页面
    #driver.maximize_window() #浏览器最大化
    #print("username=",username,";password=",password)
    time.sleep(5)
    driver.find_element(By.ID,"username").send_keys(username)
    driver.find_element(By.ID,"password").send_keys(password)
    time.sleep(5)
    driver.find_element(By.NAME,"login").click()  
    #print("username=",username,";password=",password)
    try:
        login_error = driver.find_element(By.ID,"flash_error").text
        if login_error != "":
            print(login_error)
    except:
        cookie_items = driver.get_cookies()
        cookie_str = ""
        for item_cookie in cookie_items:
            item_str = item_cookie["name"] + "=" + item_cookie["value"] + "; "
            cookie_str += item_str
        return cookie_str


def GetOdmpidDiff(model_spec_csv_path, autobuild_csv_path, build_model):
    #print("model_spec_csv_path:",model_spec_csv_path,",autobuild_csv_path:",autobuild_csv_path,",build_model:",build_model)
    #打开readmine上获取的spec文件
    with open(model_spec_csv_path, 'rt', encoding='big5') as spec_csv:
        spec_reader = csv.DictReader(spec_csv)
        spec_odmpid = ['']
        autobuild_odmpid = ['']

        for row in spec_reader:
            if row['主旨'] == 'SWHW_Auth':
                spec_odmpid = row['odmpid'].split()
        for i in range(len(spec_odmpid)):
            spec_odmpid[i] = spec_odmpid[i].replace(',', '')

    with open(autobuild_csv_path, 'rt', encoding='big5') as autobuild_csv:
        autobuild_csv = open(autobuild_csv_path, 'r')
        autobuild_reader = csv.DictReader(autobuild_csv)

        for row in autobuild_reader:
            if row['Models'] == build_model:
                autobuild_odmpid = row['odmpid'].split()
                for i in range(len(autobuild_odmpid)):
                    autobuild_odmpid[i] = autobuild_odmpid[i].replace(',', '')

    autobuild_odmpid.append(build_model)

    missing_odmpid = ''
    debug_odmpid = 'spec:'

    for model in spec_odmpid:
        if model not in autobuild_odmpid:
            if missing_odmpid == '':
                missing_odmpid = model
            else:
                missing_odmpid = missing_odmpid + '\n' + model

    for model in spec_odmpid:
        debug_odmpid = debug_odmpid + model + ' '
    debug_odmpid = debug_odmpid + '\nautobuild:'
    for model in autobuild_odmpid:
        debug_odmpid = debug_odmpid + model + ' '

    return missing_odmpid

# 补全链接


def GetUrlList(sw_query):
    list_url = []
    current_url = str(redmine)
    for i in range(0, len(sw_query)):
        complete_url = current_url + sw_query[i]
        list_url.append(complete_url)
    return list_url

# 下载sw spec


def GetSWSpec(num, a):
    url_querypage = sw_urllist[num]  # 传递要下载的model sw spec
    query_html = session.get(url_querypage, headers=headers)
    querydata = query_html.text
    etree_html = etree.HTML(querydata)  # 将页面转换成文档树
    csv_url = etree_html.xpath('//a[@class="csv"]//@href')  # 所有models对应的html标签
    list_url = []
    current_url = str(redmine)  # "http://172.16.98.238:3000/"
    for i in range(0, len(csv_url)):
        complete_url = current_url + csv_url[i]
        list_url.append(complete_url)
    file_path = sw_path  # 文件保存途径
    file_name = file_path + a.split('/')[-1] + '.csv'  # 文件命名
    #print("file_name=",file_name)                                # D:\SW_spec\xxx.csv
    # print(list_url[0])                               # http://172.16.98.238:3000//projects/asuswry/issues.csv?query_id=392

    r = requests.get(list_url[0], headers=headers)
    #print("r.text:",r.text)
    try:
        if not os.path.exists(file_path):  # 检查文件夹是否存在
            os.mkdir(file_path)

        if os.path.exists(file_name):  # 检查文件是否存在
            b = r.raise_for_status()
            os.remove(file_name)  # 移除旧文件
            with open(file_name, 'wb') as f:  # 开始写入
                f.write(r.content)
                f.close()
                return file_name
        else:
            with open(file_name, 'wb') as f:  # 开始写入
                f.write(r.content)
                f.close()
                return file_name
    except:
        return print("获取失败(文件正在被占用或其他未知error)")

# Build 中的models


def GetBuildSpecList(a):
    with open(build_swpath, 'rt', encoding='UTF-8') as csvfile:
        reader = csv.DictReader(csvfile)
        column = [row[a] for row in reader]
        return column

# 获取SW Spec中dict


def GetSWSpecList(get_SWPath):
    with open(get_SWPath, 'rt', encoding='big5') as csvfile:
        reader = csv.DictReader(csvfile)
        gist = []
        function_enable = []
        for row in reader:
            gist += [row['主旨']]
            function_enable += [row['Function_Enable']]
        nvs1 = zip(gist, function_enable)
        nvDict1 = dict((gist, function_enable)
                       for gist, function_enable in nvs1)
        return nvDict1

# Build


def GetBuildSpec(num):
    with open(build_swpath, 'rt', encoding='UTF-8') as csvfile:
        reader = csv.reader(csvfile)
        for i, rows in enumerate(reader):
            if i == num + 1:
                row = rows
        return row
# 转换


def GetListSwitch(build_list):
    for index, value in enumerate(build_list):
        if value == 'V':
            build_list[index] = '''Enable'''
        elif value == ' ':
            build_list[index] = '(N/A)'

# Get branch  ----------new


def Get_Branch(get_SWPath, branch_Path):     # 新增内容
    gist = []
    c_branch = []
    print("Open: %s"%(branch_Path))
    try:
        f = open(branch_Path, "rt")
        text_branch = (f.read()).replace('\n', "")
    except:
        print("No branch file.")
        text_branch = "N/A"

    #print(text_branch)

    with open(get_SWPath, 'rt', encoding='big5') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            gist += [row['主旨']]
            c_branch += [text_branch]
        # print(gist)
        nvs1 = zip(gist, c_branch)
        nvDict1 = dict((gist, c_branch) for gist, c_branch in nvs1)
        return nvDict1

# 获取SW Spec中Enable Branch


def GetSWSpecList_EnableBranch(get_SWPath):
    with open(get_SWPath, 'rt', encoding='big5') as csvfile:
        reader = csv.DictReader(csvfile)
        gist = []
        enable_branch = []
        for row in reader:
            gist += [row['主旨']]
            enable_branch += [row['Enable_branch']]
        nvs1 = zip(gist, enable_branch)
        nvDict1 = dict((gist, enable_branch) for gist, enable_branch in nvs1)
    return nvDict1


def GetLocalSize():
    d = {}
    with open(build_swpath, 'rt', encoding='big5') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:

            d.setdefault(row['Models'], []).append(row['FW SIZE'])
            d.setdefault(row['Models'], []).append(row['JFFS SIZE'])
            d.setdefault(row['Models'], []).append(row['NVRAM SIZE'])
        # print("SW Spec: ",d)
    return d


def Size(build_swpath, build_model):
    name = (build_model.split('/')[-1]).swapcase()
    # print(name)
    num = 0
    numlist = []
    re_fwsize = r'fwsize":"(.*?)"'
    re_nvramsize = r'nvramsize":"(.*?)"'
    re_jffssize = r'jffssize":"(.*?)"'
    re_modelname = r'(.*?) ;'

    try:
        f = open(models_Path, "rt")
        list_size = []
        for i in f.readlines():
            if name in i:
                try:
                    modelname = (re.findall(re_modelname, i))[0]
                except:
                    modelname = i.replace("\n", "")
                if modelname == name:
                    print("原文： ", i.replace("\n", ""))
                    if ("fwsize" or "nvramsize" or "jffssize") in i:
                        # print("有相關內容...")

                        tx_fwsize = re.findall(re_fwsize, i)
                        if len(tx_fwsize):
                            # print("fwsize: ", tx_fwsize[0])
                            list_size.append(tx_fwsize[0])
                        else:
                            tx_fwsize = ''
                            list_size.append(tx_fwsize)
                            # print("fwsize為空值，暫無法比較")

                        tx_nvramsize = re.findall(re_nvramsize, i)
                        if len(tx_nvramsize):
                            # print("nvramsize: ", tx_nvramsize[0])
                            list_size.append(tx_nvramsize[0])
                        else:
                            tx_nvramsize = ''
                            list_size.append(tx_nvramsize)
                            # print("nvramsize為空值，暫無法比較")

                        tx_jffssize = re.findall(re_jffssize, i)
                        if len(tx_jffssize):
                            # print("jffssize: ", tx_jffssize[0])
                            list_size.append(tx_jffssize)
                        else:
                            tx_jffssize = ''
                            list_size.append(tx_jffssize)
                            # print("jffssize為空值，暫無法比較")
                    else:
                        print("暫無該型號size相關資訊")
                        list_size = ['', '', '']
                    # print(list_size)
    except:
        print("No models file, skip parse")
        list_size = ['', '', '']

    return list_size


def Compare_Size(build_model):  # 比較結果
    swspec_size = GetLocalSize()  # sw spec size  新size
    result_fwsize = ""
    result_nvramsize = ""
    result_jffssize = ""
    result = []
    models_size = Size(build_swpath, build_model)  # models size 標準
    print("FW size check:", swspec_size[build_model])  # sw spec size  新size
    # 比較

    if models_size[0] != "":
        print(models_size[0])
        try:
            if int(swspec_size[build_model][0]) <= int(models_size[0]):
                # print(swspec_size[a][0])
                # print(models_size[0])
                print("PASS")
                result_fwsize = "PASS"
            else:
                print("超過上限")
                result_fwsize = "overflow: FW(" + \
                    swspec_size[build_model][0] + "," + models_size[0] + ")"
                print(result_fwsize)
        except:

            print("SW Spec中fwsize無值，biild fail")
            result_fwsize = "BUFWFAIL"

    else:
        print("models中fwsize無值，不比較")
        result_fwsize = "No fwsize in models"

     # nvramsize
    if models_size[1] != "":
        #print(swspec_size[build_model][1])
        #print(models_size[1])
        try:
            if int(swspec_size[build_model][1]) <= int(models_size[1]):
                # print(swspec_size[a][1])
                # print(models_size[1])
                print("PASS")
                result_nvramsize = "PASS"
            else:
                print("超過上限")
                result_nvramsize = "overflow: FW(" + \
                    swspec_size[build_model][1] + "," + models_size[1] + ")"
                print(result_nvramsize)
        except:

            print("SW Spec中nvramsize無值，不比較")
            result_nvramsize = "NA(No nvramize in models)"
    else:
        print("models中nvramsize無值，不比較")
        result_nvramsize = "NA"

    # jffssize
    if models_size[2] != "":
        try:
            if int(swspec_size[build_model][2]) <= int(models_size[2]):
                # print(swspec_size[a][2])
                # print(models_size[2])
                print("PASS")
                result_jffssize = "PASS"
            else:
                print("超過上限")
                result_jffssize = "overflow: FW(" + \
                    swspec_size[build_model][2] + "," + models_size[2] + ")"
                print(result_jffssize)
        except:
            print("SW Spec中jffssize無值，不比較")
            result_jffssize = "NA(No jffssize in models)"

    else:
        print("models中jffssize無值，不比較")
        result_jffssize = "NA"

    # if result_fwsize=="PASS":

    result = "fwsize:"+result_fwsize + " nvramsize:" + \
        result_nvramsize + " jffssize:"+result_jffssize
    print(result)
    return result

# def Download_and_Copypath(name):   #新增内容
#     print("Get Chorme Download Path and Copy to D:\SW_spec")
#     time.sleep(1)
#     l = os.listdir(base_dir)
#     l.sort(key=lambda fn: os.path.getmtime(base_dir + fn) if not os.path.isdir(base_dir + fn) else 0)
#     # d=datetime.datetime.fromtimestamp(os.path.getmtime(base_dir+l[-1]))
#     print('最后改动的文件是' + l[-1])
#     newname = (name + ".csv")
#     if "issue" in l[-1]:
#         print("waiting...")
#         os.rename((base_dir + l[-1]), (base_dir +newname))
#         shutil.copy((base_dir + newname), ("D:\\SW_spec\\"))
#     else:
#         print("error...and please to check Chrome Download Path...")


title = 0


def Write(dyname, diff_vals, diff_branch, result_size, missing_odmpid):
    # 打开文件，追加a
    global title
    ''''
    with open(result_path, 'r')as f:
        read = csv.reader(f)
        for index, info in enumerate(read):
            if index == 0:
                # print(info)
                # print(info[0:1])
                if len(info[0:1]) == 0:
                    title = 0
    #                 print(",",title)
    # print(title)
    '''
    if title == 0:
        print("w")
        out = open(result_path, 'w', newline='')
        csv_write = csv.writer(out, dialect='excel')
        csv_write.writerow(
            ['MODEL', 'RESULT', 'FALL REASON("Function","FW Config","SW spec")', 'Size Check', 'Missing odmpid'])
        title = 1
    else:
        print("a")
        out = open(result_path, 'a', newline='')
        csv_write = csv.writer(out, dialect='excel')
    # if title == 0:
    #     csv_write.writerow(['MODEL','RESULT','FALL REASON("Function","FW Config","SW spec")','Size Check'])
    #     print("aaaaaaaaaaa")
    #     title = 1
    # 写入具体内容
    #csv_write.writerow(['MODEL','RESULT','FALL REASON(''Function'',''Config'',''SW spec'')'])

    if diff_vals == [] and diff_branch == [] and result_size == [] and missing_odmpid == '':

        csv_write.writerow([dyname, 'PASS', "", "", missing_odmpid])
    elif diff_vals == [] and diff_branch == [] and missing_odmpid == '':

        csv_write.writerow([dyname, 'PASS', "", result_size, missing_odmpid])
    #elif diff_vals == [] and missing_odmpid == '':

    #    csv_write.writerow([dyname, 'PASS', diff_branch,
    #                       result_size, missing_odmpid])
    elif diff_vals == 'Do not find model name in Redmine or unknown issue.' and result_size == []:
        # csv_write.writerow([dyname,"",diff_vals+","+diff_branch,result_size])
        csv_write.writerow([dyname, "", diff_vals, "", missing_odmpid])
    elif diff_vals == 'Do not find model name in Redmine or unknown issue.':
        # csv_write.writerow([dyname,"",diff_vals+","+diff_branch,result_size])
        csv_write.writerow(
            [dyname, "NA", diff_vals+diff_branch, result_size, missing_odmpid])
    elif "BUFWFAIL" in result_size:
        csv_write.writerow([dyname, 'BUILD FW FAIL'])
    else:
        csv_write.writerow(
            [dyname, 'FAIL', diff_vals+diff_branch, result_size, missing_odmpid])
    out.close()


def specparse(redmine_path,url_path,s_path,c_path,o_path,red_user,red_pwd):
    flag = 0
    global redmine
    redmine = redmine_path
    global url
    url = url_path
    global  build_swpath
    build_swpath = s_path + "\\autobuild_speccheck.csv"
    global sw_path
    print("c_path:",c_path)
    if not c_path.strip():
        sw_path = s_path + "\\"
    else:
        sw_path = c_path + "\\"
    global result_path
    result_path = o_path + "\\result.csv"
    username = red_user
    password = red_pwd
    url_branch = build_swpath.split('autobuild_speccheck.csv')[0]
    branch_Path = url_branch+"\\branch"
    global models_Path
    models_Path = url_branch+"\\models"
    try:
        getcookie = GetCookie(username, password)
        # headers = {
        #     'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36 Edge/18.18362',
        #     'Cookie': getcookie, }
        global headers
        headers = {
            "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.54 Safari/537.36",
            'Cookie': getcookie, }
        global session
        session = requests.Session()
        response = session.get(url, headers=headers)
        print('是否打开Redmine: ', response.ok)

        wb_data = response.text
        html = etree.HTML(wb_data)  # 将页面转换成文档树
        sw_queries = html.xpath(
            '//ul[@class="queries"]//li//a//text()')  # SW spec name
        # 所有models对应的html标签
        sw_query = html.xpath('//ul[@class="queries"]//li//a//@href')
        print("sw_queries")
        #print(sw_queries)
        print("sw_query")
        #print(sw_query)
        global sw_urllist
        sw_urllist = GetUrlList(sw_query)  # 所有sw spec完整链接
        if sw_urllist == []:
            build_model = int("a")
        # Get sw spec
        build_models = GetBuildSpecList("Models")
        print("Model No: %d"%(len(build_models)))
        for i in range(0, len(build_models)):
            print("> %s"%(build_models[i]))

        for i in range(0, len(build_models)):
            build_id = build_models.index(build_models[i])
            # Build SPEC
            b = -1
            build_functionname = GetBuildSpec(b)
            build_function = GetBuildSpec(build_id)
            GetListSwitch(build_function)
            nvs2 = zip(build_functionname, build_function)
            nvDict2 = dict((build_functionname, build_function)
                           for build_functionname, build_function in nvs2)
            build_model = str(build_models[i])  # 对应的build 中的model name
            print("Parse build model: %s"%(build_model))
            for j in range(0, len(sw_queries)):
                query_name = str(sw_queries[j])
                result = str.find(query_name, build_model) != -1
                if result is not False:
                    print('SW Spec: ', query_name)
                    num = sw_queries.index(sw_queries[j])
                    get_SWPath = GetSWSpec(num, build_model)
                    # SW SPEC
                    sw_dict = GetSWSpecList(get_SWPath)

                    # Size
                    #Size(build_swpath, a)
                    result_size = Compare_Size(build_model)
                    # Branch
                    print("branch check\n")
                    enable_branch = GetSWSpecList_EnableBranch(get_SWPath)  # 新增
                    #print(enable_branch)
                    txt_branch = Get_Branch(get_SWPath, branch_Path)
                    #print(txt_branch)
                    diffcc = txt_branch.keys() & enable_branch.keys()
                    #print(diffcc)
                    diff_branch = [(k, txt_branch[k], enable_branch[k]) for k in diffcc if (
                        enable_branch[k] != "" and (txt_branch[k] not in enable_branch[k]))]
                    print('Different Branch：', diff_branch)

                    # 两字典进行相同Key不同value比较   SW  Spen
                    diff = nvDict2.keys() & sw_dict
                    diff_vals = [(k, nvDict2[k], sw_dict[k])
                                 for k in diff if nvDict2[k] != sw_dict[k]]
                    print('Different：', diff_vals)
                    if diff_vals:
                        flag = 1
                    # ============odmpid==============================
                    try:
                        missing_odmpid = GetOdmpidDiff(
                            get_SWPath, build_swpath, build_model)
                    except:
                        print("Get odmpid error")
                        break
                    # ============odmpid==============================

                    Write(build_model, diff_vals, diff_branch,
                          result_size, missing_odmpid)
                    break

                elif (query_name == '標案: RT-AC2600P (Starhub)') and (
                        (str.find('標案: RT-AC2600P (Starhub)', build_model) != -1) is not True):
                    print("无此標案或其他未知")
                    diff_vals = 'Do not find model name in Redmine or unknown issue.'
                    diff_branch = 'NA'
                    result_size = 'NA'

                    # ============odmpid==============================
                    missing_odmpid = "NA"
                    # ============odmpid==============================
                    Write(build_model, diff_vals, diff_branch,
                          result_size, missing_odmpid)
                    break
    except:
        print('未连接网络,即将使用本地资源进行比较!!!')
        build_models = GetBuildSpecList("Models")
        for i in range(0, len(build_models)):
            build_model = build_models[i]
            build_id = build_models.index(build_models[i])
            file_name = sw_path + build_model.split('/')[-1] + '.csv'
            print('SW Spec: ', build_model)
            try:
                sw_dict = GetSWSpecList(file_name)
                none_id = build_id
            except:
                print('!!!!--------未找到与'+build_model+'相关SW Spec')
                none_id = build_id+100
            b = -1
            build_functionname = GetBuildSpec(b)
            if build_id+100 == none_id:
                diff_vals = 'Do not find model name in Redmine or unknown issue.'
                # Write(a, diff_vals)    #new
            else:
                build_function = GetBuildSpec(build_id)
                GetListSwitch(build_function)
                nvs2 = zip(build_functionname, build_function)
                nvDict2 = dict((build_functionname, build_function)
                               for build_functionname, build_function in nvs2)
                # Size   new
                # Size(build_swpath, a)
                result_size = Compare_Size(build_model)
                # Branch
                enable_branch = GetSWSpecList_EnableBranch(file_name)  # 新增
                txt_branch = Get_Branch(file_name, branch_Path)
                diffcc = txt_branch.keys() & enable_branch.keys()
                diff_branch = [(k, txt_branch[k], enable_branch[k]) for k in diffcc if
                               (enable_branch[k] != "" and (txt_branch[k] not in enable_branch[k]))]
                print('Different Branch：', diff_branch)
                ####################
                diff = nvDict2.keys() & sw_dict
                diff_vals = [(k, nvDict2[k], sw_dict[k])
                             for k in diff if nvDict2[k] != sw_dict[k]]
                print('Different：', diff_vals)
                if diff_vals:
                    flag = 1
                # ============odmpid==============================
                try:
                    print("build:",build_models)
                    missing_odmpid = GetOdmpidDiff(
                        file_name, build_swpath, build_model)
                except:
                    print("Get odmpid error")
                # ============odmpid==============================
                Write(build_model, diff_vals, diff_branch,
                      result_size, missing_odmpid)
    return flag

if __name__ == "__main__":
    specparse("","","C:\\eSupport\\test\\SPEC","C:\\eSupport\\test\\SPEC","C:\\eSupport\\test\\SPEC","","")