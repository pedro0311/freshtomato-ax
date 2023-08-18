/*
**  cfg_mnt_test.c
**
**
**
*/
#include <shared.h>
#include <rtconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cfg_common.h>
#include <chmgmt.h>
#include <cfg_onboarding.h>
#include <cfg_chanspec.h>
#include <shutils.h>
#include <wlioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>
#if defined(RTCONFIG_HND_ROUTER_AX) || defined(RTCONFIG_HND_ROUTER_AX_675X)
#include <bcmwifi_rspec.h>
#endif
#include "cfg_mnt_test.h"

extern int wl_set_chconf(const char* ifname, const chmgmt_chconf_t chconf);
extern int wl_get_chconf(const char* ifname, chmgmt_chconf_t* chconf);

#define ENABLE_SHOW_OPTS
#define ENABLE_TC_LOGFILE
#define TC_LOGFILE_PATH		"/tmp/unittestlog"
#define TC_LOGFILE_NAME		"%s/%s.log"

static char* prog_name = NULL;
static int tc_logfile = 0;
static char logfname[132];
static char* case_name = NULL;
static char* test_title = NULL;

#define DBG_PRINTF(frm, arg...) do {\
	TC_PRINTF("%s: [DBG][LINE:%d] "frm, case_name, __LINE__, ##arg);\
}while(0)\

#define DBG_TRACE_LINE do {\
	TC_PRINTF("%s: [DBG][LINE:%d] TRACE LINE ...\n", case_name, __LINE__);\
}while(0)

#define LIST_ALL_TESTCASE() do {\
	const testcase *tc;\
	printf("\nAvailable test case:\n");\
	for (tc=testcases; tc->name; ++tc)\
		printf("  %s\t - %s\n", tc->name, tc->description);\
	printf("\n");\
}while(0)

#define SHOW_USAGE() do {\
	printf("\nUsage: [-t test-case name [-v arg1,arg2...] [-c test-cycle] [-w wait-time]] [-l] [-m] [-f] [-o]\n");\
	printf("\t-t: test-case name [-v arg1,arg2...] [-c test-cycle] [-w wait-time]\n");\
	printf("\t\t-v: specified argument\n");\
	printf("\t\t-c: test-case cycle\n");\
	printf("\t\t-w: test-case time wait(sec)\n");\
	printf("\t-l: list all test-case\n");\
	printf("\t-m: test title\n");\
	printf("\t-f: log file\n");\
	printf("\t-o: more options:\n");\
	printf("\t\t1: enable termination test-case when test-case fails\n");\
	printf("\t\t2: enable termination all test-cases when the test-case fails\n");\
}while(0)

struct testcase_param_t {
	testcase *tc;
	int argc;
	char **argv;
	char outmsg[MAX_OUTMSG_BUFFER_SIZE];
	unsigned int test_cycle;
	unsigned int test_passed;
	unsigned int test_failed;
	unsigned int wait_time;
};

char *str_lowercase(
	char *str)
{
	int i;
	char *p = str;
	
	for (i=0; i<(int)strlen(str) && p != NULL; i++, p++)
		*p = (char)tolower((int)*p);
	return str;
}

char* tc_argv_to_string(struct testcase_param_t *tc_param, char *b, int b_size)
{
	int i, c = 0;

	if (!tc_param || !b || b_size < 1)
		return NULL;

	if (tc_param->argc > 1)
	{
		for (i=1, c=0; i<tc_param->argc && c<b_size; i++)
		{
			strlcat(b, tc_param->argv[i], b_size);
			c += strlen(tc_param->argv[i]);
			strlcat(b, ",", b_size);
			c += 1;
		}

		if (strlen(b) > 0)
			b[strlen(b)-1] = '\0';
	}

	return b;
}

testcase* find_test_case(char *name)
{
	char found = 0;
	testcase *tc;

	if (name)
	{
		for (found=0, tc=testcases; tc->name; ++tc)
		{
			if (strlen(tc->name) == strlen(name))
			{
				if ((found = (strncmp(name, tc->name, strlen(tc->name)) == 0)))
					break;	
			}
		}
	}

	return (found==1) ? tc : NULL;
}

static int args_set(char *args, char **argv, char comp)
{
	int count = 0;

	//while (isspace(*args)) ++args;
	while (*args == comp) ++args;
	while (*args) 
	{
    	if (argv) argv[count] = args;
   		//while (*args && !isspace(*args)) ++args;
   		while (*args && *args != comp) ++args;
   		if (argv && *args) *args++ = '\0';
    	//while (isspace(*args)) ++args;
 		while (*args == comp) ++args;
 		count++;
	}
	
	return count;
}

char** args_parsed(char *args, int *argc, char comp)
{
	char **argv = NULL;
	int argn = 0;

  	if (args && *args
        && (args = strdup(args))
        //&& (argn = args_set(args,NULL))
        && (argn = args_set(args, NULL, comp))
        && (argv = malloc((argn+1) * sizeof(char *)))) 
  	{
    	*argv++ = args;
     	//argn = args_set(args,argv);
     	argn = args_set(args, argv, comp);
  	}

 	if (args && !argv) free(args);

  	//*argc = argn;
  	if (argc) *argc = argn;
  	return argv;
}

void args_parsed_free(char **argv)
{
  	if (argv) 
  	{
   		free(argv[-1]);
        free(argv-1);
  	} 
}

void free_tcs_args_parsed(struct testcase_param_t *tcs_param, int tcs_count)
{
	int i;
	struct testcase_param_t *p = NULL;

	for (i=0, p=&tcs_param[0]; i<tcs_count && p; i++, p++)
		args_parsed_free(p->argv);

	return;
}

