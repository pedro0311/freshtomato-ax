*** Settings ***
Suite Setup       NEWOB_Setup
Suite Teardown    NEWOB_Teardown
Library           RequestsLibrary
Library           getB64.py
Library           Collections
Library           BuiltIn
Library           SeleniumLibrary
Library           Telnet
Library           String
Library           ../auto_build_spec_test/aaa_function/aaa_function.py

*** Variables ***
${SUITE_SETUP_LOG}     ==== NewOB Auto Notify test Start! ====
${SUITE_DOWN_LOG}     ==== NewOB Auto Notify test End! ====
${USERNAME_1}       admin
${PASSWD_1}         admin             #newRE's passwd
${USERNAME_2}       admin
${PASSWD_2}         passwd            #CAP's passwd
${CAP_DUT_NAME}       RT-AX53U
${NEWRE_DUT_NAME}       RT-AX53U
${NEWRE_PATH}       192.168.50.1
${DUT_PATH_1}       192.168.51.221    #RE's ip
${DUT_PATH_2}       192.168.51.1      #CAP's ip
${DUT_END}    ${USERNAME_1}@
${CAP_MAC}    04:92:26:88:F5:C0
${NEWEW_MAC}    58:11:22:87:89:38
${RULE_DISMISS}    DISMISS
${RULE_BLOCK}    BLOCK
${NEWOB_LIST_FOLDER}    /jffs/newob/
${NEWOB_LIST_RULE_JSON_PATH}    ${NEWOB_LIST_FOLDER}newob_rule_onboarding.json
${NEWOB_NT_LIST_JSON_PATH}    ${NEWOB_LIST_FOLDER}newob_notify.json
${NEWOB_DISMISS_FOLDER}    /tmp/newob/
${NEWOB_DISMISS_LIST_JSON_PATH}    ${NEWOB_DISMISS_FOLDER}newob_dismiss.json
${IPTABLES_CHECK}    tcp dpt:7780
${RESTORE_DEFAULT}    1

*** Test Cases ***
NEWRE_setup
    Telnet Login    ${USERNAME_1}    ${PASSWD_1}    ${NEWRE_PATH}    60s    120    NULL
    ${fw_name}    Write_Read    Telnet    0    nvram get extendno    ${DUT_END}
    ${re_mode}    Write_Read    Telnet    0    nvram get re_mode    ${DUT_END}
    ${sw_mode}    Write_Read    Telnet    0    nvram get sw_mode    ${DUT_END}
    ${amas_newob_status}    Write_Read    Telnet    0    nvram get amas_newob_status    ${DUT_END}
    Write_Read    Telnet    0    nvram set asuslog_path=/jffs/newob/    ${DUT_END}
    Write_Read    Telnet    0    nvram set cfg_syslog=1    ${DUT_END}
    Write_Read    Telnet    0    nvram commit    ${DUT_END}
    ${flag}    String_contain    ${sw_mode}    1    ${DUT_END}
    IF    ${flag}==1                                                      #router mode check iptables
        Write_wait    Telnet    iptables -nvL INPUT| grep tcp\r\n    180s    10s    ${IPTABLES_CHECK}
    ELSE
        ${flag}    String_contain    ${sw_mode}    2    ${DUT_END}
        IF    ${flag}==1
            Log To Console    sw_mode is repeater mode, cannot do notify newob
            FAIL
        END
    END
    Telnet.Close Connection
    Telnet Login    ${USERNAME_2}    ${PASSWD_2}    ${DUT_PATH_2}    60s    120    1st conn

