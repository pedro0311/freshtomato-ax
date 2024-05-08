*** Settings ***
Suite Setup    Connect_DUT
Suite Teardown    Close_DUT
Library           String
Library           Collections
Library           Telnet
Library           function.py
Library           SeleniumLibrary
Library           BuiltIn
Resource          Get_IP_FILE.resource
Library           OperatingSystem
Library           Process
*** Test Cases ***
#將DUT上的CAP與RE相關的文件復製到本地，請在關於Band的功能發生變化時使用
Get_All_File
    Get_File_Dut_To_Local   1   NULL
#關於DUT做CAP的測試
CAP_Setup   #復製所有本地文件到DUT，若已手動復製請勿執行該test
    Copy_File_Local_To_Dut     ${CAP_PATH}    CAP     1     ALL
CAP_ALL_TEST    #運行/jffs/CAP文件夾下所有的testcase
    Bandindex   CAP     1       ALL
CAP_ALL_EBMode_TEST     #運行/jffs/CAP文件夾下所有的testcase,模式為EBMode
    Bandindex   CAP     0       ALL
CAP_Individual_Setup    #復製指定的某個band文件夾到DUT，建議debug時使用
    Copy_File_Local_To_Dut     ${CAP_PATH}    CAP     1     ${BAND_NAME}
CAP_Individual_TEST     #運行指定的某個band文件夾下的testcase
    Bandindex   CAP     1       ${BAND_NAME}
CAP_Individual_EBMode_TEST      #運行指定的某個band文件夾下的testcase,模式為EBMode
    Bandindex   CAP     0       ${BAND_NAME}
#關於DUT做RE的測試，以下testcase釋義同上
RE_Setup
    Copy_File_Local_To_Dut     ${RE_PATH}    RE     1     ALL
RE_ALL_TEST
    Bandindex   RE     1       ALL
RE_ALL_EBMode_TEST
    Bandindex   RE     0       ALL
RE_Individual_TEST
    Bandindex   RE     1       ${BAND_NAME}
RE_Individual_EBMode_TEST
    Bandindex   RE     0       ${BAND_NAME}
DEMO
    Get_RE_Json_Standby
*** Keywords ***
Connect_DUT
    open connection    ${IP}    prompt_is_regexp=yes    prompt=\#    timeout=10
    login    ${NAME}    ${PWD}
Close_DUT
    Close Connection

