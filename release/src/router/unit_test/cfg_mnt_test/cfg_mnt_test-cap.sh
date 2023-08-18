#!/bin/sh
export tcs="\
sta_list -c 3 -w 1;\
sta_rssi_list -c 3 -w 1;\
get_chans_info -v 0 -c 3 -w 1;\
get_chans_info -v 1 -c 3 -w 1;\
wl_ctrl_channel -v 0 -c 3 -w 1;\
wl_ctrl_channel -v 1 -c 3 -w 1;\
set_chconf -v 0 -c 3 -w 1;\
set_chconf -v 1 -c 3 -w 1;\
set_maclist -c 3 -w 1;\
set_macmode -v 1 -c 3 -w 1;\
get_bw_cap -v 0 -c 3 -w 1;\
get_bw_cap -v 1 -c 3 -w 1;\
wl_chspec_changed -c 3 -w 10;\
"
## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=1
export tc_prog="./cfg_mnt_test"
export tc_title="Test cfg_mnt sysdeps for CAP"
export tc_logfile=""
$(dirname "$0")/tc-script
