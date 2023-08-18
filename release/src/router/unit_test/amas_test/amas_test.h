/*
**  amas_test.h
**
**
**
*/
#ifndef __AMAS_TESTH__
#define __AMAS_TESTH__

void testcase_init(int argc, char *argv[]);
void testcase_free(void);
void testcase_start(int argc, char *argv[]);
void testcase_end(void);

#define MAX_TESTCASE_LIST_SIZE 	256
#define MAX_ARGV_BUFFER_SIZE	128
#define MAX_OUTMSG_BUFFER_SIZE	1024

#define TC_PRINTF(fmt, arg...) do {\
	printf(fmt, ##arg);\
	if (strlen(logfname) > 0)\
	{\
		FILE *fp = fopen(logfname, "a");\
		if (fp)\
		{\
			time_t t;\
			char *pos = NULL;\
			t = time(NULL);\
			pos = ctime(&t);\
			if (strlen(pos) > 12)\
			{\
				pos[strlen(pos)-1]=' ';\
				pos+=4;\
				pos[strlen(pos)-6] = '\0';\
			}\
			fprintf(fp, "[%s] "fmt, pos, ##arg);\
			fclose(fp);\
		}\
	}\
}while(0)

#define SET_OUTMSG(...) do {\
	memset(outmsg, 0, MAX_OUTMSG_BUFFER_SIZE);\
	snprintf(outmsg, MAX_OUTMSG_BUFFER_SIZE, __VA_ARGS__);\
}while(0)

#define CLEAN_OUTMSG() do {\
	memset(outmsg, 0, MAX_OUTMSG_BUFFER_SIZE);\
}while(0)

typedef struct testcase_t {
	char* name;
	int (*fn) (int argc, char *argv[], char *outmsg);
	char* description;
} testcase;

int tc_get_amas_ssd_result(int argc, char *argv[], char *outmsg);
int tc_Pty_get_wlc_status(int argc, char *argv[], char *outmsg);
int tc_Pty_start_wlc_connect(int argc, char *argv[], char *outmsg);
int tc_Pty_stop_wlc_connect(int argc, char *argv[], char *outmsg);
int tc_Pty_get_upstream_rssi(int argc, char *argv[], char *outmsg);
int tc_set_pre_sysdep_config(int argc, char *argv[], char *outmsg);
int tc_set_post_sysdep_config(int argc, char *argv[], char *outmsg);
int tc_get_radar_status(int argc, char *argv[], char *outmsg);
int tc_pty_procedure_check(int argc, char *argv[], char *outmsg);
int tc_amas_wait_wifi_ready(int argc, char *argv[], char *outmsg);
int tc_get_wlan_service_status(int argc, char *argv[], char *outmsg);
int tc_set_wlan_service_status(int argc, char *argv[], char *outmsg);
int tc_no_need_obd(int argc, char *argv[], char *outmsg);
int tc_no_need_obdeth(int argc, char *argv[], char *outmsg);
int tc_apply_config_to_driver(int argc, char *argv[], char *outmsg);

static struct testcase_t testcases[] = 
{
	{	"get_amas_ssd_result\0",	tc_get_amas_ssd_result,			"Test get_amas_ssd_result\0"        				},
	{	"get_wlc_status\0",			tc_Pty_get_wlc_status,     		"Test pty_get_wlc_status\0"        	},
	{	"start_wlc_connect\0", 		tc_Pty_start_wlc_connect, 		"Test pty_start_wlc_connect\0"		},
	{	"stop_wlc_connect\0", 		tc_Pty_stop_wlc_connect, 		"Test pty_stop_wlc_connect\0"		},
	{	"get_upstream_rssi\0", 		tc_Pty_get_upstream_rssi, 		"Test pty_get_upstream_rssi\0"		},
	{	"set_pre_config\0", 		tc_set_pre_sysdep_config, 		"Test set_pre_sysdep_config\0"		},
	{	"set_post_config\0", 		tc_set_post_sysdep_config, 		"Test set_post_sysdep_config\0"		},
	{	"get_radar_status\0", 		tc_get_radar_status, 			"Test get_radar_status\0"			},
	{	"pty_procedure_check\0", 	tc_pty_procedure_check, 		"Test Pty_procedure_check\0"		},
	{	"wait_wifi_ready\0", 		tc_amas_wait_wifi_ready, 		"Test amas_wait_wifi_ready\0"		},
	{	"get_wlan_status\0", 		tc_get_wlan_service_status, 	"Test get_wlan_service_status\0"	},
	{	"set_wlan_status\0", 		tc_set_wlan_service_status, 	"Test set_wlan_service_status\0"	},
	{	"no_need_obd\0", 			tc_no_need_obd, 				"Test no_need_obd\0"				},
	{	"no_need_obdeth\0", 		tc_no_need_obdeth,				"Test no_need_obdeth\0"				},
	{	"config_to_driver\0", 		tc_apply_config_to_driver, 		"Test apply_config_to_driver\0"		},
	{ 	NULL, NULL, NULL },
};

#endif	/* __AMAS_TESTH__ */
