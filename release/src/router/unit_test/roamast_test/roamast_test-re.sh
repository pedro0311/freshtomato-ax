#!/bin/sh
export tcs="\
get_stainfo -v 48:4b:aa:ba:50:2e -c 3 -w 1;\
stamon_get_rssi -v 48:4b:aa:ba:50:2e -c 3 -w 1;\
retrieve_static_maclist -v 48:4b:aa:ba:50:2e -c 3 -w 1;\
set_maclist -v 48:4b:aa:ba:50:2e -c 3 -w 1;\
"
## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=1
export tc_prog="./roamast_test"
export tc_title="Test Roamast sysdeps for RE"
export tc_logfile=""
$(dirname "$0")/tc-script
	
