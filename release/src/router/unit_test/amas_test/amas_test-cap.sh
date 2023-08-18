#!/bin/sh
export tcs="\
get_radar_status -v 1 -c 3 -w 5;\
wait_wifi_ready -c 3 -w 1;\
no_need_obd -c 3 -w 1;\
no_need_obdeth -c 3 -w 1;\
"
## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=1
export tc_prog="./amas_test"
export tc_title="Test AiMesh sysdeps for CAP"
export tc_logfile=""
$(dirname "$0")/tc-script