int tc_args_parsed(struct testcase_param_t *tc_param, char *optarg)
{
	testcase *tc = NULL;
	int i, argc = 0, ac = 0;
	char **argv = NULL;
	char *args = NULL, *as = NULL, b[1024];

	if (!tc_param || !optarg || strlen(optarg) <= 0)
		goto tc_args_parsed_fail;

	args = optarg;
	argv = args_parsed(args, &argc, ' ');
	if (argc <= 0)
		goto tc_args_parsed_fail;
	if (!(tc = find_test_case(argv[0])))
		goto tc_args_parsed_fail;

	if (argc > 1)
	{
		for (i=1; i<argc; i++)
		{
			if (argv[i][0] == '-')
			{
				switch (argv[i][1])
				{
					case 'v':
					case 'V':
						i++;
						if (i<argc && strlen(argv[i])>0)
						{
#ifdef ENABLE_PARSE_TC_ARGS							
							memset(b, 0, sizeof(b));
							snprintf(b, sizeof(b), "%s,", argv[0]);
							strlcat(b, argv[i], sizeof(b));
							as = b;
							tc_param->argv = args_parsed(as, &ac, ',');
							tc_param->argc = ac;
#else 	/* ENABLE_PARSE_TC_ARGS */
							memset(b, 0, sizeof(b));
							snprintf(b, sizeof(b), "%s ", argv[0]);
							strlcat(b, argv[i], sizeof(b));
							as = b;
							tc_param->argv = args_parsed(as, &ac, ' ');
							tc_param->argc = ac;
#endif	/* ENABLE_PARSE_TC_ARGS */						
						}
						break;
					case 'C':
					case 'c':
						i++;
						if (i<argc && strlen(argv[i])>0)
							tc_param->test_cycle = (atoi(argv[i]) > 0) ? atoi(argv[i]) : 1;
						break;
					case 'W':
					case 'w':
						i++;
						if (i<argc && strlen(argv[i])>0)
							tc_param->wait_time = atoi(argv[i]);
						break;
				}
			}
		}
	}

	tc_param->tc = tc;

	if (!tc_param->argv)
	{
		memset(b, 0, sizeof(b));
		snprintf(b, sizeof(b), "%s", argv[0]);
		as = b;
		tc_param->argv = args_parsed(as, &ac, ',');
		tc_param->argc = ac;
	}

	if (tc_param->test_cycle < 1)
		tc_param->test_cycle = 1;

	args_parsed_free(argv);
	return 1;

tc_args_parsed_fail:
	args_parsed_free(argv);
	return 0;
}

char *path_name(const char *str)
{
	char *path = strdup(str);
	return (!path) ? NULL : dirname(path);
}

char *file_name(const char *str)
{
	char *name = strdup(str);
	return (!name) ? NULL : basename(name);
}


