#!/bin/sh
export tcs="\
get_wlc_status -v 0 -c 3 -w 1;\
stop_wlc_connect -v 0 -c 1 -w 1;\
get_wlc_status -v 0 -c 3 -w 1;\
start_wlc_connect -v 0,00heidi_ax53u,FC:34:97:09:42:40 -c 1 -w 10;\
get_wlc_status -v 0 -c 3 -w 1;\
get_wlc_status -v 1 -c 3 -w 1;\
stop_wlc_connect -v 1 -c 1 -w 1;\
get_wlc_status -v 1 -c 3 -w 1;\
start_wlc_connect -v 1,00heidi_ax53u_5G,FC:34:97:09:42:44 -c 1 -w 10;\
get_wlc_status -v 1 -c 3 -w 10;\
"

## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=100
export tc_prog="./amas_test"
export tc_title="Test AiMesh wlcconnect for RE"
export tc_logfile=""
$(dirname "$0")/tc-script