#測試CAP的所有測項
Bandindex
    [Arguments]    ${type}      ${dwb_flag}     ${band}
    Write_Read    1     cd /jffs/     ${DUT_END}
    #Write_Read    1     rm -rf  /jffs/amas_bandindex_test      ${DUT_END}
    # Write_Read    1     wget ${BANDINDEX_DOWN_PATH}      ${DUT_END}
    Write_Read    1     chmod -R 777 amas_bandindex_test    ${DUT_END}
    Log To Console    ${type}
    Dwb_Mode    ${dwb_flag}
    #修改MAC相關的文件名
    ${all_folders}      Create List
    ${arguments}    Create List
    IF    "${band}" == "ALL"
        ${all_folders_original}=    Write_Read    5     ls -a ./${type} --color=none   ${DUT_END}
        ${all_folders}=     Handle_Files_Original     ${all_folders_original}     ${DUT_END}
    ELSE
        #${arguments}=    Get_Arg    ${band}
        #${len}=     Get Length    ${arguments}
        Append To List    ${all_folders}     ${band}
    END
    FOR    ${folder}    IN      @{all_folders}
        IF    "${type}" == "RE"
            Get_RE_Json     ./${type}/${folder}
        END
        ${all_files_original}=     Write_Read    5      ls -a ./${type}/${folder} --color=none   ${DUT_END}
        ${all_files}=    Handle_Files_Original     ${all_files_original}     ${DUT_END}
        ${change_files}=    Change_File    ${all_files}
        ${rule_flag}        Set Variable      0
        FOR    ${change_file}    IN     @{change_files}
            ${file_src}=     Set Variable    ${change_file.src}
            ${file_dst}=    Set Variable    ${change_file.det}
            Write_Read    5     mv /jffs/${type}/${folder}/${file_src} /jffs/${type}/${folder}/${file_dst}    ${DUT_END}
            IF     ".rule" in "${file_dst}"
                Write_Read    5     cp -rf CAP/${folder}/${file_dst} .sys/cfg_mnt/${file_dst}      ${DUT_END}
                ${rule_flag}     Set Variable      1
            END
        END
        FOR    ${all_file}      IN      @{all_files}
            IF     ${dwb_flag}==0
                IF      "cap_result.json" in "${all_file}" or "cfg.json" in "${all_file}"
                    Write_Read    1     cd ./${type}/${folder}      ${DUT_END}
                    Write_Read    5     sed -i 's/\"dwb_mode\":\"1\"/\"dwb_mode\":\"0\"/g' ./${all_file}     ${DUT_END}
                    Write_Read    1     cd /jffs/      ${DUT_END}
                END
            ELSE IF     ${dwb_flag}==1
                IF      "cap_result.json" in "${all_file}" or "cfg.json" in "${all_file}"
                    Write_Read    1     cd ./${type}/${folder}      ${DUT_END}
                    Write_Read    5     sed -i 's/\"dwb_mode\":\"0\"/\"dwb_mode\":\"1\"/g' ./${all_file}     ${DUT_END}
                    Write_Read    1     cd /jffs/      ${DUT_END}
                END
            END
            IF     ${rule_flag}==0 and ".rule" in "${all_file}"
                Write_Read    5     cp -rf CAP/${folder}/${all_file} .sys/cfg_mnt/${all_file}      ${DUT_END}
            END
        END

        ${argument}     Get_Arg     ${folder}    ${type}
        IF      "${type}" == "RE"
            Write_Read    5       amas_status&       ${DUT_END}
            Sleep    10
        END
        Log To Console    Arg:${argument.arg0},${argument.arg1},${argument.arg2},${argument.arg3},${argument.arg4}
        ${result}=    Write_Read   20   /jffs/amas_bandindex_test -t\"cm_bandindex_test -v ${argument.arg0},${argument.arg1},${argument.arg2},${argument.arg3},${argument.arg4}\" –f /tmp/amas_re_sync.log     ${DUT_END}
        ${flag}    Run Keyword And Return Status    Should Contain    ${result}    cm_bandindex_test: OK
        Log    ${result}
        IF     ${flag}==0
            Log To Console    FAIL,Try again...
            ${result}=    Write_Read   20   /jffs/amas_bandindex_test -t\"cm_bandindex_test -v ${argument.arg0},${argument.arg1},${argument.arg2},${argument.arg3},${argument.arg4}\" –f /tmp/amas_re_sync.log     ${DUT_END}
            ${flag}    Run Keyword And Return Status    Should Contain    ${result}    cm_bandindex_test: OK
            Log    ${result}
            IF     ${flag}==0
                FAIL    Arg:${argument.arg0},${argument.arg1},${argument.arg2},${argument.arg3},${argument.arg4}
            ELSE
                Log To Console    Arg:${argument.arg0},${argument.arg1},${argument.arg2},${argument.arg3},${argument.arg4}: Pass!
            END
        ELSE
            Log To Console    Arg:${argument.arg0},${argument.arg1},${argument.arg2},${argument.arg3},${argument.arg4}: Pass!
        END
    END
    #刪除文件
    #Delete_Dut_File     ${all_files}
    #Close Connection