int main(int argc, char *argv[])
{
#define TC_OPT_FAIL_STOP		0x01
#define TC_OPT_FAIL_STOP_ALL	0x02

	struct testcase_param_t tc_param[MAX_TESTCASE_LIST_SIZE];
	int i, j, cmd_opt = 0, tc_count = 0, result = 0;
	unsigned char tc_options = 0;
	char show_list = 0, tc_stop = 0, s[1024], s1[1024], s2[1024], *dir_name = NULL;
	struct stat st;

	(void)testcase_init(argc, argv);
	if (strncmp(argv[0], "./", 2) == 0)
		prog_name = &argv[0][2];
	else
		prog_name = argv[0];

	// default log filename	
	memset(logfname, 0, sizeof(logfname));
	snprintf(logfname, sizeof(logfname), TC_LOGFILE_NAME, TC_LOGFILE_PATH, prog_name);
	memset(tc_param, 0, (sizeof(struct testcase_param_t) * MAX_TESTCASE_LIST_SIZE));
	if (argc > 1)
	{
		while (1)
		{
			cmd_opt = getopt(argc, argv, "T:t:LlM:m:F:f:O:o:");
			if (cmd_opt == -1)
				break;

			switch (cmd_opt)
			{
				case 'T':
				case 't':
					if (optarg)
					{
						if (tc_args_parsed(&tc_param[tc_count], optarg) == 1)
							tc_count++;
						else
							TC_PRINTF("unknown optarg: %s\n", optarg);
					}
					break;
				case 'O':
				case 'o':
					if (optarg)
					{
						switch (atoi(optarg))
						{
							case 1:	/* 1: enable termination test-case when test-case fails */
								tc_options |= TC_OPT_FAIL_STOP;
								break;
							case 2: /* 2: enable termination all test-cases when the test-case fails */
								tc_options |= TC_OPT_FAIL_STOP_ALL;
								break;
						}
					}
					break;
				case 'M':
				case 'm':
					test_title = (optarg) ? optarg : NULL;
					break;
				case 'F':
				case 'f':
					if (optarg)
					{
						memset(logfname, 0, sizeof(logfname));
						strlcpy(logfname, optarg, sizeof(logfname)); 
					}
					break;
				case 'L':
				case 'l':
					show_list = 1;
					break;
            	case '?':
        			TC_PRINTF("Illegal option:-%c\n", isprint(optopt)?optopt:'#');
        			break;
        		default:
        			TC_PRINTF("Not supported option: -%c\n", cmd_opt);
        			break;
			}
		}
	}

	// options check "TC_OPT_FAIL_STOP" & "TC_OPT_FAIL_STOP_ALL"
	if ((tc_options & TC_OPT_FAIL_STOP_ALL) == TC_OPT_FAIL_STOP_ALL)
		tc_options |= ~TC_OPT_FAIL_STOP;

	if (tc_count > 0)
	{
		memset(&st, 0, sizeof(struct stat));		
		if ((dir_name = path_name(logfname)))
		{
			if (stat(dir_name, &st) == -1) 
		    	mkdir(dir_name, 0700);
		    free(dir_name);
		}
		else
		{
			memset(logfname, 0, sizeof(logfname));
		}
	
		TC_PRINTF("==============================[START]=======================================\n");
		if (test_title)			
			TC_PRINTF("Title: %s\n", test_title);		
		TC_PRINTF("Test Module: %s\n", prog_name);
		TC_PRINTF("LOG File: %s\n", logfname);

#ifdef ENABLE_SHOW_OPTS
		if (tc_options != 0)
		{
			TC_PRINTF("Options:\n");
			if ((tc_options & TC_OPT_FAIL_STOP) == TC_OPT_FAIL_STOP)
				TC_PRINTF("\t1: enable termination test-case when test-case fails\n");
			if ((tc_options & TC_OPT_FAIL_STOP_ALL) == TC_OPT_FAIL_STOP_ALL)
				TC_PRINTF("\t2: enable termination all test-cases when the test-case fails\n");
		} 
#endif	/* ENABLE_SHOW_OPTS */		

#ifdef ENABLE_SHOW_ARGS	
		memset(s, 0, sizeof(s));
		for (i=1; i<argc; i++)
		{
			strlcat(s, argv[i], sizeof(s));
			strlcat(s, " ", sizeof(s));
		}
		TC_PRINTF("Args: %s\n", s);
#endif	/* ENABLE_ARGS_SHOW */
		(void)testcase_start(argc, argv);

		for (i=0; i<tc_count && !tc_stop; i++)
		{
			TC_PRINTF("\n");
			case_name = tc_param[i].tc->name;
			memset(s1, 0, sizeof(s1));
			// print test-case
			memset(s2, 0, sizeof(s2));
			snprintf(s2, sizeof(s2), "test-case: %s, cycle: %d ", tc_param[i].tc->name, tc_param[i].test_cycle);
			strlcat(s1, s2, sizeof(s1));
			// print wait_time
			if (tc_param[i].wait_time > 0)
			{
				memset(s2, 0, sizeof(s2));
				snprintf(s2, sizeof(s2), ", time-wait: %d ", tc_param[i].wait_time);
				strlcat(s1, s2, sizeof(s1));
			}
			// print args
			if (tc_param[i].argc > 1)
			{
				memset(s, 0, sizeof(s));
				memset(s2, 0, sizeof(s2));
				snprintf(s2, sizeof(s2), ", args: %s ", tc_argv_to_string(&tc_param[i], s, sizeof(s)-1));
				strlcat(s1, s2, sizeof(s2));
			}
			TC_PRINTF("%s\n", s1);
			for (j=0; j<tc_param[i].test_cycle; j++)
			{
				memset(tc_param[i].outmsg, 0, MAX_OUTMSG_BUFFER_SIZE);
				result = tc_param[i].tc->fn(tc_param[i].argc, tc_param[i].argv, &tc_param[i].outmsg[0]);
				TC_PRINTF("[#%d]%s: %s\n", j+1, tc_param[i].tc->name, (strlen(tc_param[i].outmsg) > 0) ? tc_param[i].outmsg : "done");
				if (result > 0)
					tc_param[i].test_passed++;
				else
					tc_param[i].test_failed++;

				if (result <= 0)
				{
					if ((tc_options & TC_OPT_FAIL_STOP_ALL) == TC_OPT_FAIL_STOP_ALL)
						tc_stop = 1;

					if ((tc_options & TC_OPT_FAIL_STOP) == TC_OPT_FAIL_STOP ||
						(tc_options & TC_OPT_FAIL_STOP_ALL) == TC_OPT_FAIL_STOP_ALL)
						break;					
				}

				if (tc_param[i].wait_time > 0)
					sleep(tc_param[i].wait_time);
			}

			TC_PRINTF("%s: cycle: %d, pass: %d, fail: %d\n", tc_param[i].tc->name, tc_param[i].test_cycle, tc_param[i].test_passed, tc_param[i].test_failed);
		}

		TC_PRINTF("===============================[END]========================================\n");
		testcase_end();		
		free_tcs_args_parsed(tc_param, tc_count);
	}
	else
	{
		if (show_list)
			LIST_ALL_TESTCASE();
		else
		{
			SHOW_USAGE();
			LIST_ALL_TESTCASE();
		}		
	}

	(void)testcase_free();
	return (tc_stop==1) ? 0 : 1;
}

void testcase_init(int argc, char *argv[])
{	
	// TODO
	return;
}

void testcase_free(void)
{
	// TODO
	return;
}

void testcase_start(int argc, char *argv[])
{
	// TODO
	TC_PRINTF("Model: %s\n", nvram_safe_get("model"));
	TC_PRINTF("Firmware: %s.%s_%s\n", 
		nvram_safe_get("firmver"), 
		nvram_safe_get("buildno"), 
		nvram_safe_get("extendno"));
	return;
}

void testcase_end(void)
{
	// TODO
	return;
}

#define TC_SET_MACLIST_CHECK_SHOW_USAGE() do {\
	if (argc < 2)\
	{\
		printf("usage: %s mac\n", argv[0]);\
		printf("\tmac: mac address\n");\
		return 0;\
	}\
}while(0)

#define TC_SET_MACMODE_CHECK_SHOW_USAGE() do {\
	if (argc < 2)\
	{\
		printf("usage: %s allow\n", argv[0]);\
		printf("\tallow: 0:disable, 1:allow\n");\
		return 0;\
	}\
}while(0)

#if 0
#define TC_SYNCCTRLCH_CHECK_SHOW_USAGE() do {\
	if (argc < 5)\
	{\
		printf("usage: %s unit,channel,bw,nctrlsb\n", argv[0]);\
		printf("\tunit: 0:2.4g, 1:5g1, 2:5g2\n");\
		printf("\tchannel: wireless channel\n");\
		printf("\tbw: channel bandwitch\n");\
		printf("\tnctrlsb: nctrlsb\n");\
		return 0;\
	}\
}while(0)
#endif