NEWOB_ACCEPT
    ENABLE_NEWOB_AUTO_NOTIFY
    CHECK_NOTIFY
    Write_wait    Telnet    cat /tmp/newob_onboarding.json\r\n    180s    1s    ${NEWEW_MAC}
    ${data}    Create Dictionary    action_mode=newre_selection    device_list=${NEWEW_MAC}
    WebApply    ${USERNAME_2}    ${PASSWD_2}    ${DUT_PATH_2}    applyapp.cgi    ${data}
    sleep    10
    ${ob_list}    Write_Read    Telnet    0    cat /tmp/newob_onboarding.json    ${DUT_END}
    ${list}    aaa_function.String_sqlit_noendline    ${ob_list}    ${DUT_END}
    ${reboot_time}    GetValueFromJson    ${list}    ${NEWEW_MAC}    reboot_time
    ${conn_timeout}    GetValueFromJson    ${list}    ${NEWEW_MAC}    conn_timeout
    ${traffic_timeout}    GetValueFromJson    ${list}    ${NEWEW_MAC}    traffic_timeout
    Set Global Variable    ${conn_timeout}
    Set Global Variable    ${traffic_timeout}
    ${sleep_t}    Evaluate    ${reboot_time}-30
    sleep    ${sleep_t}

CHECK_RESULT
    ${extendno}    Write_Read    Telnet    0    nvram get extendno    ${DUT_END}
    ${re_mode}    Write_Read    Telnet    0    nvram get re_mode    ${DUT_END}
    ${sw_mode}    Write_Read    Telnet    0    nvram get sw_mode    ${DUT_END}
    ${cfg_group_cap}    Write_Read    Telnet    0    nvram get cfg_group    ${DUT_END}
    Write_wait    Telnet    ps|grep cfg\r\n    30s    1s    cfg_server
    ${x}=    Set Variable    ${0}
    ${timeout}    Evaluate    ${conn_timeout}+${traffic_timeout}
    WHILE    ${x} < ${timeout}
        ${ob_list}    Write_Read    Telnet    0    cat /tmp/newob_onboarding.json    ${DUT_END}
        ${list}    aaa_function.String_sqlit_noendline    ${ob_list}    ${DUT_END}
        ${status}    GetValueFromJson    ${list}    ${NEWEW_MAC}    status
        # log to console    cfg_obresult=${cfg_obresult}
        IF    '${status}'=='9'    #JOIN
            BREAK
        END
        sleep    5
        ${x}    Evaluate    ${x} + 5
    END
    sleep    20
    ${x}=    Set Variable    ${0}
    WHILE    ${x} < 20
        ${ob_list}    Write_Read    Telnet    0    cat /tmp/newob_onboarding.json    ${DUT_END}
        ${list}    aaa_function.String_sqlit_noendline    ${ob_list}    ${DUT_END}
        ${cfg_obresult}    GetValueFromJson    ${list}    ${NEWEW_MAC}    cfg_obresult
        # log to console    cfg_obresult=${cfg_obresult}
        IF    '${cfg_obresult}'=='4'    #PASS
            log to console    newRE(${NEWEW_MAC}) onboarding success(${cfg_obresult})
            BREAK
        ELSEIF    '${cfg_obresult}'=='2'    #connection timeout
            log to console    newRE(${NEWEW_MAC}) onboarding failed(${cfg_obresult}), connection timeout
            BREAK
        ELSEIF    '${cfg_obresult}'=='3'    #traffic timeout
            log to console    newRE(${NEWEW_MAC}) onboarding failed(${cfg_obresult}), traffic timeout
            BREAK
        END
        sleep    5
        ${x}    Evaluate    ${x} + 5
    END
    IF    '${cfg_obresult}'!='4'
        FAIL
    END
    sleep    10
    Write_wait    Telnet    ping -c1 ${DUT_PATH_1}\r\n    60s    5s    1 packets received
    Telnet Login    ${USERNAME_2}    ${PASSWD_2}    ${DUT_PATH_1}    60s    120    2nd conn
    ${extendno}    Write_Read    Telnet    0    nvram get extendno    ${DUT_END}
    ${re_mode}    Write_Read    Telnet    0    nvram get re_mode    ${DUT_END}
    ${flag}    String_contain    ${re_mode}    1    ${DUT_END}
    IF    ${flag}==0
        FAIL
    END
    ${sw_mode}    Write_Read    Telnet    0    nvram get sw_mode    ${DUT_END}
    ${cfg_group_re}    Write_Read    Telnet    0    nvram get cfg_group    ${DUT_END}
    ${flag}    String_contain    ${cfg_group_re}    ${cfg_group_cap}    ${DUT_END}
    IF    ${flag}==0
        FAIL
    END
    ${amas_newob_status}    Write_Read    Telnet    0    nvram get amas_newob_status    ${DUT_END}
    ${flag}    String_contain    ${amas_newob_status}    4    ${DUT_END}
    IF    ${flag}==0
        FAIL
    END
    Write_wait    Telnet    ps|grep cfg\r\n    30s    1s    cfg_client

