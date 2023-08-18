import csv
import os
import sys
import getopt
import re
import configparser
from tokenize import tabsize



class myconfparser(configparser.ConfigParser):
    def __init__(self, defaults=None):
        configparser.ConfigParser.__init__(self, defaults=None)

    def optionxform(self, optionstr):
        return optionstr


def list_header_rtconfig(spec):
    print("spec:",spec)
    header = []
    for i in range(len(spec)):
        header.append(spec[i][0])
    return header


def list_header(spec):
    header = []
    for i in range(len(spec)):
        header.append(spec[i][1])
    return header


def parse_rtconfig(spec, model, cdir):
    #print("spec:",spec,",model:",model,",cdir:",cdir)
    rtconfig = []
    for i in range(len(spec)):
        parse_str = spec[i][1]
        if(spec[i][1].find("||") == -1):
            or_case = 0
            rtenable = 1
        else:
            or_case = 1
            rtenable = 0
        for rtc in spec[i][1].split():
            if rtc == "&&" or rtc == "||":
                continue
            key = rtc.upper()+'=y'
            if (os.path.isfile(cdir+'/.config')):
                if or_case:
                    if open(cdir+'/.config', 'r').read().find(key) != -1:
                        rtenable = 1
                else:
                    if open(cdir+'/.config', 'r').read().find(key) == -1:
                        rtenable = 0
            else:
                rtenable = 0

        if rtenable:
            rtconfig.append("V")
        else:
            rtconfig.append(" ")
        #print("rtconfig:",rtconfig)
    return rtconfig


def parse_cstruct_tables(table, cdir):
    cstruct = []
    found = False
    if (os.path.isfile(cdir)):
        with open(cdir, 'r') as fin:
            for line in fin:
                if (table in line):
                    found = True
                elif found and ('};' in line):
                    break
                elif found and ('{' in line) and ('}' in line):
                    line = re.sub(r'.*{(.*)}.*', r'\1', line)
                    strip_list = [item.strip() for item in line.split(',')]
                    cstruct.append(strip_list)

    return cstruct


def parse_default(spec, model, cdir):
    # crlf=b'\xe2\x80\xa8'.decode('utf-8')
    crlf = ';'
    default = []
    defaults = []
    tcode = []
    value = ' '
    defaults = parse_cstruct_tables(
        'router_defaults[] = {', cdir+'/defaults.prep')
    tcode = parse_cstruct_tables(
        'tcode_init_nvram_list[] = {', cdir+'/tcode.prep')

    for i in range(len(spec)):
        key = spec[i][0]
        for item in defaults:
            if '"'+key+'"' in item[0]:
                value = item[1].lstrip('"').rstrip('"')
        for item in tcode:
            #TODO: GENERIC or MODEL

            if '"'+key+'"' in item[3]:
                if len(item[2]) == 3:
                    value = item[1].lstrip('"').rstrip('"')
                else:
                    value = value+crlf + \
                        item[2].strip('"').rstrip('"')+":" + \
                        item[4].lstrip('"').rstrip('"')
        default.append(value)
    return(default)


def parse_rcsupport(spec, model, cdir):
    # crlf=b'\xe2\x80\xa8'.decode('utf-8')
    crlf = ';'
    rc_supports_all = []
    rc_supports = ""
    add_tcode = []
    remove_tcode = []

    if (os.path.isfile(cdir+'/init.prep')):
        with open(cdir+'/init.prep', 'r') as fin:
            for line in fin:
                if ('add_rc_support' in line) and ('extern' not in line):
                    rc_supports = rc_supports + " " + \
                        re.sub(
                            r'.*add_rc_support\(\"(.*)\".*', r'\1', line).strip()

    rc_supports = re.sub(r'^ ', r'', rc_supports).split()

    tcode_added = parse_cstruct_tables(
        'tcode_rc_support_list[] = {', cdir+'/tcode.prep')
    tcode_removed = parse_cstruct_tables(
        'tcode_del_rc_support_list[] = {', cdir+'/tcode.prep')

    for i in range(len(spec)):
        key = spec[i][0]
        if key in rc_supports:
            if(key == "update"):
                if open(cdir+'/.config', 'r').read().find('RTCONFIG_HTTPS=y') != -1:
                    rcsupport = "V"
                else:
                    rcsupport = " "
            else:
                rcsupport = "V"

            for j in range(len(tcode_removed)):
                m = tcode_removed[j][0]
                t = tcode_removed[j][1]
                r = tcode_removed[j][2]
                if key not in r:
                    continue
                if (m == "MODEL_GENERIC") or (m == model):
                    if rcsupport == "V":
                        rcsupport += crlf+"except for "
                    rcsupport += "["+t+"]"
        else:
            rcs = []
            for j in range(len(tcode_added)):
                m = tcode_added[j][0]
                t = tcode_added[j][1]
                r = tcode_added[j][2]
                if key not in r:
                    continue

                if (m == "MODEL_GENERIC") or (m == model):
                    rcs.append(t)

            for j in range(len(tcode_removed)):
                m = tcode_removed[j][0]
                t = tcode_removed[j][1]
                r = tcode_removed[j][2]
                if key not in r:
                    continue
                if (m == "MODEL_GENERIC") or (m == model):
                    if t in rcs:
                        rcs.remove(t)

            rcsupport = ""
            for j in range(len(rcs)):
                rcsupport += "["+rcs[j].lstrip('"').rstrip('"')+"]"

            if rcsupport == "":
                rcsupport = " "
        rc_supports_all.append(rcsupport)

    return(rc_supports_all)


