*** Settings ***
Suite Setup       Tr069_Setup
Suite Teardown    Tr069_Teardomn
Default Tags      TR069
Library           RequestsLibrary
Library           getB64.py
Library           Collections
Library           BuiltIn
Library           SeleniumLibrary
Library           Telnet
Library           String
Library           auto_test.py

*** Variables ***
${USERNAME}       admin
${PASSWD}         admin123
${SERIAL_NO}      000C43264644
${DUT_NAME}       4G-AX56
${DUT_PATH}       192.168.51.1
${OPENACS_PATH}    http://192.168.50.6:8080/openacs/cpe_find.jsf
${GET_OPTION}     sed -n '/GetParameterValuesResponse/,/GetParameterValuesResponse/p' /tmp/tr/tr.log
${ADD_OPTION}     sed -n '/AddObjectResponse/,/AddObjectResponse/p' /tmp/tr/tr.log
${RESET_PATH}     http://router.asus.com/Main_Password.asp
${RESET_PATH_1}    http://router.asus.com/QIS_wizard.htm?flag=welcome/









*** Test Cases ***
Test TR181_to_98
    TR069_Verify    TR181_to_98    X_ASUS_TR_Type    cat /tmp/tr/tr.xml|grep \\\'InternetGatewayDevice\\\'    InternetGatewayDevice    nvram get tr_type    TR098    10

Test set_PPPoE
    TR069_Verify    set_PPPoE    ConnectionType    NULL    NULL    nvram get wan_proto    pppoe    10

Test TR98_to_181
    TR069_Verify    TR98_to_181    X_ASUS_TR_Type    cat /tmp/tr/tr.xml|grep \\\'Device\\\'    Device    nvram get tr_type    TR181    10

Test ADD DART
    TR069_Verify    ADD DART    X_ASUS_TR_Fun    cat /tmp/tr/tr.xml|grep DART    DART    nvram get tr_fun    DART    10

Test Add Object
    [Tags]    TR069
    TR069_Verify    Add Object    ParentControl    ${ADD_OPTION}    \<Status\>0\<\/Status\>    NULL    NULL    10

Test DinnerTime
    [Tags]    TR069
    TR069_Verify    DinnerTime    ParentControlEnable    NULL    NULL    nvram get OPTUS_MULTIFILTER_ALL    1    10

Test DART
    [Tags]    TR069
    TR069_Verify    DART    Device.DART.Enable    NULL    NULL    nvram get tr_dart_enable    1    10

Test Hosts
    [Tags]    TR069
    TR069_verify    Hosts    Device.Hosts.Host    ${GET_OPTION}    *nvram get custom_clientlist|cut -f2 -d '<'|cut -f1 -d '>'    NULL    NULL    10

Test QoS
    [Tags]    TR069
    TR069_verify    QoS    QoS    NULL    NULL    shell_username    test    10

Test ACS ACL
    [Tags]    TR069
    TR069_verify    ACS ACL    AcsSubnets    NULL    NULL    nvram get tr_acs_acl    211.29.152.129/32,211.29.152.131/32,211.29.152.148/32,210.49.120.1    10
    #TR069_telnet    cat /tmp/tr/aaa.txt    *nvram get custom_clientlist|cut -f2 -d '<'|cut -f1 -d '>'
    #TR069_telnet_value    nvram get custom_clientlist|cut -f2 -d '<'|cut -f1 -d '>'

Test reboot
    TR069_reboot

Test upgrade
    TR069_upgrade

Test reset
    TR069_reset

TEST
    Telnet_head

*** Keywords ***
Tr069_Setup
    Openacs_head
    Telnet_head

Base64 Loginfo
    [Arguments]    ${username}    ${passwd}
    ${info}    Catenate    SEPARATOR=:    ${username}    ${passwd}
    ${loginfo}    getB64.Base 64 Encode    ${info}
    RETURN    ${loginfo}

Router Login
    # 更改用户密码
    ${loginfo}    Base64 Loginfo    ${username}    ${passwd}
    &{header}    Create Dictionary    Connection=keep-alive    Cache-Control=max-age=0    Origin=${Dut_path}    Content-Type=application/x-www-form-urlencoded    User-Agent=asusrouter-Windows-DUTUtil-1.0
    Set Global Variable    &{header}
    &{data}    Create Dictionary    group_id=    action_mod=    action_script=    action_wait=5    current_page=Main_login.asp    login_authorization=${loginfo}    login_captcha=
    Create Session    asus    ${Dut_path}
    ${res}    POST On Session    asus    login.cgi    data=&{data}    headers=&{header}
    Set Global Variable    ${res}

JSON_Equal
    [Arguments]    ${json}    ${property}    ${value_expected}
    #判断
    ${value_found}    Get From Dictionary    ${json}    ${property}
    Log    ${value_found}
    ${error_message}=    Catenate    SEPARATOR=    ${property}    expected value is:    ${value_expected}    but it is actually:    ${value_found}
    Should Be Equal As Strings    ${value_found}    ${value_expected}    ${error_message}

TR069_Verify
    [Arguments]    ${testcase}    ${testcase_expected}    ${file}    ${file_expected}    ${nvram}    ${nvram_expected}    ${sleep}
    [Documentation]    执行命令后验证
    Openacs_operate    ${testcase}
    TR069_telnet_value    rm -rf /tmp/tr/tr.log
    Telnet.Write    service restart_tr
    sleep    5
    ${tmp}    Telnet.Read
    sleep    5
    Write    cat /tmp/tr/tr.log |grep SOAP-ENV:Envelope|tail -1
    sleep    10
    ${testcase_accept}    Telnet.Read
    sleep    5
    Openacs_operate    defult
    LOG    ${testcase_accept}
    TR069_Compare    ${testcase}    ${testcase_accept}    ${testcase_expected}
    sleep    ${sleep}
    Run Keyword IF    "${file}"=="NULL"    log    Skip
    ...    ELSE    TR069_telnet    ${file}    ${file_expected}
    Run Keyword IF    "${nvram}"=="NULL"    log    Skip
    ...    ELSE    TR069_telnet    ${nvram}    ${nvram_expected}