#define TC_WLUNIT_CHECK_SHOW_USAGE() do {\
	if (argc < 2)\
	{\
		printf("usage: %s wl_unit\n", argv[0]);\
		printf("\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n");\
		return 0;\
	}\
}while(0)

#if 0
#define TC_SETCHCONF_CHECK_SHOW_USAGE() do {\
	if (argc < 3)\
	{\
		printf("usage: %s wl_unit,wl_chnum\n", argv[0]);\
		printf("\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n");\
		printf("\twl_chnum: channel number\n");\
		return 0;\
	}\
}while(0)
#endif

#define TC_SETMACMODE_CHECK_SHOW_USAGE() do {\
	if (argc < 2)\
	{\
		printf("usage: %s allow\n", argv[0]);\
		printf("\allow: 0:, 1:allow\n");\
		return 0;\
	}\
}while(0)

int tc_wl_sta_list(int argc, char *argv[], char *outmsg)
{
	int test_cycle = 0, i = 0, fail_count = 0;
	int result = 0;
	char s[1024];

	if (argc > 1)
		test_cycle = atoi(argv[1]);
	else
		test_cycle = 1;

	for (i=0, fail_count=0; i<test_cycle; i++)
	{
		memset(s, 0, sizeof(s));
		if (!wl_sta_list(s, sizeof(s)))
		{
			result = 0;
			DBG_PRINTF("Call wl_sta_list() fail ...\n");
		}
		else
		{
			result = 1;
		}

		if (result == 0) 
		{
			fail_count++;
		}
	}

	if (test_cycle > 1)
		SET_OUTMSG("Test cycle: %d, Pass: %d, Fail: %d", test_cycle, test_cycle-fail_count, fail_count);
	else
		SET_OUTMSG("%s", ((result==1) ? s : "FAIL"));

	return (test_cycle > 1) ? 1 : result;
}

#if 0
int tc_wl_sta_list(int argc, char *argv[], char *outmsg)
{	
	char msg[1024];

	memset(msg, 0, sizeof(msg));
	if (!wl_sta_list(msg, sizeof(msg)))
	{
		DBG_PRINTF("Call wl_sta_list() fail ...\n");
		goto tc_wl_sta_list_fail;
	}

	SET_OUTMSG("%s", msg);
	return 1;

tc_wl_sta_list_fail:
	SET_OUTMSG("FAIL");
	return 0;
}
#endif

int tc_wl_sta_rssi_list(int argc, char *argv[], char *outmsg)
{
	json_object *json = NULL;

	if (!(json = json_object_new_object()))
	{
		DBG_PRINTF("Call json_object_new_object() fail ...\n");
		goto tc_wl_sta_rssi_list_fail;
	}

	if (wl_sta_rssi_list(json) != 1)
	{
		DBG_PRINTF("Call wl_sta_rssi_list() fail ...\n");
		goto tc_wl_sta_rssi_list_fail;		
	}

	SET_OUTMSG("%s", json_object_get_string(json));
	json_object_put(json);
	return 1;

tc_wl_sta_rssi_list_fail:
	if (json) json_object_put(json);
	SET_OUTMSG("FAIL");
	return 0;
}

int tc_wl_get_chans_info(int argc, char *argv[], char *outmsg)
{
	int unit = 0;
	char chInfo[MAX_CHANSPEC_BUFLEN];

	TC_WLUNIT_CHECK_SHOW_USAGE();
	unit = atoi(argv[1]);
	memset(chInfo, 0, sizeof(chInfo));
	if (wl_get_chans_info(unit, chInfo, sizeof(chInfo)) < 0)
	{
		DBG_PRINTF("Call wl_get_chans_info() fail ...\n");
		goto tc_wl_get_chans_info_fail;
	}

	SET_OUTMSG("%s", chInfo);
	return 1;

tc_wl_get_chans_info_fail:
	SET_OUTMSG("FAIL");
	return 0;
}


int tc_wl_control_channel(int argc, char *argv[], char *outmsg)
{
	int unit = 0, channel = 0, bw = 0, nctrlsb = 0;

	TC_WLUNIT_CHECK_SHOW_USAGE();
	unit = atoi(argv[1]);
	wl_control_channel(unit, &channel, &bw, &nctrlsb);
	SET_OUTMSG("channel:%d, bw:%d, nctrlsb:%d", channel, bw, nctrlsb);
	return 1;
}

int tc_wl_set_chconf(int argc, char *argv[], char *outmsg)
{	
	char *ifname = NULL, s[80];
	chmgmt_chconf_t sel_chconf, cur_chconf;

	if (argc < 2)
		goto tc_wl_set_chconf_usage;

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wl%d_ifname", atoi(argv[1]));
	if (!(ifname = nvram_get(s)))
	{
		SET_OUTMSG("FAIL");
		return 0;
	}

	wl_set_chconf(ifname, sel_chconf);
	if (wl_get_chconf(ifname, &cur_chconf) < 0)
	{
		SET_OUTMSG("FAIL");
		return 0;
	}

	SET_OUTMSG("%s", (cur_chconf==sel_chconf)?"FAIL":"OK");
	return (cur_chconf==sel_chconf) ? 0 : 1;

tc_wl_set_chconf_usage:
	SET_OUTMSG(
		"usage: %s wl_unit,wl_chnum\n"\
		"\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n", argv[0]);
	return 0;	
}

