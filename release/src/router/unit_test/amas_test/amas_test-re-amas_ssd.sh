#!/bin/sh
export tcs="\
get_amas_ssd_result -v 0,00heidi_ax53u -c 3 -w 5;\
get_amas_ssd_result -v 1,00heidi_ax53u_5G -c 3 -w 5;\
"

## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=100
export tc_prog="./amas_test"
export tc_title="Test AiMesh sitesurvey for RE"
export tc_logfile=""
$(dirname "$0")/tc-script