TR069_telnet_value
    [Arguments]    ${operate}
    #获取通过telnet操作后获取的值
    Write    ${operate}
    sleep    2
    ${value}    Read
    ${len}    get length    ${operate}
    ${value}=    Run Keyword IF    ${len}>45    del_operate_from_string    ${value}
    ...    ELSE    Set Variable    ${value}
    LOG    ====================================
    LOG    ${value}
    LOG    ====================================
    #sleep    5
    [Return]    ${value}

TR069_telnet
    [Arguments]    ${operate}    ${value_expected}
    #执行要在telnet执上的命令
    Run Keyword IF    "${value_expected}"=="NULL"    LOG    NO compare
    ${value}=    TR069_telnet_value    ${operate}
    ${value2_len}    get length    ${value_expected}
    ${tmp}    Get Substring    ${value_expected}    1    ${value2_len}
    ${value_expected}=    Run Keyword IF    "${value_expected[0]}"=="*"    TR069_telnet_value    ${tmp}
    ...    ELSE    Set Variable    ${value_expected}
    TR069_Compare    ${operate}    ${value}    ${value_expected}
    sleep    2

Openacs_head
    #开启${openacs_path}的网页
    open browser    ${openacs_path}    chrome
    sleep    5
    maximize browser window
    click element    xpath=//select[@name="j_id41:j_id44"]/option[text()="${dut_name}"]
    sleep    2
    input text    xpath=//*[@name="j_id41:cpeSerialNo"]    ${serial_no}
    sleep    2
    click element    xpath=//*[@name="j_id41:j_id48"]
    sleep    2
    click element    xpath=//*[@href="deviceprofile_new.jsf"]
    sleep    2
    click element    xpath=//*[contains(@onclick,'prfname')]
    sleep    2

Telnet_head
    #打开telnet的连接
    Open Connection    ${Dut_path}    prompt_is_regexp=yes    prompt=\#    timeout=15
    Login    ${username}    ${passwd}    login_prompt=${dut_name} login:    password_prompt=Password:
    ${output}    Execute Command    pwd
    Log    ${output}

Openacs_operate
    [Arguments]    ${operate}
    #在openacs上面选择要操作的指令
    Run Keyword IF    "${operate}"=="defult"    click element    xpath=//select[@name="j_id59:scripts"]/option[@value=""]
    ...    ELSE    click element    xpath=//select[@name="j_id59:scripts"]/option[@value="${operate}"]
    sleep    2
    click element    xpath=//*[@name="j_id59:j_id86"]
    sleep    2

TR069_Compare
    [Arguments]    ${operate}    ${value_found}    ${value_expected}
    #判断
    ${error_message}=    Catenate    SEPARATOR=    ${operate}    =>expected value is:    ${value_expected}    ,but it is actually:    ${value_found}
    ${flag}=    TR069_Compare_print    ${operate}    ${value_found}    ${value_expected}
    Run Keyword IF    ${flag}!=0    LOG    ${error_message}    FAIL    Else    Log    "Pass"

TR069_start
    #判断tr069是否正常来连接至openacs
    Openacs_operate    defult
    #TR069_telnet    ps |grep tr0    tr069
    TR069_telnet    rm /tmp/tr/tr.log    NULL
    TR069_telnet    service restart_tr    NULL
    sleep    3
    Write    cat /tmp/tr/tr.log |grep \"Receive an empty package\"
    ${start_1}    Read
    TR069_Compare    Connection test    ${start_1}    Receive an empty package

TR069_reboot
    #判断TR069是否可以使用指令进行重启操作
    Openacs_operate    reboot
    TR069_telnet    service restart_tr    NULL
    sleep    2
    write    ps |grep tr0 |head -1
    sleep    2
    ${PID}    read
    #sleep    2
    write    cat /tmp/tr/tr.log |grep SOAP-ENV:Envelope|tail -1
    sleep    2
    ${tmp}    read
    sleep    5
    Openacs_operate    defult
    TR069_Compare    SOAP-ENV    ${tmp}    Reboot
    Close Connection
    sleep    100
    Telnet_head
    write    ps |grep tr0|head -1
    ${PID_2}    read
    Should not Be Equal As Strings    ${PID}    ${PID_2}    PID no be edual

TR069_reset
    #判断TR069是否可以使用指令进行重启恢复出场设置功能
    TR069_Verify    Reset    Reset    NULL    NULL    NULL    NULL    5
    Close Connection
    sleep    100
    #open browser    ${RESET_PATH}    chrome
    #sleep    5
    #maximize browser window
    #input text    xpath=//*[@name="http_username_x"]    admin
    #sleep    2
    #input text    xpath=//*[@name="http_passwd_x"]    admin123
    #sleep    2
    #input text    xpath=//*[@name="http_passwd_2_x"]    admin123
    #sleep    2
    #click element    xpath=//*[@name="btn_modify"]
    #sleep    2
    #sleep    100

TR069_upgrade
    #判断TR069是否可以使用指令进行更新FW
    TR069_Verify    Upgrade    1 Firmware Upgrade Image    NULL    NULL    NULL    NULL    2
    Close Connection
    sleep    100
    Telnet_head
    TR069_telnet    nvram get extendno    45678-gbaea983

Tr069_Teardomn
    Openacs_operate    defult
    Close Connection