int verify_wl_set_macfilter_list(char *str, pid_t pid)
{
	char s[132];
	char *b = NULL;
	FILE *f = NULL;
	size_t alloc_size = 0;
	int found = 0;

	if (!str || strlen(str) <= 0)
		goto verify_obd_start_wps_enrollee_fail;

	if (!(f = fopen("/tmp/syslog.log", "r")))
	{
		DBG_PRINTF("fopen fail ...\n");
		goto verify_obd_start_wps_enrollee_fail;
	}

	fseek(f, 0, SEEK_END); // seek to end of file
	alloc_size = ftell(f); // get current file pointer
	fseek(f, 0, SEEK_SET);

	if (!(b = malloc(alloc_size)))
	{
		DBG_PRINTF("malloc fail ...\n");
		goto verify_obd_start_wps_enrollee_fail;
	}

	memset(b, 0, alloc_size);
	if (fread(b, 1, alloc_size, f) <= 0)
	{
		DBG_PRINTF("fread fail ...\n");
		goto verify_obd_start_wps_enrollee_fail;
	}

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "rc_service: %s %d:notify_rc %s", prog_name, (int)pid, str);
	if (strstr(b, s))
		found = 1;

	free(b);
	fclose(f);
	return found;

verify_obd_start_wps_enrollee_fail:
	if (b) free(b);
	if (f) fclose(f);
	return 0;
}

#if 0
int verify_wl_set_macfilter_list(void)
{
	time_t t;
	char *pos;
	int sz=0;
	char s[132];
	char key[] = {"rc_service: %s %d:notify_rc restart_roamast"};
	char *b = NULL, *b1 = NULL;
	FILE *f = NULL;
	size_t alloc_size = 0;
	int found = 0;

	t = time(NULL);
	pos = ctime(&t);
	if ((sz = strlen(pos)) < 12)
		goto verify_wl_set_macfilter_list_fail;

	pos[sz-1]=' ';
	pos+=4;
	//pos[strlen(pos)-9] = '\0';
	pos[strlen(pos)-6] = '\0';

	if (!(f = fopen("/tmp/syslog.log", "r")))
	{
		DBG_PRINTF("fopen fail ...\n");
		goto verify_wl_set_macfilter_list_fail;
	}

	fseek(f, 0, SEEK_END); // seek to end of file
	alloc_size = ftell(f); // get current file pointer
	fseek(f, 0, SEEK_SET);

	if (!(b = malloc(alloc_size)))
	{
		DBG_PRINTF("malloc fail ...\n");
		goto verify_wl_set_macfilter_list_fail;
	}

	memset(b, 0, alloc_size);
	if (fread(b, 1, alloc_size, f) <= 0)
	{
		DBG_PRINTF("fread fail ...\n");
		goto verify_wl_set_macfilter_list_fail;
	}
	if (!(b1 = strstr(b, pos)))
		goto verify_wl_set_macfilter_list_fail;

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), key, prog_name, getpid());
	if (strstr(b1, s))
		found = 1;
	else
		found = 0;

	free(b);
	fclose(f);
	return found;

verify_wl_set_macfilter_list_fail:
	if (b) free(b);
	if (f) fclose(f);
	return 0;	
}
#endif

