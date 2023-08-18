#!/bin/sh
export tcs="\
switch_set -v 501 -c 3;\
switch_set -v 502 -c 3;\
switch_set -v 503 -c 3;\
switch_set -v 504 -c 3;\
switch_unset -v 501 -c 3;\
switch_unset -v 502 -c 3;\
switch_unset -v 503 -c 3;\
switch_unset -v 504 -c 3;\
"
## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=1
export tc_prog="./amas_wgn_test"
export tc_title="Test AiMesh guest network sysdeps"
export tc_logfile=""
$(dirname "$0")/tc-script
