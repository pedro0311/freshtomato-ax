*** Settings ***
Suite Setup       Test_setup
Test Teardown     Test_recover
Library           RequestsLibrary
Library           getB64.py
Library           Collections
Library           BuiltIn
Library           SeleniumLibrary
Library           Telnet
Library           String
Library           auto_test.py

*** Variables ***
${PATH_COMMON}    D:\\auto_test\\testcase
${DEVICE_0_IP}    192.168.50.1
${DEVICE_0_USER}    admin
${DEVICE_0_PWD}    adminAdmin123@
${DEVICE_1_IP}    192.168.50.121
${DEVICE_1_USER}    admin
${DEVICE_1_PWD}    adminAdmin123@
${COM_ENT_FH}     0






*** Test Cases ***
ob_eth_recover
    testcase    ${PATH_COMMON}\\aimesh\\ob_eth_recover

ob_eth
    testcase    ${PATH_COMMON}\\aimesh\\ob_eth

channel_plan_db
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\channel_plan_db

channel_plan_eap_tb_1
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\channel_plan_eap_tb_1

channel_plan_eap_tb_2
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\channel_plan_eap_tb_2

channel_plan_eap_qb
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\channel_plan_eap_qb

channel_plan_eap_db
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    [Template]    testcase
    ${PATH_COMMON}\\aimesh\\channel_plan_eap_db

channel_plan_manual_eap_qb
    [Documentation]    Channel Plan by manual驗證 for ethernet backhaul mode
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\channel_plan_manual_eap_qb

channel_plan_manual_eap_tb_1
    [Documentation]    Channel Plan by manual驗證 for ethernet backhaul mode
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\channel_plan_manual_eap_tb_1

channel_plan_manual_eap_tb_2
    [Documentation]    Channel Plan by manual驗證 for ethernet backhaul mode
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\channel_plan_manual_eap_tb_2

ent_fh_b1
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\ent_fh_b1

ent_fh_b3
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\ent_fh_b3

ent_fh_b2
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\ent_fh_b2

ent_fh_b4
    [Tags]    AMAS_CHANNEL_PLAN    AMAS_CENTRAL_CONTROL
    testcase    ${PATH_COMMON}\\aimesh\\ent_fh_b4

conn_d_t
    testcase    ${PATH_COMMON}\\aimesh\\conn_d_t

conn_dwb_d_t
    testcase    ${PATH_COMMON}\\aimesh\\conn_dwb_d_t

conn_dwb_d_d
    testcase    ${PATH_COMMON}\\aimesh\\conn_dwb_d_d

nvram_dwb_sync_d_d
    testcase    ${PATH_COMMON}\\aimesh\\nvram_dwb_sync_d_d

nvram_dwb_sync_d_t
    testcase    ${PATH_COMMON}\\aimesh\\nvram_dwb_sync_d_t

nvram_dwb_sync_t_d
    testcase    ${PATH_COMMON}\\aimesh\\nvram_dwb_sync_t_d

nvram_dwb_sync_t_t
    testcase    ${PATH_COMMON}\\aimesh\\nvram_dwb_sync_t_t

nvram_sync_d_d
    testcase    ${PATH_COMMON}\\aimesh\\nvram_sync_d_d

nvram_sync_t_d
    testcase    ${PATH_COMMON}\\aimesh\\nvram_sync_t_d

nvram_sync_t_t
    testcase    ${PATH_COMMON}\\aimesh\\nvram_sync_t_t

nvram_sync_d_t
    testcase    ${PATH_COMMON}\\aimesh\\nvram_sync_d_t

wlc1_chk
    testcase    ${PATH_COMMON}\\aimesh\\wlc1_chk

wlct1
    testcase    ${PATH_COMMON}\\wlct1

wlct2
    testcase    ${PATH_COMMON}\\wlct2

wlct3_dual
    testcase    ${PATH_COMMON}\\wlct3_dual

wlct4_dual
    testcase    ${PATH_COMMON}\\wlct4_dual

wlct3_tri
    testcase    ${PATH_COMMON}\\wlct3_tri

wlct4_tri
    testcase    ${PATH_COMMON}\\wlct4_tri

wlct5_tri
    testcase    ${PATH_COMMON}\\wlct5_tri

wlct6_tri
    testcase    ${PATH_COMMON}\\wlct6_tri

time_schedule_SDN
    testcase    ${PATH_COMMON}\\time_schedule_SDN

wgn_test_setup
    testcase    ${PATH_COMMON}\\wgn_test_setup

wgn_test_1
    testcase    ${PATH_COMMON}\\wgn_test_1

wgn_test_2
    testcase    ${PATH_COMMON}\\wgn_test_2

wgn_test_3
    testcase    ${PATH_COMMON}\\wgn_test_3

wgn_test_4
    testcase    ${PATH_COMMON}\\wgn_test_4

wgn_test_check
    testcase    ${PATH_COMMON}\\wgn_test_check

apg_check_status
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_check_status

apg_create_8
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_create_8

apg_create_16
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_create_16

apg_create_32
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_create_32

apg_remove_8
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_remove_8

apg_remove_16
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_remove_16

apg_remove_32
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_remove_32

apg_test_1
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_test_1

apg_test_2
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_test_2

apg_test_3
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_test_3

apg_test_4
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_test_4

apg_test_5
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\apg_test_5

led_sched_2
    [Tags]    RTCONFIG_LED_SCHED
    testcase    ${PATH_COMMON}\\led_sched_2

led_sched_1
    [Tags]    RTCONFIG_LED_SCHED
    testcase    ${PATH_COMMON}\\led_sched_1

sdn_access_time
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_access_time

sdn_ifcap_test
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_ifcap_test

sdn_port_isolation
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_port_isolation

sdn_time_sched_1
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_time_sched_1

sdn_time_sched_2
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_time_sched_2

sdn_swctl_test_setup
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_swctl_test_setup

sdn_swctl_run-1
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_swctl_run-1

sdn_swctl_run-1
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_swctl_chk-1

sdn_swctl_run-2
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_swctl_run-2

sdn_swctl_run-2
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_swctl_chk-2

sdn_swctl_test_check
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_swctl_test_check

sdn_swctl_test_recover
    [Tags]    MTLANCFG
    testcase    ${PATH_COMMON}\\sdn_swctl_test_recover

sdn_to_wgn_test
    [Tags]    AMAS_WGN
    testcase    ${PATH_COMMON}\\sdn_to_wgn_test

wgn_to_sdn_test
    [Tags]    AMAS_WGN
    testcase    ${PATH_COMMON}\\wgn_to_sdn_test

*** Keywords ***
Test_setup
    LOG    "\nAiMesh Auto test tool v1.3"
    LOG    "[MAIN] Be sure target DuT which enabled telnet function."

testcase
    [Arguments]    ${path}
    #selectTestCaseScript    ${path}
    selectTestCase    ${path}
    load_device_config
    enable_telnet
    telnet_connect_to_devices
    SetupEnviConfig
    sleep    1
    runTestConfig
    ${flag}=    chkResult
    Run Keyword IF    ${flag}!=0    FAIL    Else    Log    "Pass"

Test_recover
    recover_settings
    telnet_exit_devices
