#!/bin/sh
export tcs="\
get_wlc_status -v 0 -c 3 -w 1;\
get_wlc_status -v 1 -c 3 -w 1;\
get_wlc_status -v 2 -c 3 -w 1;\
stop_wlc_connect -v 0 -c 1 -w 1;\
start_wlc_connect -v 0 -c 1 -w 1;\
stop_wlc_connect -v 1 -c 1 -w 1;\
start_wlc_connect -v 1 -c 1 -w 1;\
stop_wlc_connect -v 2 -c 1 -w 1;\
start_wlc_connect -v 2 -c 1 -w 1;\
get_upstream_rssi -v 0 -c 3 -w 1;\
get_upstream_rssi -v 1 -c 3 -w 1;\
get_upstream_rssi -v 2 -c 3 -w 1;\
set_pre_config -v 0 -c 3 -w 1;\
set_pre_config -v 1 -c 3 -w 1;\
set_pre_config -v 2 -c 3 -w 1;\
set_post_config -v 0 -c 3 -w 1;\
set_post_config -v 1 -c 3 -w 1;\
set_post_config -v 2 -c 3 -w 1;\
wait_wifi_ready -c 3 -w 1;\
get_wlan_status -v 0,1 -c 3 -w 1;\
get_wlan_status -v 1,1 -c 3 -w 1;\
set_wlan_status -v 0,1,0 -c 3 -w 1;\
set_wlan_status -v 1,1,1 -c 3 -w 1;\
set_wlan_status -v 0,1,1 -c 3 -w 1;\
set_wlan_status -v 1,1,1 -c 3 -w 1;\
no_need_obd -c 3 -w 1;\
no_need_obdeth -c 3 -w 1;\
config_to_driver -c 3;\
"

## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=1
export tc_prog="./amas_test"
export tc_title="Test AiMesh sysdeps for RE"
export tc_logfile=""
$(dirname "$0")/tc-script