def parse_dpisupport(spec, model, cdir):
    dpifeatures = ["tcode", "MALS", "VP", "CC", "adaptive_qos", "traffic_analyzer",
                   "webs_filter", "apps_filter", "web_history", "bandwidth_monitor"]
    dpisupport = []
    table = []
    CF = -1
    Nomodelfound = 1
    #print('parse dpi: '+model)

    fp = open(modelnickname_file, 'r')
    lines = fp.readlines()
    for line in lines:
        if model in line:
            splitstr = line.split('=')
            if len(splitstr) > 1:
                model = splitstr[1].replace('\n', '').replace('\r', '')
                break

    if (os.path.isfile(cdir+'/dpi_support.prep')):
        table = parse_cstruct_tables(
            'struct dpiSupport s_tuple[] =', cdir+'/dpi_support.prep')
        for item in table:
            #print('\n>>> %s ? %s' %(model, item[0]))
            uc_model = model.lower()
            uc_item = item[0].lower()
            #print('\n>>> %s ? %s' %(uc_model, uc_item))
            if model in item[0] or uc_model in uc_item:
                for f in range(len(dpifeatures)):
                    #print('%d: %s -> %s' %(f, dpifeatures[f], item[f+1]))
                    if f > 0:
                        if (f == 6) or (f == 7):
                            if item[f+1] == '1':
                                CF = CF + 1
                            if f == 7:
                                if CF == 1:
                                    dpisupport.append("V")
                                else:
                                    dpisupport.append(" ")
                        else:
                            if item[f+1] == '1':
                                dpisupport.append("V")
                            else:
                                dpisupport.append(" ")
                Nomodelfound = 0
                break

    if Nomodelfound == 1:
        for ii in range(8):
            dpisupport.append(" ")

     #for xx in dpisupport:
     #   print(xx)

    return(dpisupport)


def parse_hwauth(spec, model, cdir):
    hwauth = []

    table = []
    table = parse_cstruct_tables(
        'struct HW_AUTH_T s_hw_auth_tuple[] =', cdir+'/hw_auth.prep')

    chk_model = model
    if "RT-AC68" in chk_model:
        chk_model = "RT-AC68"

    supported = " "
    for item in table:
        if chk_model in item[0]:
            supported = "V"

    hwauth.append(supported)
    # ========================odmpid================================
    supported = '"'

    for item in table:
        if item[0] != '0' and item[0] != 'NA':
            temp = item[0]
            temp = temp.replace('"', '')
            temp = temp.replace(',', '')
            temp = temp.replace(' ', '')
            supported = supported + temp + "\n"
    supported += '"'
    hwauth.append(supported)
    # ========================odmpid================================
    table = []
    table = parse_cstruct_tables(
        'struct AMAS_WHITELIST_T s_amas_whitelist_tuple[] =', cdir+'/hw_auth.prep')

    supported = " "
    for item in table:
        if chk_model in item[0]:
            if "0x0001" in item[1]:
                supported = "[CAP]"
            if "0x0002" in item[1]:
                supported += "[RE]"

    hwauth.append(supported)

    table = []

    table = parse_cstruct_tables(
        'struct TUNNEL_WHITELIST_T s_tunnel_whitelist_tuple[] =', cdir+'/hw_auth.prep')

    supported = " "
    for item in table:
        if chk_model in item[0]:
            if open(cdir+'/.config', 'r').read().find('RTCONFIG_AIHOME_TUNNEL=y') != -1:
                supported = "[AIHOME]"
            if open(cdir+'/.config', 'r').read().find('RTCONFIG_AICLOUD_TUNNEL=y') != -1:
                supported += "[AICLOUD]"

    hwauth.append(supported)
    return(hwauth)


