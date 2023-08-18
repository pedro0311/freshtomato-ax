#!/bin/sh
export tcs="\
obd_init -c 3 -w 1;\
obd_final -c 3 -w 1;\
obd_save_para -c 3 -w 1;\
obd_start_active_scan -c 1 -w 5;\
obd_get_bss_scan_result -c 1 -w 1;\
obd_start_active_scan -c 1 -w 5;\
obd_get_bss_scan_result -c 1 -w 1;\
obd_start_active_scan -c 1 -w 5;\
obd_get_bss_scan_result -c 1 -w 1;\
obd_add_probe_req_vsie -v 0 -c 3 -w 1;\
obd_del_probe_req_vsie -v 0 -c 3 -w 1;\
obd_led_blink -c 3 -w 1;\
obd_led_off -c 3 -w 1;\
obd_start_wps_enrollee -c 3 -w 1;\
"
## tc_stop=0; no stop rule
## tc_stop=1; termination test-case when test-case fails
## tc_stop=2; termination all test-cases when the test-case fails
export tc_stop=0;
export tcs_item="all"
export tc_cycle=1
export tc_prog="./obd_test"
export tc_title="Test OB sysdeps"
export tc_logfile=""
$(dirname "$0")/tc-script