int tc_wl_set_macfilter_list(int argc, char *argv[], char *outmsg)
{	
	char s[81], prefix[] = "wlXXXXXXXXXX_", tmp[128];
	char word[256], *next;
	char *wlif_name = NULL;
	int unit = 0, verify = 0;
	char nv_tmp_macmode[3][128], nv_tmp_maclist_x[3][4096], *nv_macmode = NULL, *nv_maclist_x = NULL;
	pid_t pid = -1;
	int exit_status = 0;

#ifdef BCM_BSD
	char find_str[] = {"start_bsd"};
#else 	// RTCONFIG_RALINK
	char find_str[] = {"restart_roamast"};
#endif	// RTCONFIG_RALINK	
	
	unit = 0;
	memset(nv_tmp_macmode, 0, 3*128);
	memset(nv_tmp_maclist_x, 0, 3*128);
	foreach (word, nvram_safe_get("wl_ifnames"), next) 
	{
		SKIP_ABSENT_BAND_AND_INC_UNIT(unit);
       	if (nvram_get_int("re_mode") == 1)
         	snprintf(prefix, sizeof(prefix), "wl%d.1_", unit);
       	else
         	snprintf(prefix, sizeof(prefix), "wl%d_", unit);		
       	wlif_name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

       	nv_maclist_x = nvram_safe_get(strcat_r(prefix, "maclist_x", tmp));
       	strlcpy(nv_tmp_maclist_x[unit], nv_maclist_x, 128);
       	nvram_set(strcat_r(prefix, "maclist_x", tmp), s);

       	nv_macmode = nvram_safe_get(strcat_r(prefix, "macmode", tmp));
       	strlcpy(nv_tmp_macmode[unit], nv_macmode, 128);
       	nvram_set(strcat_r(prefix, "macmode", tmp), "allow");       	
       	unit++;
	}

	pid = fork();
	if (pid == -1)
	{
		DBG_PRINTF("fork() return fail ...\n");
		verify = 0;
	}
	else if (pid == 0)
	{
		wl_set_macfilter_list();
	}
	else
	{
		wait(&exit_status);
		verify = verify_wl_set_macfilter_list(find_str, pid);
	}

	if (verify == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	// restore nvram 
	unit = 0;
	foreach (word, nvram_safe_get("wl_ifnames"), next) 
	{
		SKIP_ABSENT_BAND_AND_INC_UNIT(unit);
		nvram_set(strcat_r(prefix, "maclist_x", tmp), nv_tmp_maclist_x[unit]);
		nvram_set(strcat_r(prefix, "macmode", tmp), nv_tmp_macmode[unit]);
		unit++;
	}

	return (verify == 1) ? 1 : 0;
}

int verify_wl_set_macfilter_mode(int allow)
{
#ifdef RTCONFIG_RALINK
	FILE *f = NULL;
	char s[81], *b = NULL;
	size_t alloc_size = 4096;
#endif	/* RTCONFIG_RALINK */	

	char tmp[128], prefix[] = "wlXXXXXXXXXX_";
   	char *wlif_name = NULL;
   	int ret = 0;
   	int val = 2;    /* allow mode */
  	int unit = 0;
  	int result = 1;
  	int verify_val = val;

  	if (nvram_get_int("re_mode") == 1)
     	snprintf(prefix, sizeof(prefix), "wl%d.1_", unit);
   	else
      	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	
	wlif_name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
 	if (nvram_match(strcat_r(prefix, "macmode", tmp), "allow")) 
 	{
   		if (allow == 1)
       		verify_val = 0;        /* disabled */

#ifdef RTCONFIG_RALINK
   		result = 0;
  		memset(s, 0, sizeof(s));
   		snprintf(s, sizeof(s), "iwpriv %s set Debug=3", wlif_name);
   		system(s);   		   	
   		memset(s, 0, sizeof(s));
   		snprintf(s, sizeof(s), "iwpriv %s set ACLShowAll=1", wlif_name);
   		if ((f = popen(s, "r")))
   		{
			//fseek(f, 0, SEEK_END); // seek to end of file
			//alloc_size = ftell(f); // get current file pointer
			//fseek(f, 0, SEEK_SET);
			if ((b = malloc(alloc_size)))
			{
				memset(b, 0, alloc_size);
				fread(b, 1, alloc_size, f);
				DBG_PRINTF("b : %s\n", b);
				/*
				if (fread(b, 1, alloc_size, f) > 0)
				{
					memset(s, 0, sizeof(s));
					snprintf(s, sizeof(s), "Policy is %d", val);
					result = (strstr(b, s)) ? 1 : 0;
				}
				else
				{
					DBG_PRINTF("fread failed ...\n");
				}
				*/
				free(b);
			}
			else
			{
				DBG_PRINTF("Memory allocate failed, alocate size: %d ...\n", alloc_size);
			}

   			pclose(f);
   		}
   		else
   		{
   			DBG_PRINTF("popen %s error ...\n", s);
   		}

  		memset(s, 0, sizeof(s));
   		snprintf(s, sizeof(s), "iwpriv %s set Debug=0", wlif_name);
   		system(s);   		
#else	/* RTCONFIG_RALINK */
      	ret = wl_ioctl(wlif_name, WLC_GET_MACMODE, &val, sizeof(val));
       	if(ret < 0)
       	{
          	DBG_PRINTF("[%s] set macmode failed\n", wlif_name);
          	result = -1;
       	}
       	else
       	{
       		result = (val == verify_val) ? 1 : 0;
       	}
#endif	/* RTCONFIG_RALINK */   			
 	}

 	return result;
}


int tc_wl_set_macfilter_mode(int argc, char *argv[], char *outmsg)
{
	int verify = 0;

#ifdef RTCONFIG_RALINK
	SET_OUTMSG("TBD");
	return 1;
#endif	/* RTCONFIG_RALINK */	

	TC_SET_MACMODE_CHECK_SHOW_USAGE();
	wl_set_macfilter_mode(atoi(argv[1]));
	verify = verify_wl_set_macfilter_mode(atoi(argv[1]));
	if (verify==1) 
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return (verify==1) ? 1 : 0;
}

int tc_get_wsc_status(int argc, char *argv[], char *outmsg)
{
	int status = 0, fail_result = 0;

	if ((status = get_wsc_status(&fail_result)) != 1)
	{
		switch (fail_result)
		{
			case OB_WPS_UNKNOWN_FAIL:
				DBG_PRINTF("%d, OB_WPS_UNKNOWN_FAIL(%d)\n", status, fail_result);
				break;
			case OB_WPS_TIMEOUT_FAIL:
				DBG_PRINTF("%d, OB_WPS_TIMEOUT_FAIL(%d)\n", status, fail_result);
				break;
			case OB_WPS_OVERLAP_FAIL:
				DBG_PRINTF("%d, OB_WPS_OVERLAP_FAIL(%d)\n", status, fail_result);
				break;
			default:
				DBG_PRINTF("%d, UNKNOWN_FAIL(%d)\n", status, fail_result);
				break;				
		}	
	}

	SET_OUTMSG("%s", (status==1) ? "OK" : "FAIL");
	return (status==1) ? 1 : 0;
}

int tc_get_bw_cap(int argc, char *argv[], char *outmsg)
{
	int bwCap = 0, result = 0;

	TC_WLUNIT_CHECK_SHOW_USAGE();
	
	result = (wl_get_bw_cap(atoi(argv[1]), &bwCap) < 0) ? 0 : 1;
	if (result)
		SET_OUTMSG("%d", bwCap);
	else
		SET_OUTMSG("FAIL");

	return result;
}

int verify_wl_chanspec_changed()
{
	time_t t;
	char *pos;
	int sz=0;
	char s[132];
	char find_restart_acsd[] = {"rc_service: %s %d:notify_rc restart_acsd"};
	char find_restart_wireless[] = {"rc_service: %s %d:notify_rc restart_wireless"};
	char *b = NULL, *b1 = NULL;
	FILE *f = NULL;
	size_t alloc_size = 0;
	int found = 0;

	t = time(NULL);
	pos = ctime(&t);
	if ((sz = strlen(pos)) < 12)
		goto verify_wl_chanspec_changed_fail;

	pos[sz-1]=' ';
	pos+=4;
	//pos[strlen(pos)-9] = '\0';
	pos[strlen(pos)-6] = '\0';

	if (!(f = fopen("/tmp/syslog.log", "r")))
	{
		DBG_PRINTF("fopen fail ...\n");
		goto verify_wl_chanspec_changed_fail;
	}

	fseek(f, 0, SEEK_END); // seek to end of file
	alloc_size = ftell(f); // get current file pointer
	fseek(f, 0, SEEK_SET);

	if (!(b = malloc(alloc_size)))
	{
		DBG_PRINTF("malloc fail ...\n");
		goto verify_wl_chanspec_changed_fail;
	}

	memset(b, 0, alloc_size);
	if (fread(b, 1, alloc_size, f) <= 0)
	{
		DBG_PRINTF("fread fail ...\n");
		goto verify_wl_chanspec_changed_fail;
	}
	if (!(b1 = strstr(b, pos)))
		goto verify_wl_chanspec_changed_fail;

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), find_restart_wireless, prog_name, getpid());
	if (strstr(b1, s))
		found = 1;
	else
	{
		memset(s, 0, sizeof(s));
		snprintf(s, sizeof(s), find_restart_acsd, prog_name, getpid());
		if (strstr(b1, s))
			found = 1;
	}

	free(b);
	fclose(f);
	return found;

