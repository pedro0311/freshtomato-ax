import base64

def base64Encode(value):
    # if type(value) != str:
    #     print("not valid value")
    #     value = str(value)
    #     byte_info = value.encode('utf-8')
    # else:
    byte_info = value.encode('utf-8')
    return base64.b64encode(byte_info)

def del_value_from_list(data_list, index):
    data_list.pop(int(index))
    return data_list

def del_operate_from_string(value):
    value = value.split("\r\n",1)[1]
    return value

def TR069_Compare_print(operate,str1,str2):
    flag = 0
    if str2=="NULL":
        return 0
    str1_list = str1.split("\r\n")
    str2_list = str2.split("\r\n")
    print(str2_list[0])
    r = [s for s in str1_list if str2_list[0] in s]
    if len(r)<=0:
        flag = 1
    #if str1_list[len(str1_list)-1] != str2_list[len(str2_list)-1]:
    #    flag = 2
    return flag