#!/bin/sh
export tcs="\
sta_list -c 3 -w 1;\
sta_rssi_list -c 3 -w 1;\
get_chans_info -v 0 -c 3 -w 1;\
get_chans_info -v 1 -c 3 -w 1;\
wl_ctrl_channel -v 0 -c 3 -w 1;\
wl_ctrl_channel -v 1 -c 3 -w 1;\
set_maclist -c 3 -w 1;\
set_macmode -v 1 -c 3 -w 1;\
get_bw_cap -v 0 -c 3 -w 1;\
get_bw_cap -v 1 -c 3 -w 1;\
get_pap_bssid -v 0 -c 3 -w 1;\
get_pap_bssid -v 1 -c 3 -w 1;\
get_pap_rssi -v 0 -c 3 -w 1;\
get_pap_rssi -v 1 -c 3 -w 1;\
get_sta_mac -v 0 -c 3 -w 1;\
get_sta_mac -v 1 -c 3 -w 1;\
sync_ctrl_channel -v 0,7,20,0 -c 3;\
sync_ctrl_channel -v 0,8,20,0 -c 3;\
sync_ctrl_channel -v 0,7,20,0 -c 3;\
sync_ctrl_channel -v 1,36,20,0 -c 3;\
sync_ctrl_channel -v 1,40,20,0 -c 3;\
sync_ctrl_channel -v 1,36,20,0 -c 3;\
sync_ctrl_channel -v 2,104,20,0 -c 3;\
sync_ctrl_channel -v 2,108,20,0 -c 3;\
sync_ctrl_channel -v 2,104,20,0 -c 3;\
"

## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=1
export tc_prog="./cfg_mnt_test"
export tc_title="Test cfg_mnt sysdeps for RE"
export tc_logfile=""
$(dirname "$0")/tc-script