#修改CAP的smart設置
CAP_SmartConnect
    Run    webdrivermanager chrome --linkpath ./chromedriver --browser_version {desired_version} --webdriver chrome --chromedriver_url_base https://npm.taobao.org/mirrors/chromedriver/
    #open browser    http://${IP}    chrome
    ${options}=    Evaluate    sys.modules['selenium.webdriver'].ChromeOptions()    sys, selenium.webdriver
    Call Method    ${options}    add_argument    --headless
    Call Method    ${options}    add_argument    --disable-gpu
    #open browser        chrome       executable_path=./chromedriver/chromedriver.exe
    open browser    http://${IP}    chrome       options=${options}
    sleep    2
    input text    xpath=//*[@name="login_username"]    ${name}
    Sleep    2
    input text    xpath=//*[@name="login_passwd"]    ${pwd}
    Sleep    2
    click element    xpath=//div[@id="button" or contains(@class, "button")]
    Sleep    2
    click element    xpath=//div[@id="Advanced_Wireless_Content_menu"]
    Open Connection    ${IP}    prompt_is_regexp=yes    prompt=\#    timeout=10
    login    ${NAME}    ${PWD}

    ${smart_flag}=    Write_Read    1     nvram get smart_connect_x    ${DUT_END}
    ${flag}    Get Substring   ${smart_flag}    0    1
    Log    ${flag}
    IF    ${flag}==0
        #click element    xpath=//div[@id="smartcon_enable_block"]
        Write_Read    1     nvram set smart_connect_x=1    ${DUT_END}
        Write_Read    1     nvram commit    ${DUT_END}
        Sleep    10
        Execute JavaScript    location.reload();
        Sleep    10
    END
    @{input_elements}=    Get WebElements    xpath=${SMART_CONNECT_INPUT}
    ${input_flag}=   Get Length    ${input_elements}
    IF    ${input_flag}==0
        #下拉框选择
        @{option_elements}=    Get WebElements    xpath=${SMART_CONNECT_OPTION}
        ${option_len}      Get Length     ${option_elements}
        #获取下拉框的value
        ${option_values}     Create List
        FOR    ${option_element}    IN      @{option_elements}
            ${option_value}      Get Element Attribute      ${option_element}      value
            Append To List    ${option_values}    ${option_value}
        END
        #遍历下拉框
        IF    ${option_len}>1
            FOR    ${option_value}    IN     @{option_values}
                ${option_element}    Get WebElement     xpath=${SMART_CONNECT_OPTION}\[@value=\"${option_value}\"\]
                Click Element    ${option_element}
                ${option_text}      Get Text    ${option_element}
                #IF    'Tri' in '${option_text}' and '6 GHz' in '${option_text}'
                #END
                Log    ${option_text}
                click element    xpath=//input[@id="applyButton"]
                Sleep   ${HANDLE_TIME}
                Execute Javascript    location.reload(true)
                Sleep    2
            END
        END
    ELSE
        #多选框的操作
        ${input_len}      Get Length    ${input_elements}
        IF    ${input_len}>2
            #取消的选项列表
            ${cancel_combinations}=    Create List
            #全部选项的id
            ${input_ids}     Create List
            #全部选项的内容
            ${td_text}       Get Text     xpath=//tr[@id="smart_connect_band_field"]//td
            #获取元素的id
            FOR    ${tmp}       IN      @{input_elements}
                ${input_id}      Get Element Attribute      ${tmp}      id
                Append To List    ${input_ids}      ${input_id}
            END
            #对要取消选择的元素进行组合成列表数组
            FOR    ${index}     IN RANGE      1     ${input_len}-1
                ${cancel_combination}    Combination     ${input_ids}     ${index}
                FOR    ${tmp}     IN     @{cancel_combination}
                    Append To List      ${cancel_combinations}      ${tmp}
                END
            END
            #对多选选项进行选择
            FOR    ${combination}    IN    @{cancel_combinations}
                @{input_elements}=    Get WebElements    xpath=${SMART_CONNECT_INPUT}
                #将选项全选
                FOR    ${input_element}     IN      @{input_elements}
                    Select Checkbox    ${input_element}
                END
                Sleep    2
                #取消部分选项
                Log    ${combination}
                #打印出选择了哪些选项
                ${log_test}=     Log_uncheckout_text    ${combination}     ${td_text}
                Log    ${log_test}
                FOR    ${uncheckout}    IN     @{combination}
                    Log    ${uncheckout}
                    ${uncheckout}      Get WebElement       xpath=${SMART_CONNECT_INPUT}\[@id=\"${uncheckout}\"\]
                    Unselect Checkbox       ${uncheckout}
                END
                Sleep   2
                click element    xpath=//input[@id="applyButton"]
                Sleep   ${HANDLE_TIME}
                #连接CAP
                Get_File_Dut_To_Local   0   ${log_test}
            END
        END
    END

    Close Connection
#將本地文件復製到DUT上
Copy_File_Local_To_Dut
    [Arguments]    ${path}     ${type}      ${dwb_flag}     ${band}
    ${folders}    Create List
    IF    "${band}" == "ALL"
        ${folders}=      Get_Folder      ${path}
    ELSE
        Append To List    ${folders}     ${band}
    END
    Log To Console    ${folders}
    FOR    ${folder}   IN     @{folders}
        Write_Read    1     mkdir /jffs/${type}    ${DUT_END}
        Write_Read    1     mkdir /jffs/${type}/${folder}    ${DUT_END}
        IF      "${type}" == "RE"
            Write_Read    1     mkdir /jffs/${type}/${folder}/amas      ${DUT_END}
        END
        ${files}=      Get_Files     ${path}\\${folder}
        Log To Console    ${files}
        ${json_super}    Set Variable    0
        FOR    ${file}      IN      @{files}
            ${file_text}=   get_file_text     ${path}\\${folder}\\${file}
            Write_Section       ${file_text}     /jffs/${type}/${folder}/${file}
            IF     ${dwb_flag}==0 and "_result.json" in "${file}"
                Write_Read    5     sed -i 's/\"dwb_mode\":\"1\"/\"dwb_mode\":\"0\"/g' ${file}     ${DUT_END}
            END
        END
    END
#分段寫入文件
Write_Section
    [Arguments]    ${file_text}     ${path}
    ${file_text_len}=    Get Length    ${file_text}
    # 分段寫入設備
    Log To Console    ${path}
    Write_Read    1     rm -rf ${path}    ${DUT_END}
    FOR    ${i}    IN RANGE    0    ${file_text_len}    ${chunk size}
        ${chunk}=    Get Substring    ${file_text}    ${i}    ${i+${CHUNK_SIZE}}
        ${chunk}=    Replace String    ${chunk}    "'"    "'\\''"
        Log    ${chunk}
        Write_Read    10     echo -n '${chunk}' >> ${path}    ${DUT_END}
    END
#刪除運行amas_bandindex測試時復製的文件
Delete_Dut_File
    [Arguments]    ${files}
    FOR    ${file}     IN     @{files}
        Write_Read    2     rm -rf /tmp/${file}     ${DUT_END}
    END

#從CAP與RE獲取文件以便後續使用
Get_File_Dut_To_Local
    [Arguments]    ${json_flag}     ${cfg_name}
    open connection    ${IP}    prompt_is_regexp=yes    prompt=\#    timeout=10
    login    ${NAME}    ${PWD}
    #Get_File    ${CAP_PATTERN}      ${CAP_PATTERN_PY}
    #${RE_IP_str}=      Write_Read    1     nvram get cfg_device_list       ${CAP_END}
    ${RES}=     Set_RE_IP   ${IP}   ${NAME}   ${RE_IP}   ${RE_NAME}
    ${CAP_LOG}      Set Variable    0
    ${RE_LEN}       Get Length    ${RES}
    IF    ${RE_LEN}<=1
        Log To Console    AiMesh无RE
        #FAIL
    ELSE
        FOR    ${RE}      IN     @{RES}
            IF     ${CAP_LOG}==0
                ${CAP_LOG}   Set Variable    1
                Continue For Loop
            END
            ${re_tmp_name}     Set Variable    ${RE.name}
            ${re_tmp_ip}   Set Variable    ${RE.ip}
            #获取CAP内的文件
            IF   ${json_flag}==1
                Get_File_Json    ${LOG_PATH}\\CAP\\${re_tmp_name}     ${CAP_PATTERN}      ${CAP_PATTERN_PY}    ${DUT_END}
            END
            Write_Read    1     nvram set cfg_syslog=1      ${DUT_END}
            Write_Read    1     nvram commit      ${DUT_END}
            Sleep    10
            Write_Read    1     killall cfg_server&& cfg_server&    ${DUT_END}
            Write_Read    2     rm -rf /tmp/asusdebuglog/cfg_mnt.log*      ${DUT_END}
            Sleep    10


            #获取RE内的文件
            open connection    ${re_tmp_ip}    prompt_is_regexp=yes    prompt=\#    timeout=10
            login    ${NAME}    ${PWD}
            #Get_RE_Json_Standby
            ${RE_END}    Set Variable    ${NAME}@${re_tmp_name}
            IF    ${json_flag}==1
                Get_File_Json    ${LOG_PATH}\\RE\\${re_tmp_name}     ${RE_PATTERN}      ${RE_PATTERN_PY}     ${RE_END}
            END
            Write_Read    1     nvram set cfg_syslog=1      ${RE_END}
            Write_Read    1     nvram commit      ${RE_END}
            Sleep    10
            Write_Read    5     cd /tmp/asusdebuglog      ${RE_END}
            ${cfg_cap_result}=    Write_Read    5     cat cfg_mnt.log|grep cfg_all|grep decodeMsg    ${RE_END}
            ${cfg_cap_inRoot}=    Write_Read    5     cat cfg_mnt.log|grep cfg_ver|grep cm_prepareCheckMsg      ${RE_END}
            ${cfg_inroot_len}       Get Length    ${cfg_cap_inRoot}
            ${cfg_cap_len}=     Get Length    ${cfg_cap_result}
            ${re_end_len}=      Get Length    ${RE_END}
            Log To Console    ${cfg_cap_result}
            IF    ${cfg_inroot_len}<=${re_end_len}+5
                Write_Read    5    killall cfg_client && cfg_client&     ${RE_END}
                Write_Read    5    rm -rf /tmp/asusdebuglog/cfg_mnt.log*    ${RE_END}
                Sleep    10
                Log To Console    sleep
                ${cfg_cap_inRoot}=    Write_Read    5     cat cfg_mnt.log|grep cfg_ver|grep cm_prepareCheckMsg    ${RE_END}
                ${cfg_inroot_len}=     Get Length    ${cfg_cap_inRoot}
                IF    ${cfg_inroot_len}<=${re_end_len}+5
                    Sleep    20
                    ${cfg_cap_inRoot}=    Write_Read    5     cat cfg_mnt.log-1|grep cfg_ver|grep cm_prepareCheckMsg    ${RE_END}
                END
            END
            IF    ${cfg_cap_len}<=${re_end_len}+5
                Write_Read    5    killall cfg_client && cfg_client&     ${RE_END}
                Write_Read    5    rm -rf /tmp/asusdebuglog/cfg_mnt.log*    ${RE_END}
                Sleep    10
                Log To Console    sleep
                ${cfg_cap_result}=    Write_Read    5     cat cfg_mnt.log|grep cfg_all|grep decodeMsg    ${RE_END}
                ${cfg_cap_len}=     Get Length    ${cfg_cap_result}
                IF    ${cfg_cap_len}<=${re_end_len}+5
                    Sleep    20
                    ${cfg_cap_result}=    Write_Read    5     cat cfg_mnt.log-1|grep cfg_all|grep decodeMsg    ${RE_END}
                END
            END
            Log    ${cfg_cap_result}
            IF     ${json_flag}==1
                ${inroot_flag}     Get_Result_File     ${cfg_cap_inRoot}      ${LOG_PATH}\\CAP\\${re_tmp_name}\\cfg_cap_inRoot_dwb.json
                ${result_flag}=    Get_Result_File     ${cfg_cap_result}      ${LOG_PATH}\\CAP\\${re_tmp_name}\\cfg_cap_result_dwb.json
            ELSE
                ${inroot_flag}     Get_Result_File     ${cfg_cap_inRoot}      ${LOG_PATH}\\CAP\\${re_tmp_name}\\cfg_cap_inRoot${cfg_name}_dwb.json
                ${result_flag}=    Get_Result_File     ${cfg_cap_result}      ${LOG_PATH}\\CAP\\${re_tmp_name}\\cfg_cap_result${cfg_name}_dwb.json
            END
            IF    ${inroot_flag}==0
                FAIL    cfg_cap_inRoot.json出现错误
            END
            IF  ${result_flag}==0
                FAIL    cfg_cap_result.json出现错误,建议使用手动进行获取
            ELSE
                Copy_Files_Fun   ${LOG_PATH}\\CAP\\${re_tmp_name}\\cfg_cap_result.json    ${LOG_PATH}\\RE\\${re_tmp_name}\\cfg.json
                #Get_RE_Json    ${LOG_PATH}\\RE\\${re_tmp_name}\\cfg_re_result.json
            END
            Close connection
        END
    END
    Telnet.Close connection

#开启和关闭DWB功能
Dwb_Mode
    [Arguments]    ${flag}
    Write_Read    1     nvram set dwb_mode=${flag}    ${DUT_END}
    Write_Read    1     nvram commit    ${DUT_END}
#使用正则 获取json文件
Get_File_Json
    [Arguments]    ${file_path}     ${pattern}    ${py_pattern}      ${end}
    ${ip_file}=     Write_Read      2     ls -a /tmp | grep ${pattern}    ${end}
    LOG     ${ip_file}
    @{ip_files}=     get_file_name    ${ip_file}    ${py_pattern}
    Log    ${ip_files}
    FOR     ${ip_file}      IN      @{ip_files}
        IF    "${ip_file}" == "amas"
            ${amas_files_original}=     Write_Read    2     ls -a /tmp/amas/ --color=none   ${end}
            ${amas_files}=     Handle_Files_Original    ${amas_files_original}     ${end}
            FOR    ${amas_file}     IN      @{amas_files}
                ${amas_file_con}=    Write_Read     2       cat /tmp/amas/${amas_file}      ${end}
                Copy_File_Fun    ${file_path}      ${ip_file}/${amas_file}      ${amas_file_con}     ${end}
            END
            Continue For Loop
        END
        IF    "${ip_file}" == "cfg_mnt"
            Continue For Loop
        END
        ${file_con}=      Write_Read    2     cat /tmp/${ip_file}     ${end}
        Copy_File_Fun       ${file_path}     ${ip_file}      ${file_con}     ${end}
    END
Write_Read
    [Arguments]     ${time}    ${option}    ${end}
    #Log To Console    ${option}
    Telnet.Write    ${option}
    sleep    ${time}
    IF    '${end}'=="NULL"
        ${requset}    Telnet.Read
    ELSE
        ${requset}    Telnet.Read until     ${end}
    END
    [Return]    ${requset}
Get_RE_Json
    [Arguments]    ${path}
    ${band_original}     Write_Read    5     nvram show|grep _nband      ${DUT_END}
    ${re_bands}=     Get_RE_Band     ${band_original}     ${DUT_END}
    #获取文件中的band对应
    ${cap_band_list}    Create List
    ${cap_key_origial}=     Write_Read    2     grep -oE \"SUBFT_BASIC_BAND_.{4}\" ${path}/cfg.json|awk \'{print substr(\$0,21,1)}\'      ${DUT_END}
    ${cap_keys}     Split String    ${cap_key_origial}
    ${band6_flag}    Set Variable    0
    FOR     ${cap_key}     IN      @{cap_keys}
        ${cap_key_len}    Get Length    ${cap_key}
        IF    ${cap_key_len} == 1
            ${tmp}      Set Variable    ${cap_key}[0]
            ${is_numeric}    Evaluate    '${tmp}'.isdigit()
            IF    ${is_numeric} == 1
                Run Keyword If    '${tmp}'=='0'    Append To List    ${cap_band_list}     2G
                Run Keyword If    '${tmp}'=='1'    Append To List    ${cap_band_list}     5G
                Run Keyword If    '${tmp}'=='2'    Append To List    ${cap_band_list}     5GL
                Run Keyword If    '${tmp}'=='3'    Append To List    ${cap_band_list}     5GH
                IF     '${tmp}'=='4' and ${band6_flag}==0
                    Append To List      ${cap_band_list}    6GL
                    ${band6_flag}=    Evaluate       ${band6_flag}+1
                ELSE IF     '${tmp}'=='4' and ${band6_flag}==1
                    Append To List      ${cap_band_list}    6GH
                    ${band6_flag}=    Evaluate       ${band6_flag}+1
                END
                Log    ${cap_band_list}
            END
        END
    END
    IF     ${band6_flag}==1
        ${index}    Get Index From List    ${cap_band_list}    6GL
        Set List Value    ${cap_band_list}    ${index}    6G
    END
    Log    ${cap_band_list}
    ${cap_band_len}     Get Length    ${cap_band_list}
    ${re_band_len}      Get Length    ${re_bands}
    IF    ${cap_band_len}<=0
        Fail    Please check this address:${path}
    END

    Log To Console    The BAND of this CAP is:${cap_band_list}
    ${json_txt}=     Set Variable    {\"wireless\"\:\{
    ${index}    Set Variable    0
    FOR    ${cap_band}      IN      @{cap_band_list}
        IF    ${index} >= ${re_band_len}
            BREAK
        END
        ${value}=    Set Variable     ${CAP_RE_VALUE['${cap_band}']}
        ${re_index}     Set Variable    0
        FOR     ${re_band}      IN      @{re_bands}
            ${re_band_name}     Set Variable    ${re_band.value}
            IF    "${cap_band}" == "${re_band_name}"
                ${json_txt}     Set Variable    ${json_txt}\"wlc${re_index}_ssid\":\"${value}\",
                BREAK
            END
            ${re_index}     Evaluate    ${re_index}+1
        END
        IF    ${index}==1 and ${cap_band_len} <=2
            ${json_txt}     Set Variable    ${json_txt}\"wlc2_ssid\":\"${value}\",
        END
        ${index}=    Evaluate       ${index}+1
    END


    ${index}    Set Variable    0
    FOR    ${re_band}     IN     @{re_bands}
        ${key}=     Set Variable    ${re_band.key}
        ${tmp}=     Set Variable    ${re_band.value}
        ${value}    Set Variable     ${CAP_RE_VALUE['${tmp}']}
        ${discrepancy}      Set Variable    0
        Log To Console    tmp:${tmp}
        IF    "${tmp}" in @{cap_band_list}
            ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":"${value}\",
        ELSE
            Log     tmp:${tmp}
            ${value_5g}     Set Variable
            IF    "5G" in @{cap_band_list}
                ${value_5g}    Set Variable    admin_5G
            ELSE IF     "5GL" in @{cap_band_list}
                ${value_5g}    Set Variable    admin_5G-1
            END
            IF    "5G" == "${tmp}" or "5GL" in "${tmp}"
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"${value_5g}\",
            ELSE IF    "5GH" == "${tmp}" and "5G" in @{cap_band_list}
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"admin_5G_dwb\",
            ELSE IF    "6G" == "${tmp}" and "6GL" not in @{cap_band_list}
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"${value_5g}_6G_dwb\",
            ELSE IF     "6G" == "${tmp}" and "6GL" in @{cap_band_list}
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"admin_6G-1\",
            ELSE IF     "6GL" == "${tmp}" and "6G" not in @{cap_band_list}
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"${value_5g}_6G_dwb\",
            ELSE IF     "6GL" == "${tmp}" and "6G" in @{cap_band_list}
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"admin_6G\",
            ELSE IF    "6GH" == "${tmp}" and "6G" not in @{cap_band_list}
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"${value_5g}_6G\",
            ELSE IF    "6GH" == "${tmp}" and "6G" in @{cap_band_list}
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"admin_6G\",
            ELSE
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_ssid\":\"${value}_dwb\",
            END
        END
        ${key_len}      Get Length    ${CAP_RE_OTHER_KEY}
        FOR     ${i}    IN RANGE    ${key_len}
            ${other_key}    Set Variable    ${CAP_RE_OTHER_KEY}[${i}]
            ${other_value}      Set Variable    ${CAP_RE_OTHER_VALUE}[${i}]
            IF    "${other_key}" == "closed" and "${tmp}" == "5GH" and "5GH" not in "${cap_band_list}"
                ${json_txt}     Set Variable    ${json_txt}\"wlc${key}_${other_key}\":\"0\",
                ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_${other_key}\":\"1\",
                Continue For Loop
            END
            ${json_txt}     Set Variable    ${json_txt}\"wlc${key}_${other_key}\":\"${other_value}\",
            ${json_txt}     Set Variable    ${json_txt}\"wl${key}.1_${other_key}\":\"${other_value}\",
        END
    END
    ${json_txt}     Get Substring      ${json_txt}      0     -1
    ${json_txt}     Set Variable    ${json_txt}}}
    #Log To Console    ${json_txt}
    Write_Section    ${json_txt}    ${path}/cfg_re_result.json
    Sleep    10
    #Append To File    ${path}     ${json_txt}

Get_RE_Json_Standby
    ${band_num_orial}=      Write_Read    1     nvram get band_type    ${DUT_END}
    ${band_num}=    get_write_text    ${band_num_orial}      ${DUT_END}
    #${number}    Convert To Number    ${band_num}
    ${number}    Convert To Integer     ${band_num}
    ${json_text}     Set Variable    {\"wireless\":{
    FOR    ${band}     IN RANGE      ${number}
        ${result_origial}=   Write_Read    1     nvram get wlc${band}_ssid      ${DUT_END}
        ${result}=      get_write_text      ${result_origial}       ${DUT_END}
        ${json_text}=    Set Variable    ${json_text}\"wlc${band}_ssid\":\"${result}\",
        ${result_origial}=   Write_Read    1     nvram get wl${band}\.1_ssid      ${DUT_END}
        ${result}=      get_write_text      ${result_origial}       ${DUT_END}
        ${json_text}=    Set Variable    ${json_text}\"wl${band}\.1_ssid\":\"${result}\",
        FOR    ${type}     IN       @{CAP_RE_OTHER_KEY}
            ${result_origial}=   Write_Read    1     nvram get wlc${band}_${type}      ${DUT_END}
            ${result}=      get_write_text      ${result_origial}       ${DUT_END}
            ${json_text}=    Set Variable    ${json_text}\"wlc${band}_${type}\":\"${result}\",
        END
    END
    ${json_text}     Get Substring      ${json_text}      0     -1
    ${json_text}     Set Variable    ${json_text}}}
    Log To Console    ${json_text}
    Write_Section       ${json_text}     ${LOG_PATH}cfg_re_result.json