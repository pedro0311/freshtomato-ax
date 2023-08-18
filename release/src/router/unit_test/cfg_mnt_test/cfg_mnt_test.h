/*
**  cfg_mnt_test.h
**
**
**
*/
#ifndef __CFGMNT_TESTH__
#define __CFGMNT_TESTH__

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

int tc_wl_sta_list(int argc, char *argv[], char *outmsg);
int tc_wl_sta_rssi_list(int argc, char *argv[], char *outmsg);
int tc_wl_get_chans_info(int argc, char *argv[], char *outmsg);
int tc_wl_control_channel(int argc, char *argv[], char *outmsg);
int tc_wl_set_chconf(int argc, char *argv[], char *outmsg);
int tc_get_wsc_status(int argc, char *argv[], char *outmsg);
int tc_wl_set_macfilter_list(int argc, char *argv[], char *outmsg);
int tc_wl_set_macfilter_mode(int argc, char *argv[], char *outmsg);
int tc_get_bw_cap(int argc, char *argv[], char *outmsg);
int tc_wl_chanspec_changed(int argc, char *argv[], char *outmsg);
int tc_get_pap_bssid(int argc, char *argv[], char *outmsg);
int tc_get_pap_rssi(int argc, char *argv[], char *outmsg);
int tc_get_sta_mac(int argc, char *argv[], char *outmsg);
int tc_sync_ctrl_channel(int argc, char *argv[], char *outmsg);

static struct testcase_t testcases[] =
{
	{	"sta_list\0", 				tc_wl_sta_list, 			"Test tc_wl_sta_list()\0"				},
	{	"sta_rssi_list\0", 			tc_wl_sta_rssi_list, 		"Test wl_sta_rssi_list()\0"				},
	{	"get_chans_info\0", 		tc_wl_get_chans_info, 		"Test wl_get_chans_info()\0"			},
	{	"wl_ctrl_channel\0", 		tc_wl_control_channel, 		"Test wl_control_channel()\0"			},
	{	"set_chconf\0", 			tc_wl_set_chconf, 			"Test wl_set_chconf()\0"				},
	{	"wsc_status\0", 			tc_get_wsc_status, 			"Test get_wsc_status()\0"				},
	{	"set_maclist\0", 			tc_wl_set_macfilter_list,	"Test wl_set_macfilter_list()\0"		},
	{	"set_macmode\0", 			tc_wl_set_macfilter_mode,	"Test wl_set_macfilter_mode()\0"		},
	{	"get_bw_cap\0", 			tc_get_bw_cap, 				"Test wl_get_bw_cap()\0"				},
	{	"wl_chspec_changed\0", 		tc_wl_chanspec_changed,		"Test wl_chanspec_changed_action()\0"	},
	{	"get_pap_bssid\0", 			tc_get_pap_bssid,			"Test get_pap_bssid()\0"				},
	{	"get_pap_rssi\0",			tc_get_pap_rssi,     		"Test get_pap_rssi()\0"   				},
	{	"get_sta_mac\0",			tc_get_sta_mac,     		"Test get_pap_bssid()\0"   				},
	{	"sync_ctrl_channel\0",		tc_sync_ctrl_channel,     	"Test sync_ctrl_channel()\0"   			},
  	{  	NULL, NULL, NULL },
};

#endif	/* __CFGMNT_TESTH__ */
