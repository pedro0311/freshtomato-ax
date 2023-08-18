/*
**  obd_test.h
**
**
**
*/
#ifndef __OBD_TESTH__
#define __OBD_TESTH__

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

int tc_obd_init(int argc, char *argv[], char *outmsg);
int tc_obd_final(int argc, char *argv[], char *outmsg);
int tc_obd_save_para(int argc, char *argv[], char *outmsg);
int tc_obd_start_active_scan(int argc, char *argv[], char *outmsg);
int tc_obd_get_bss_scan_result(int argc, char *argv[], char *outmsg);
int tc_obd_start_wps_enrollee(int argc, char *argv[], char *outmsg);
int tc_obd_add_probe_req_vsie(int argc, char *argv[], char *outmsg);
int tc_obd_del_probe_req_vsie(int argc, char *argv[], char *outmsg);
int tc_obd_led_blink(int argc, char *argv[], char *outmsg);
int tc_obd_led_off(int argc, char *argv[], char *outmsg);

static struct testcase_t testcases[] =
{
	{	"obd_init\0",					tc_obd_init,     				"Test obd_init()\0"       			},
  	{  	"obd_final\0",  				tc_obd_final,    				"Test obd_final()\0"      			},
  	{	"obd_save_para\0", 				tc_obd_save_para, 				"Test obd_save_para()\0"			},
  	{	"obd_start_active_scan\0", 		tc_obd_start_active_scan, 		"Test obd_start_active_scan()\0"	},
  	{	"obd_get_bss_scan_result\0", 	tc_obd_get_bss_scan_result,		"Test obd_get_bss_scan_result()\0"	},
  	{	"obd_start_wps_enrollee\0", 	tc_obd_start_wps_enrollee, 		"Test obd_start_wps_enrollee()\0"	},
  	{	"obd_add_probe_req_vsie\0", 	tc_obd_add_probe_req_vsie, 		"Test obd_add_probe_req_vsie()\0"	},
  	{	"obd_del_probe_req_vsie\0", 	tc_obd_del_probe_req_vsie, 		"Test obd_del_probe_req_vsie()\0"	},
  	{	"obd_led_blink\0", 				tc_obd_led_blink,				"Test obd_led_blink()\0"			},
  	{	"obd_led_off\0", 				tc_obd_led_off, 				"Test obd_led_off()\0"				},
  	{  	NULL, NULL, NULL },
};

#endif	/* __OBD_TESTH__ */