TEST_END
    IF    ${RESTORE_DEFAULT}==1
        Log To Console    restore default
        ${data}    Create Dictionary    action_mode=reset_default    slave_mac=${NEWEW_MAC}
        WebApply    ${USERNAME_2}    ${PASSWD_2}    ${DUT_PATH_2}    applyapp.cgi    ${data}
    ELSE
        Log To Console    restore to backup setting
        Telnet.Switch Connection    2nd conn
        ${output}    Write_Read    Telnet    0    reload_backup_config    ${DUT_END}
    END
    DISABLE_NEWOB_AUTO_NOTIFY
    sleep    180

*** Keywords ***
CHECK_NOTIFY
    Write_wait    Telnet    cat /tmp/NTC.log\r\n    50s    3s    MSG:[{CAP_MacAddress:${CAP_MAC},CAP_DUT:${CAP_DUT_NAME},RE_MacAddress:${NEWEW_MAC},RE_DUT:${NEWRE_DUT_NAME}}]
    Write_wait    Telnet    cat ${NEWOB_NT_LIST_JSON_PATH}\r\n    10s    3s    ${NEWEW_MAC}

ENABLE_NEWOB_AUTO_NOTIFY
    Write_Read    Telnet    0    rm /jffs/.sys/cfg_mnt/cfg_dbg.log    ${DUT_END}
    Write_Read    Telnet    0    echo {} > /tmp/newob_onboarding.json    ${DUT_END}
    Write_Read    Telnet    0    nvram set aimesh_newob_auto_notify=1    ${DUT_END}
    Write_Read    Telnet    0    echo 1 > /tmp/NTC_DEBUG    ${DUT_END}

DISABLE_NEWOB_AUTO_NOTIFY
    Telnet.Switch Connection    1st conn
    Write_Read    Telnet    0    rm /tmp/NTC_DEBUG    ${DUT_END}
    Write_Read    Telnet    0    rm /tmp/NTC.log    ${DUT_END}
    Write_Read    Telnet    0    nvram unset aimesh_newob_auto_notify    ${DUT_END}

SET_NEWOB_RULE
    [Arguments]    ${username}    ${passwd}    ${path}    ${mac}    ${rule}    ${setup}
    ${data}    Create Dictionary    re_mac=${mac}    rule=${rule}    setup=${setup}
    WebApply    ${username}    ${passwd}    ${path}    set_cfg_newob.cgi    ${data}

Base64 Loginfo
    [Arguments]    ${username}    ${passwd}
    ${info}    Catenate    SEPARATOR=:    ${username}    ${passwd}
    ${loginfo}    getB64.Base 64 Encode    ${info}
    RETURN    ${loginfo}

Router Login
    [Arguments]    ${user_name}    ${user_passwd}    ${DUT_PATH}
    # 更改用户密码
    ${loginfo}    Base64 Loginfo    ${user_name}    ${user_passwd}
    &{header}    Create Dictionary    Connection=keep-alive    Cache-Control=max-age=0    Origin=${DUT_PATH}    Content-Type=application/x-www-form-urlencoded    User-Agent=asusrouter-Windows-DUTUtil-1.0
    Set Global Variable    &{header}
    &{data}    Create Dictionary    group_id=    action_mod=    action_script=    action_wait=5    current_page=Main_login.asp    login_authorization=${loginfo}    login_captcha=
    Create Session    asus    http://${DUT_PATH}
    ${res}    POST On Session    asus    login.cgi    data=&{data}    headers=&{header}
    Set Global Variable    ${res}

Telnet Login
    [Arguments]    ${user_name}    ${user_passwd}    ${path}    ${connection_timeout}    ${timeout}    ${alias}
    IF    '${alias}'=="NULL"
        Telnet.open connection    ${path}    prompt_is_regexp=yes    prompt=\#    timeout=${timeout}    connection_timeout=${connection_timeout}
    ELSE
        Telnet.open connection    ${path}    alias=${alias}    prompt_is_regexp=yes    prompt=\#    timeout=${timeout}    connection_timeout=${connection_timeout}
    END
    Telnet.Login    ${user_name}    ${user_passwd}    login_prompt=login:    password_prompt=Password:

