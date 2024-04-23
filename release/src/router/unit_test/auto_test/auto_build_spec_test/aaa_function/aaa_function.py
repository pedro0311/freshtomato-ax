import base64
import os


def String_sqlit_noendline(str1, str2):
    return str1.split(str2)[0]

def String_sqlit_end(str1, str2):
    return str1.split("\r\n" + str2)[0]


def String_contain(str1, str2: str, str3):
    #Determines whether str1 contains str2.
    print("str1=", str1, ",str2=", str2)
    str2 = str2.split("\r\n" + str3)[0]
    print(str2)
    flag = 0
    if str2 in str1:
        flag = 1
    return flag

def Get_Spec_Folder(path:str,dut_name):
    import time
    time = time.strftime('%Y%m%d%H%M%S', time.localtime())
    path = path + "\\" + dut_name + "_" +time
    try:
        fi_folder = os.path.exists(path)
        if not fi_folder:
            os.makedirs(path)
        se_folder = os.path.exists(path+"\\"+dut_name)
        if not se_folder:
            os.makedirs(path+"\\"+dut_name)
    except Exception as e:
        print("*ERROR*" + str(e))
        raise
    print("path:", path)
    return path

def Get_Other_File(path:str,models,branch_new:str,fw_size:str,dut_name,end):
    fw_size_context = fw_size.split()[0]
    fw_path = path + "\\" + dut_name + "\\fw_size.txt"
    models_path = path + "\\models"
    branch_new_path = path + "\\branch"
    try:
        file = open(fw_path, "w")
        file.write(fw_size_context)
        file.close()
        models_fp = open(models_path, "w")
        models = models[:-len(end)]
        models_fp.write(models)
        models_fp.close()
        branch_new_path_fp = open(branch_new_path, "w")
        branch_new_path_fp.write(branch_new.split()[1])
        branch_new_path_fp.close()
    except Exception as e:
        print("*ERROR*" + str(e))
        raise


def Config_write_File(context,file_name:str,path,end:str,dut_name):
    print("file_name:",file_name,",path:",path)
    file_name_true = file_name.split("/")[-1]
    print(file_name_true)
    context = context[:-len(end)]

    path = path + "\\" + dut_name
    folder = os.path.exists(path)
    if not folder:
        os.makedirs(path)

    try:
        file = open(path+"\\"+file_name_true, "w")
        file.write(context)
        file.close()
    except Exception as e:
        print("*ERROR*" + str(e))
        raise