def parse_sizecheck(model, cdir):
    size = []

    if (os.path.isfile(cdir+'/fw_size.txt')):
        f = open(cdir+'/fw_size.txt', 'r')
        text = f.read().replace('\n', '').replace('\r', '')
        text.replace('\n', '')
        size.append(text)
    else:
        size.append('')

    if (os.path.isfile(cdir+'/jffs_requirement.txt')):
        f = open(cdir+'/jffs_requirement.txt', 'r')
        text = f.read().replace('\n', '').replace('\r', '')
        text.replace('\n', '')
        size.append(text)
    else:
        size.append('')

    if (os.path.isfile(cdir+'/nvram_requirement.txt')):
        f = open(cdir+'/nvram_requirement.txt', 'r')
        text = f.read().replace('\n', '').replace('\r', '')
        size.append(text)
    else:
        size.append('')

    return(size)


def dump_specs(specs):
    row = ""
    for i in range(len(specs)):
        if i == 0:
            row += specs[i]
        else:
            if re.fullmatch("\".*\"",specs[i]):
                #print("TEST")
                row += ","+specs[i][1:-1]
            elif re.fullmatch("\".*\n\"",specs[i],flags=re.DOTALL):
                row += "," + specs[i][1:-2] + "\n"
            else:
                row += "," + specs[i]
    #print("row:",row)
    with open(auto_spec_path, "a", encoding='utf-8', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(row.split(","))
    return


def showusage():
    #auto_spec = auto_spec + "speccheck.py -c <conffile> -v <confdir>"
    print('speccheck.py -c <conffile> -v <confdir>')


def speccheck(cfile,vdir):
    #print("cfile:",cfile,",vdir:",vdir,",path_tmp:",path_tmp)
    global auto_spec_path
    auto_spec_path  = vdir + "\\autobuild_speccheck.csv"
    print(auto_spec_path)
    global modelnickname_file
    modelnickname_file = cfile + "\\modelnickname"
    cfile = cfile + "\\speccheck.conf"
    try:
        print(auto_spec_path)
        open(auto_spec_path,"w")
    except Exception as e:
        print("*ERROR*" + str(e))
        raise
    flag = 0
    # config=configparser.ConfigParser()
    config = myconfparser()
    config.read(cfile)

#       list title
    specs = []
    specs.append("Models")
    print("Anne:",config.items('RTCONFIG'))
    specs.extend(list_header_rtconfig(config.items('RTCONFIG')))
    specs.extend(list_header(config.items('DEFAULTS')))
    specs.extend(list_header(config.items('RCSUPPORT')))
    specs.extend(list_header(config.items('DPISUPPORT')))
    specs.extend(list_header(config.items('HWAUTH')))
    specs.extend(list_header(config.items('SIZECHECK')))
    dump_specs(specs)
    for root, dirs, files in os.walk(vdir):
        #print("root:",root,",dirs:",dirs,",file:",files)
        for item in dirs:
            cdir = vdir+"/"+item
            model = item.upper()
            specs = []
            specs.append(model)
            specs.extend(parse_rtconfig(config.items('RTCONFIG'), model, cdir))
            specs.extend(parse_default(config.items('DEFAULTS'), model, cdir))
            specs.extend(parse_rcsupport(
                config.items('RCSUPPORT'), model, cdir))
            specs.extend(parse_dpisupport(
                config.items('DPISUPPORT'), model, cdir))
            specs.extend(parse_hwauth(config.items('HWAUTH'), model, cdir))
            specs.extend(parse_sizecheck(model, cdir))
            dump_specs(specs)
            sister_models = config.items('SISTERS')
            #print("sister:",sister_models)
            for sisters in sister_models:
                if model == sisters[0].upper():
                    for smodel in sisters[1].split():
                        specs = []
                        specs.append(smodel)
                        specs.extend(parse_rtconfig(
                            config.items('RTCONFIG'), smodel, cdir))
                        specs.extend(parse_default(
                            config.items('DEFAULTS'), smodel, cdir))
                        specs.extend(parse_rcsupport(
                            config.items('RCSUPPORT'), smodel, cdir))
                        specs.extend(parse_dpisupport(
                            config.items('DPISUPPORT'), smodel, cdir))
                        specs.extend(parse_hwauth(
                            config.items('HWAUTH'), smodel, cdir))
                        dump_specs(specs)
    return flag

if __name__ == "__main__":
    speccheck("C:\\eSupport\\test\\SPEC" ,"C:\\eSupport\\test\\SPEC\\RT-AX53U_20230612133718")