verify_wl_chanspec_changed_fail:
	if (b) free(b);
	if (f) fclose(f);
	return 0;
}

int update_chanspec_list(char value)
{
	json_object *root = NULL;
   	json_object *bandObj = NULL;
   	json_object *channelObj = NULL;
    int unit = 0, result = 0;
   	char word[128], *next = NULL, v[1024];
 
    root = json_object_from_file(CHANSPEC_LIST_JSON_PATH);
   	if (!root) 
   	{
   		DBG_PRINTF("open %s failed ...\n", CHANSPEC_LIST_JSON_PATH);
   	   	return result;
   	}

   	json_object_object_foreach(root, key, val) 
   	{
   		bandObj = val;	
	   	json_object_object_foreach(bandObj, key, val)
	   	{
	     	unit = -1;
	   		if (strcmp(key, CFG_STR_2G) == 0)
	      		unit = 0;
	       	else if (strcmp(key, CFG_STR_5G) == 0 || strcmp(key, CFG_STR_5G1) == 0)
	          	unit = 1;
	   		
	   		if (unit == 1)
	   		{
	          	json_object_object_get_ex(val, CFG_STR_CHANNEL, &channelObj);
	          	if (channelObj)
	          	{
	          		memset(v, 0, sizeof(v));
	          		foreach(word, json_object_get_string(channelObj), next)
	          		{
	          			word[4] = value;
	          			strlcat(v, word, sizeof(v));
	          			strlcat(v, " ", sizeof(v));
	          		}

	          		if (strlen(v) > 0)
	          			v[strlen(v)-1] = '\0';
	          		json_object_object_del(val, CFG_STR_CHANNEL);
	          		json_object_object_add(val, CFG_STR_CHANNEL, json_object_new_string(v));
	          		result = 1;
	          	}
	   		}
	   	}
   	}

   	if (!result)
   		DBG_PRINTF("No update ...\n");

	if (root)
		json_object_to_file(CHANSPEC_LIST_JSON_PATH, root);

	json_object_put(root);
	return result;
}

int tc_wl_chanspec_changed(int argc, char *argv[], char *outmsg)
{
	int test_cycle = 0, x = 0, fail_count = 0;
	int result = 0, set_ch = 0, i = 0;
	AVBL_CHANSPEC_T avblChanspec;
	char *nv_wl1_unavbl_ch = NULL, *pch = NULL, s[81], b[1024];

	if (argc > 1)
		test_cycle = atoi(argv[1]);
	else
		test_cycle = 1;

	for (x=0, fail_count=0; x<test_cycle; x++)
	{
		nvram_set("cfg_test", "1");	
		if (!update_chanspec_list('8'))
		{
			DBG_PRINTF("update_chanspec_list() return fail\n");
			SET_OUTMSG("FAIL");
			return 0;
		}

		nv_wl1_unavbl_ch = nvram_safe_get("wl1_unavbl_ch");
		memset(b, 0, sizeof(b));
		strlcpy(b, nv_wl1_unavbl_ch, sizeof(b));
		memset(s, 0, sizeof(s));		
		for (i=0; i<strlen(b) && i<sizeof(s); i++)
		{
			if (b[i] == ',')
				break;
			s[i] = b[i];
		}

		if (strlen(s) > 0)
			set_ch = atoi(s);

		if (set_ch > 0)
		{
			memset(s, 0, sizeof(s));
			snprintf(s, sizeof(s), "iwpriv rai0 set Channel=%d", set_ch);
			system(s);		
			sleep(3);
		}

		memset(&avblChanspec, 0, sizeof(AVBL_CHANSPEC_T));
		DBG_PRINTF("cm_updateAvblChanspec() return: %d\n", cm_updateAvblChanspec(&avblChanspec));
		wl_chanspec_changed_action(&avblChanspec);
		result = verify_wl_chanspec_changed();
		nvram_unset("cfg_test");
		update_chanspec_list('1');

		if (result==0)
			fail_count++;

		sleep(10);
	}			

	if (test_cycle > 1)
		SET_OUTMSG("Test cycle: %d, Pass: %d, Fail: %d", test_cycle, test_cycle-fail_count, fail_count);
	else
		SET_OUTMSG("%s", ((result==1) ? s : "FAIL"));

	return (test_cycle > 1) ? 1 : result;
}