Open Telnet
    [Arguments]    ${user_name}    ${user_passwd}    ${path}
    ${data}    Create Dictionary    action_mode=apply    telnetd_enable=1    rc_service=restart_time;restart_upnp
    WebApply    ${user_name}    ${user_passwd}    ${path}    applyapp.cgi    ${data}
    sleep    5

WebApply
    [Arguments]    ${user_name}    ${user_passwd}    ${path}    ${cgi}    ${data}
    Router Login    ${user_name}    ${user_passwd}    ${path}
    ${res}    Create Session    telnet    http://${path}    cookies=${res.text}
    ${telnet}    POST On Session    telnet    ${cgi}    data=${data}    headers=&{header}
    Log    ${telnet}

NEWOB_Setup
    Log To Console    ${SUITE_SETUP_LOG}
    Open Telnet    ${USERNAME_1}    ${PASSWD_1}    ${NEWRE_PATH}
    Open Telnet    ${USERNAME_2}    ${PASSWD_2}    ${DUT_PATH_2}
    Telnet Login    ${USERNAME_2}    ${PASSWD_2}    ${DUT_PATH_2}    60s    120    1st conn
    Write_Read    Telnet    0    nvram set asuslog_path=/jffs/newob/    ${DUT_END}
    Write_Read    Telnet    0    nvram set cfg_syslog=1    ${DUT_END}
    Write_Read    Telnet    0    nvram commit    ${DUT_END}
    Write_Read    Telnet    0    rm /jffs/.sys/cfg_mnt/cfg_dbg.log    ${DUT_END}
    Write_Read    Telnet    0    rm ${NEWOB_NT_LIST_JSON_PATH}    ${DUT_END}
    Write_Read    Telnet    0    rm /tmp/NTC.log    ${DUT_END}

List files
    [Arguments]    ${path}=.    ${options}=
    Execute command    ls ${options} ${path}

Write_Read
    [Arguments]    ${type}    ${time}    ${option}    ${end}
    IF    '${type}'=="SSH"
        SSHLibrary.write    ${option}
        sleep    ${time}
        IF    '${end}'=="NULL"
            ${requset}    SSHLibrary.Read
        ELSE
            ${requset}    SSHLibrary.Read until    ${end}
        END
    ELSE IF    '${type}'=='Telnet'
        Telnet.Write    ${option}
        sleep    ${time}
        IF    '${end}'=="NULL"
            ${requset}    Telnet.Read
        ELSE
            ${requset}    Telnet.Read until    ${end}
        END
    END
    [Return]    ${requset}

Write_wait
    [Arguments]    ${type}    ${option}    ${timeout}    ${retry_interval}    ${end}
    IF    '${retry_interval}'=="NULL"
        ${retry_interval}=1s
    END
    IF    '${type}'=="SSH"
        SSHLibrary.Set Timeout    ${timeout}
        ${requset}    SSHLibrary.Write Until Expected Output    ${option}    ${end}    ${timeout}    ${retry_interval}
        sleep    0.2
        SSHLibrary.Read
    ELSE IF    '${type}'=='Telnet'
        Telnet.Set Timeout    ${timeout}
        ${requset}    Telnet.Write Until Expected Output    ${option}    ${end}    ${timeout}    ${retry_interval}
        sleep    0.2
        Telnet.Read
    END
    [Return]    ${requset}

GetValueFromJson
    [Arguments]    ${str_list}    ${mac}    ${item}    
    ${json_string}    catenate    ${str_list}
    # log to console    \nOriginal JSON:\n${json_string}
    ${json}    evaluate    json.loads('''${json_string}''')    json
    # log to console    ${json['''${mac}''']} 
    RETURN    ${json['''${mac}''']['''${item}''']}

NEWOB_Teardown
    Log To Console    ${SUITE_DOWN_LOG}
    Telnet.Close All Connections