#if 0
int tc_wl_chanspec_changed(int argc, char *argv[], char *outmsg)
{
	int result = 0, set_ch = 0, i = 0;
	AVBL_CHANSPEC_T avblChanspec;
	char *nv_wl1_unavbl_ch = NULL, *pch = NULL, s[81], b[1024];

	nvram_set("cfg_test", "1");	
	if (!update_chanspec_list('8'))
	{
		DBG_PRINTF("update_chanspec_list() return fail\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	nv_wl1_unavbl_ch = nvram_safe_get("wl1_unavbl_ch");
	memset(b, 0, sizeof(b));
	strlcpy(b, nv_wl1_unavbl_ch, sizeof(b));
	memset(s, 0, sizeof(s));		
	for (i=0; i<strlen(b) && i<sizeof(s); i++)
	{
		if (b[i] == ',')
			break;
		s[i] = b[i];
	}

	if (strlen(s) > 0)
		set_ch = atoi(s);

	if (set_ch > 0)
	{
		DBG_TRACE_LINE;
		memset(s, 0, sizeof(s));
		snprintf(s, sizeof(s), "iwpriv rai0 set Channel=%d", set_ch);
		system(s);		
		sleep(3);
	}

	memset(&avblChanspec, 0, sizeof(AVBL_CHANSPEC_T));
	DBG_PRINTF("cm_updateAvblChanspec() return: %d\n", cm_updateAvblChanspec(&avblChanspec));
	wl_chanspec_changed_action(&avblChanspec);
	result = verify_wl_chanspec_changed();				
	if (result==1)
		SET_OUTMSG("OK");
	else 
		SET_OUTMSG("FAIL");

	nvram_unset("cfg_test");
	update_chanspec_list('1');
	return result;
}
#endif


int tc_get_pap_bssid(int argc, char *argv[], char *outmsg)
{	
	char bssid[18], *b = NULL;

	TC_WLUNIT_CHECK_SHOW_USAGE();

	memset(bssid, 0, sizeof(bssid));
	b = get_pap_bssid(atoi(argv[1]), bssid, sizeof(bssid));
	if (b && strlen(b) > 0)
		SET_OUTMSG("%s", b);
	else
		SET_OUTMSG("FAIL");

	return (b != NULL && strlen(b) > 0) ? 1 : 0;
}

int tc_get_pap_rssi(int argc, char *argv[], char *outmsg)
{
	int result = 0;

	TC_WLUNIT_CHECK_SHOW_USAGE();	
	result = get_pap_rssi(atoi(argv[1]));

	if (result == 0)
		SET_OUTMSG("FAIL");
	else
		SET_OUTMSG("%d", result);

	return (result == 0) ? 0 : 1; 
}

int tc_get_sta_mac(int argc, char *argv[], char *outmsg)
{	
	char *mac = NULL;

	TC_WLUNIT_CHECK_SHOW_USAGE();		
	mac = get_sta_mac(atoi(argv[1]));

	if (mac && strlen(mac) > 0)
		SET_OUTMSG("%s", mac);
	else
		SET_OUTMSG("FAIL");

	return (mac != NULL && strlen(mac)) ? 1 : 0;
}

int tc_sync_ctrl_channel(int argc, char *argv[], char *outmsg)
{
	int result = 1, ac = 0;
	int unit = 0, channel = 0, bw = 0, nctrlsb = 0;
	int i_channel = 0, i_bw = 0, i_nctrlsb = 0;
	char check_channel = 0, check_bw = 0, check_nctrlsb = 0, **av = NULL, *as = NULL;

	if (argc < 2)
		goto tc_sync_ctrl_channel_usage;

	as = argv[1];
	av = args_parsed(as, &ac, ',');
	if (!av)
	{
		DBG_PRINTF("args_parsed() return fail\n");
		SET_OUTMSG("FAIL");
		return 0;		
	}

	if (ac != 4)
		goto tc_sync_ctrl_channel_usage;

	unit = atoi(av[0]);
	i_channel = atoi(av[1]);
	i_bw = atoi(av[2]);
	i_nctrlsb = atoi(av[3]);
	sync_control_channel(unit, i_channel, i_bw, i_nctrlsb);

	// verify
	sleep(10);
	wl_control_channel(unit, &channel, &bw, &nctrlsb);
	check_channel = (i_channel == channel) ? 1 : 0;
	check_bw = (i_bw == bw) ? 1 : 0;
	check_nctrlsb = (i_nctrlsb == nctrlsb) ? 1 : 0;
	result = (check_channel && check_bw && check_nctrlsb) ? 1 : 0;

	if (!result)
	{
		if (!check_channel)
			DBG_PRINTF("channel not changed.\n");
		if (!check_bw)
			DBG_PRINTF("bw not changed.\n");
		if (!nctrlsb)
			DBG_PRINTF("nctrlsb not changed.\n");
		SET_OUTMSG("FAIL");
	}
	else
	{
		SET_OUTMSG("OK");
	}

	args_parsed_free(av);
	return result;


tc_sync_ctrl_channel_usage:
	SET_OUTMSG(
		"usage: %s unit,channel,bw,nctrlsb\n"\
		"\tunit: 0:2.4g, 1:5g1, 2:5g2\n"\
		"\tchannel: wireless channel\n"\
		"\tbw: channel bandwitch\n"\
		"\tnctrlsb: nctrlsb\n", argv[0]);
	return 0;

#if 0	
	int result = 1;
	int unit = 0, channel = 0, bw = 0, nctrlsb = 0;
	int i_channel = 0, i_bw = 0, i_nctrlsb = 0;
	char check_channel = 0, check_bw = 0, check_nctrlsb = 0;

	TC_SYNCCTRLCH_CHECK_SHOW_USAGE();
	unit = atoi(argv[1]);
	i_channel = atoi(argv[2]);
	i_bw = atoi(argv[3]);
	i_nctrlsb = atoi(argv[4]);
	sync_control_channel(unit, i_channel, i_bw, i_nctrlsb);

	// verify
	sleep(10);
	wl_control_channel(unit, &channel, &bw, &nctrlsb);
	check_channel = (i_channel == channel) ? 1 : 0;
	check_bw = (i_bw == bw) ? 1 : 0;
	check_nctrlsb = (i_nctrlsb == nctrlsb) ? 1 : 0;
	result = (check_channel && check_bw && check_nctrlsb) ? 1 : 0;

	if (!result)
	{
		if (!check_channel)
			DBG_PRINTF("channel not changed.\n");
		if (!check_bw)
			DBG_PRINTF("bw not changed.\n");
		if (!nctrlsb)
			DBG_PRINTF("nctrlsb not changed.\n");
		SET_OUTMSG("FAIL");
	}
	else
	{
		SET_OUTMSG("OK");
	}

	return result;
#endif	
}
