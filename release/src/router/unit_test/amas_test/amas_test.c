/*
**  amas_test.c
**
**
**
*/

#include <rc.h>
#ifdef RTCONFIG_BHCOST_OPT
#include <amas/amas_ssd.h>
#endif 	/* RTCONFIG_BHCOST_OPT */
#include <shared.h>
#include <rtconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sysdeps/amas/amas_path.h>
#include <libgen.h>
#include "amas_test.h"

#define ENABLE_SHOW_OPTS
#define TC_LOGFILE_PATH		"/tmp/unittestlog"
#define TC_LOGFILE_NAME		"%s/%s.log"

static char* prog_name = NULL;
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

#define TC_NO_SYSDEPS() do {\
	SET_OUTMSG("No sysdeps function !!");\
	return 1;\
}while(0)

#define TC_WLUNIT_CHECK_ARGV() do {\
	if (argc < 2)\
	{\
		printf("usage: %s wl_unit\n", argv[0]);\
		printf("\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n");\
		return 0;\
	}\
}while(0)

#define TC_SETWL_SERVICE_CHECK_ARGV() do {\
	if (argc < 2)\
	{\
		printf("usage: %s wl_unit,vif_idx,enable\n", argv[0]);\
		printf("\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n");\
		printf("\tvif_idx: 1,2,3\n");\
		printf("\tenable: 0:disable, 1:enable\n");\
		return 0;\
	}\
}while(0)

#define TC_SET_PRE_POST_CONFIG_CHECK_ARGV() do {\
	if (argc < 2)\
	{\
		printf("usage: %s iftype\n", argv[0]);\
		printf("\tiftype: 0:eth, 1:2.4g, 2:5g1, 3:5g2\n");\
		return 0;\
	}\
}while(0)


#define SSD_IPC_SOCKET_PATH "/etc/ssd_ipc_socket"
int tc_get_amas_ssd_result(int argc, char *argv[], char *outmsg)
{
//amas_ipc -p /etc/ssd_ipc_socket -m 1 -u 1 -e 1 -s 00heidi_ax53u_5G
	int status = 0, ac = 0;
	char **av = NULL, *as = NULL;
	char s[128] = {0}, *ssid = NULL, fpath[128]={0};
	int unit;
	FILE *f = NULL;
	struct stat sb;
	//struct stat st;

	if (argc < 2)
		goto tc_get_amas_ssd_result_usage;

	as = argv[1];
	av = args_parsed(as, &ac, ',');

	unit = atoi(av[0]);
	//printf("unit[%d]\n", unit);

	ssid = av[1];
	//printf("ssid[%s]\n", ssid);
	snprintf(s, sizeof(s), "amas_ipc -p %s -m 1 -u %d -e 1 -s %s",
				SSD_IPC_SOCKET_PATH, unit, ssid);
	printf("s[%s]\n", s);
	system(s);
	sleep(15);
	memset(&sb, 0, sizeof(struct stat));

	snprintf(fpath, sizeof(fpath), "/tmp/amas/survey_result_%d", unit);
	if (!(f = fopen(fpath, "r")))
		goto exit;

    if (stat(fpath, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
	char *file_contents = malloc(sb.st_size);
	memset(file_contents, 0, sizeof(file_contents));
    fread(file_contents, sb.st_size, 1, f);
	//SET_OUTMSG("%s:%s\n", fpath, file_contents);
	if (strstr(file_contents, "{}") !=NULL)
		status = 0;
	else
		status = 1;

	if (status == 1)
		SET_OUTMSG("OK\n%s: %s\n", fpath, file_contents);
	else
		SET_OUTMSG("FAIL\n");

	free(file_contents);
	fclose(f);
	args_parsed_free(av);

	return (status == 0) ? 0 : 1;

tc_get_amas_ssd_result_usage:
	SET_OUTMSG(
		"usage: %s wl_unit,ssid\n"\
		"\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n"\
		"\tssid: 1,2,3\n", argv[0]);
	return 0;

exit:
	return 0;
}

int tc_Pty_get_wlc_status(int argc, char *argv[], char *outmsg)
{
#if !defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)
	char nv_ifname[80];
	int status = -1;

	TC_WLUNIT_CHECK_ARGV();
	memset(nv_ifname, 0, sizeof(nv_ifname));
	snprintf(nv_ifname, sizeof(nv_ifname), "wl%d_ifname", atoi(argv[1]));
#if defined(RTCONFIG_QCA) || defined(RTCONFIG_RALINK)
	status = get_psta_status(atoi(argv[1]));
#else
	status = Pty_get_wlc_status(nvram_safe_get(nv_ifname));
#endif
	if (status == -1)
		SET_OUTMSG("FAIL");
	else
	{
		switch (status)
		{
			case WLC_STATE_CONNECTED:
				SET_OUTMSG("WLC_STATE_CONNECTED");
				break;
			case WLC_STATE_CONNECTING:
				SET_OUTMSG("WLC_STATE_CONNECTING");
				break;
			case WLC_STATE_STOPPED:
				SET_OUTMSG("WLC_STATE_STOPPED");
				break;
			case WLC_STATE_INITIALIZING:
				SET_OUTMSG("WLC_STATE_INITIALIZING");
				break;
			default:
				SET_OUTMSG("UNKNOWN");
				break;
		}
	}

	return (status == -1) ? 0 : 1;
#else 	// !defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)
	TC_NO_SYSDEPS();
#endif	//!defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)	
}

int tc_Pty_start_wlc_connect(int argc, char *argv[], char *outmsg)
{
	char nv_ifname[80], nv_wlx_ssid[80];
	int status = -1;
	int result = 0;
	int ac = 0;
	char **av = NULL, *as = NULL;
	TC_WLUNIT_CHECK_ARGV();
	as = argv[1];
	av = args_parsed(as, &ac, ',');
	memset(nv_ifname, 0, sizeof(nv_ifname));
	snprintf(nv_ifname, sizeof(nv_ifname), "wl%d_ifname", atoi(av[0]));

	memset(nv_wlx_ssid, 0, sizeof(nv_wlx_ssid));
	snprintf(nv_wlx_ssid, sizeof(nv_wlx_ssid), "wlc%d_ssid", atoi(av[0]));
	nvram_set(nv_wlx_ssid, av[1]);
#ifdef RTCONFIG_BHCOST_OPT
	Pty_start_wlc_connect(atoi(av[0]), av[2]);
#else
	Pty_start_wlc_connect(atoi(av[0]));
#endif
	sleep(30);
#if !defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)
#if defined(RTCONFIG_QCA) || defined(RTCONFIG_RALINK)
	status = get_psta_status(atoi(av[0]));
#else
	status = Pty_get_wlc_status(nvram_safe_get(nv_ifname));
#endif
	result = (status == WLC_STATE_CONNECTED) ? 1 : 0;
#else 	// !defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)
	status = get_wlc_status(nvram_safe_get(nv_ifname));
	result = (status == WLC_STATE_CONNECTED) ? 1 : 0;
#endif	//!defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)	
	SET_OUTMSG("%s", (result==1) ? "OK" : "FAIL");
	args_parsed_free(av);
	return result;
}

int tc_Pty_stop_wlc_connect(int argc, char *argv[], char *outmsg)
{
	char nv_ifname[80];
	int status = -1;
	int result = 0;

	TC_WLUNIT_CHECK_ARGV();

	system("killall check_watchdog");
	system("killall watchdog");
	system("killall amas_wlcconnect");
	system("killall amas_misc");
	system("killall amas_lanctrl");
	system("killall amas_status");
	system("killall amas_bhctrl");
	system("killall amas_ssd");
	system("killall cfg_client");

	memset(nv_ifname, 0, sizeof(nv_ifname));
	snprintf(nv_ifname, sizeof(nv_ifname), "wl%d_ifname", atoi(argv[1]));
	Pty_stop_wlc_connect(atoi(argv[1]));
#if !defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)
#if defined(RTCONFIG_QCA) || defined(RTCONFIG_RALINK)
	status = get_psta_status(atoi(argv[1]));
#else
	status = Pty_get_wlc_status(nvram_safe_get(nv_ifname));
#endif
	result = (status == WLC_STATE_INITIALIZING) ? 1 : 0;
#else 	// !defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)
	status = get_wlc_status(nvram_safe_get(nv_ifname));
	result = (status == 0) ? 1 : 0;
#endif	//!defined (RTCONFIG_BCMWL6) && !defined(HND_ROUTER)	
	SET_OUTMSG("%s", (result==1) ? "OK" : "FAIL");
	return result;
}

int tc_Pty_get_upstream_rssi(int argc, char *argv[], char *outmsg)
{
	int rssi = 0;

	TC_WLUNIT_CHECK_ARGV();
	if ((rssi = Pty_get_upstream_rssi(atoi(argv[1]))) == 0)
		SET_OUTMSG("FAIL");
	else
		SET_OUTMSG("%d", rssi);

	return (rssi == 0) ? 0 : 1;
}

int tc_set_pre_sysdep_config(int argc, char *argv[], char *outmsg)
{
	int type = 0;

	TC_SET_PRE_POST_CONFIG_CHECK_ARGV();
	switch (atoi(argv[1]))
	{
		case 0:
			type = ETH;
			break;
		case 1:
			type = WL_2G;
			break;
		case 2:
			type = WL_5G;
			break;
		case 3:
			type = WL_5G_1;
			break;
		default:
			DBG_PRINTF("unknown type\n");
			SET_OUTMSG("FAIL");
			return 0;
	}

	//set_pre_sysdep_config(type);
	SET_OUTMSG("OK");
	return 1;
}

int tc_set_post_sysdep_config(int argc, char *argv[], char *outmsg)
{
	int type = 0;

	TC_SET_PRE_POST_CONFIG_CHECK_ARGV();
	switch (atoi(argv[1]))
	{
		case 0:
			type = ETH;
			break;
		case 1:
			type = WL_2G;
			break;
		case 2:
			type = WL_5G;
			break;
		case 3:
			type = WL_5G_1;
			break;
		default:
			DBG_PRINTF("unknown type\n");
			SET_OUTMSG("FAIL");
			return 0;
	}

	//set_post_sysdep_config(type);
	SET_OUTMSG("OK");
	return 1;
}

int tc_get_radar_status(int argc, char *argv[], char *outmsg)
{
	int status = 0;

	TC_WLUNIT_CHECK_ARGV();
	status = get_radar_status(atoi(argv[1]));
	SET_OUTMSG("%d", status);
	return 1;
}

int verify_pty_procedure_check(int unit, char *check_val)
{
#define DFS_CH_FORCED_RESULT_FNAME	"/tmp/dfs_ch_forced.txt"

	char get_channel_forced[] = {"wl -i %s dfs_channel_forced >%s"};
	FILE *f = NULL;
	char s[128], b[1024], word[64], *next = NULL, *ifname = NULL;
	int result = 1;
 
	if (unit < 0)
		goto verify_pty_procedure_check_fail;

	if (!check_val || strlen(check_val) <= 0)
		goto verify_pty_procedure_check_fail;

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wl%d_ifname", unit);
	ifname = nvram_safe_get(s);
	if (!ifname || strlen(ifname) <= 0)
		goto verify_pty_procedure_check_fail;

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), get_channel_forced, ifname, DFS_CH_FORCED_RESULT_FNAME);
	system(s);

	if (!(f = fopen(DFS_CH_FORCED_RESULT_FNAME, "r")))
		goto verify_pty_procedure_check_fail;

	memset(b, 0, sizeof(b));
	if (fread(b, 1, sizeof(b), f) <= 0)
		goto verify_pty_procedure_check_fail;

	foreach(word, check_val, next)
	{
		if (!strstr(b, word))
		{
			result = 0;
			break;
		}
	}

	fclose(f);
	return result;

verify_pty_procedure_check_fail:
	if (f) fclose(f);
	return 0;
}

int tc_pty_procedure_check(int argc, char *argv[], char *outmsg)
{

#ifdef RTCONFIG_BCMARM
	char dfs_channel_forced[] = {"112/80 136u 108l 100l 140 132 116\0"};
	int i, wl_count = get_wl_count(), result = 1;
	char prefix[] = "wlXXXXXXXXXX_", tmp[NVRAM_MAX_PARAM_LEN];

	if (wl_count == 2 && nvram_match(strcat_r(prefix, "reg_mode", tmp), "h"))
	{
		for (i=0, wl_count=get_wl_count(); i<wl_count; i++)
		{
			if (!is_psta(i) && !is_psr(i))
				continue;

			DBG_PRINTF("Pty_procedure_check(unit=%d, wl_count=%d)\n", i, wl_count);
			if (Pty_procedure_check(i, wl_count) < 0)
			{
				result = 0;
				DBG_PRINTF("Pty_procedure_check() failure\n");
				break;
			}

			if (result == 1)
			{
				DBG_PRINTF("verify_pty_procedure_check(unit=%d, dfs_channel_forced=%s)\n", i, dfs_channel_forced);
				if (!verify_pty_procedure_check(i, dfs_channel_forced))
				{
					result = 0;
					DBG_PRINTF("verify_pty_procedure_check() failure\n");
					break;
				}
			}
		}
	}

	if (result == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
#else
	SET_OUTMSG("OK");
	return 1;
#endif	
}

int tc_amas_wait_wifi_ready(int argc, char *argv[], char *outmsg)
{
	amas_wait_wifi_ready();
	SET_OUTMSG("OK");
	return 1;
}

int tc_get_wlan_service_status(int argc, char *argv[], char *outmsg)
{
	int status = 0, ac = 0;
	char **av = NULL, *as = NULL;

	if (argc < 2)
		goto tc_get_wlan_service_status_usage;

	as = argv[1];
	av = args_parsed(as, &ac, ',');
	if (!av)
	{
		DBG_PRINTF("args_parsed() return fail\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	if (ac != 2)
		goto tc_get_wlan_service_status_usage;

	status = get_wlan_service_status(atoi(av[0]), atoi(av[1]));
	if (status == -1)
		SET_OUTMSG("FAIL");
	else
		SET_OUTMSG("%d", status);

	args_parsed_free(av);
	return (status == -1) ? 0 : 1;

tc_get_wlan_service_status_usage:
	SET_OUTMSG(
		"usage: %s wl_unit,vif_idx\n"\
		"\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n"\
		"\tvif_idx: 1,2,3\n", argv[0]);
	return 0;
}

int tc_set_wlan_service_status(int argc, char *argv[], char *outmsg)
{
	int ac = 0, unit = 0, subunit = 0, enable = 0, status = 0, result = 0;
	char **av = NULL, *as = NULL;

	if (argc < 2)
		goto tc_set_wlan_service_status_usage;

	as = argv[1];
	av = args_parsed(as, &ac, ',');
	if (!av)
	{
		DBG_PRINTF("args_parsed() return fail\n");
		SET_OUTMSG("FAIL");
		return 0;		
	}

	if (ac != 3)
		goto tc_set_wlan_service_status_usage;

	unit = atoi(av[0]);
	subunit = atoi(av[1]);
	enable = (atoi(av[2]) == 1) ? 1 : 0;
	set_wlan_service_status(unit, subunit, enable);

	status = get_wlan_service_status(unit, subunit);
	if (status == -1)
	{
		DBG_PRINTF("get_wlan_service_status() return fail.\n");
		goto tc_set_wlan_service_status_fail;
	}

	DBG_PRINTF("status: %d\n", status);
	DBG_PRINTF("enable: %d\n", enable);
	result = (status == enable) ? 1 : 0;

	if (result)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	args_parsed_free(av);
	return result;

tc_set_wlan_service_status_fail:
	if (av) args_parsed_free(av);
	SET_OUTMSG("FAIL");
	return 0;

tc_set_wlan_service_status_usage:
	SET_OUTMSG(
		"usage: %s wl_unit,vif_idx,enable\n"\
		"\twl_unit: 0:2.4g, 1:5g1, 2:5g2\n"\
		"\tvif_idx: 1,2,3\n"\
		"\tenable: 0:disable, 1:enable\n", argv[0]);
	return 0;
}

int tc_no_need_obd(int argc, char *argv[], char *outmsg)
{
	int result = 0, ret = 0;

	ret = no_need_obd();
	if (nvram_match("x_Setting", "0"))
		result = (ret > 0) ? 1 : 0;
	else
		result = (ret == -1) ? 1 : 0;

	if (result == 1)
		SET_OUTMSG("OK");
	else 
		SET_OUTMSG("FAIL");

	return result;
}

int tc_no_need_obdeth(int argc, char *argv[], char *outmsg)
{
	int result = 0, ret = 0;

	ret = no_need_obdeth();
	if (nvram_match("x_Setting", "0"))
		result = (ret > 0) ? 1 : 0;
	else
		result = (ret == -1) ? 1 : 0;

	if (result == 1)
		SET_OUTMSG("OK");
	else 
		SET_OUTMSG("FAIL");

	return result;

}

int tc_apply_config_to_driver(int argc, char *argv[], char *outmsg)
{
#ifdef RTCONFIG_RALINK
	SET_OUTMSG("TBD");
	return 1;
#endif 	/* RTCONFIG_RALINK */	
// sta_priority=2 0 5 1 5 1 4 0 5 2 3 1
struct wl_info_t {
	char use;
	char *ifname;
	char wl_ssid[1024];
	char wl_wpapsk[1024];
	char wlc_ssid[1024];
	char wlc_wpapsk[1024];
};
	FILE *fp = NULL;
	size_t alloc_size = 2048;
	int i, x = 0, waitfor = 60, ifidx = 0;
	struct wl_info_t wl_info[3];
	char *nv_sta_priority = NULL, s[132], *p = NULL, *b = NULL, tc_ssid[] = {"tc_ssid_test\0"};

	if (argc > 2)
		waitfor = atoi(argv[1]);

	memset(wl_info, 0, sizeof(struct wl_info_t) * 3);
	nv_sta_priority = nvram_safe_get("sta_priority");
	for (i=0, p=nv_sta_priority; i<strlen(nv_sta_priority); i++, p++)
	{
		switch(i)
		{
			case 0:
			case 8:
			case 16:
				if ((i+2) >= strlen(nv_sta_priority) || (i+6) >= strlen(nv_sta_priority))
					break;
				if (atoi(p) != 2 && atoi(p) != 5)
					break;
				if (atoi(p+6) != 1)
					break;
				if (atoi(p) == 2)
				{
					wl_info[0].use = 1;
					wl_info[0].ifname = nvram_safe_get("wl0_ifname");
					strlcpy(wl_info[0].wl_ssid, nvram_safe_get("wl0_ssid"), sizeof(wl_info[0].wl_ssid));
					strlcpy(wl_info[0].wl_wpapsk, nvram_safe_get("wl0_wpa_psk"), sizeof(wl_info[0].wl_wpapsk));
					strlcpy(wl_info[0].wlc_ssid, nvram_safe_get("wlc0_ssid"), sizeof(wl_info[0].wlc_ssid));
					strlcpy(wl_info[0].wlc_wpapsk, nvram_safe_get("wlc0_wpa_psk"), sizeof(wl_info[0].wlc_wpapsk));
				}
				else
				{
					if (atoi(p+2) == 1 || atoi(p+2) == 2)
					{
						x = atoi(p+2);
						wl_info[x].use = 1;
						memset(s, 0, sizeof(s));
						snprintf(s, sizeof(s), "wl%d_ifname", x);
						wl_info[x].ifname = nvram_safe_get(s);
						memset(s, 0, sizeof(s));
						snprintf(s, sizeof(s), "wl%d_ssid", x);
						strlcpy(wl_info[x].wl_ssid, nvram_safe_get(s), sizeof(wl_info[x].wl_ssid));
						memset(s, 0, sizeof(s));
						snprintf(s, sizeof(s), "wl%d_wpa_psk", x);
						strlcpy(wl_info[x].wl_wpapsk, nvram_safe_get(s), sizeof(wl_info[x].wl_wpapsk));
						memset(s, 0, sizeof(s));
						snprintf(s, sizeof(s), "wlc%d_ssid", x);
						strlcpy(wl_info[x].wlc_ssid, nvram_safe_get(s), sizeof(wl_info[x].wlc_ssid));
						memset(s, 0, sizeof(s));
						snprintf(s, sizeof(s), "wlc%d_wpa_psk", x);
						strlcpy(wl_info[x].wlc_wpapsk, nvram_safe_get(s), sizeof(wl_info[x].wlc_wpapsk));
						ifidx = x;
					}	
				}
				break;
		}
	}

	if (ifidx > 0)
	{
		system("killall check_watchdog");
		system("killall watchdog");
		system("killall amas_wlcconnect");
	}
	else
	{
		goto tc_apply_config_to_driver_fail;
	}

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wl%d_ssid", ifidx);
	nvram_set(s, tc_ssid);
	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wl%d_wpa_psk", ifidx);
	nvram_set(s, "00000000");

	snprintf(s, sizeof(s), "wlc%d_ssid", ifidx);
	nvram_set(s, tc_ssid);
	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wlc%d_wpa_psk", ifidx);
	nvram_set(s, "00000000");
	nvram_commit();

	Pty_stop_wlc_connect(ifidx);
#ifdef RTCONFIG_BHCOST_OPT
	apply_config_to_driver(ifidx);
	Pty_start_wlc_connect(ifidx, NULL);
#else
	apply_config_to_driver();
	Pty_start_wlc_connect(ifidx);
#endif
	sleep(waitfor);

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wl%d_ssid", ifidx);
	nvram_set(s, wl_info[ifidx].wl_ssid);
	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wl%d_wpa_psk", ifidx);
	nvram_set(s, wl_info[ifidx].wl_wpapsk);

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wlc%d_ssid", ifidx);
	nvram_set(s, wl_info[ifidx].wlc_ssid);
	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wlc%d_wpa_psk", ifidx);
	nvram_set(s, wl_info[ifidx].wlc_wpapsk);	
	nvram_commit();

	Pty_stop_wlc_connect(ifidx);
#ifdef RTCONFIG_BHCOST_OPT
	apply_config_to_driver(ifidx);
	Pty_start_wlc_connect(ifidx, NULL);
#else
	apply_config_to_driver();
	Pty_start_wlc_connect(ifidx);
#endif
	sleep(waitfor);		

	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "wl -i %s ssid", wl_info[ifidx].ifname);
	if (!(fp = popen(s, "r")))
	{
		DBG_PRINTF("popen() error !!\n");
		goto tc_apply_config_to_driver_fail;
	}

	if (!(b = malloc(alloc_size))) 
	{
		DBG_PRINTF("malloc() error !!\n");
		goto tc_apply_config_to_driver_fail;
	}

	memset(b, 0, alloc_size);
	if (fread(b, 1, alloc_size, fp) <= 0)
	{
		DBG_PRINTF("fread() error !!\n");
		goto tc_apply_config_to_driver_fail;
	}

	if (!strstr(b, wl_info[ifidx].wlc_ssid))
		goto tc_apply_config_to_driver_fail;

	free(b);
	pclose(fp);

	SET_OUTMSG("OK");
	return 1;

tc_apply_config_to_driver_fail:
	if (b) free(b);
	if (fp) pclose(fp);
	SET_OUTMSG("FAIL");
	return 0;
}



#if 0
int tc_apply_config_to_driver(int argc, char *argv[], char *outmsg)
{
#ifdef RTCONFIG_RALINK
	SET_OUTMSG("TBD");
	return 1;
#else	/* RTCONFIG_RALINK */
 	pid_t *pidlist;
  	pid_t *pl;
  	pid_t p1 = -1, p2 = -1;

#ifdef RTCONFIG_BRCM_HOSTAP
  	pidlist = find_pid_by_name("wpa_supplicant-2.7");
#else 	// RTCONFIG_BRCM_HOSTAPD  	
  	pidlist = find_pid_by_name("nas");
#endif 	// RTCONFIG_BRCM_HOSTAPD  	
  	
  	for (pl = pidlist; *pl; pl++)
  	{
  		p1 = *pl;
  		break;
  	}
  	free(pidlist);
  
  	if (p1 == -1)
  	{
  		DBG_PRINTF("p1 pid failed\n");
  		goto tc_apply_config_to_driver_fail;
  	}

	apply_config_to_driver();
 
 #ifdef RTCONFIG_BRCM_HOSTAP
  	pidlist = find_pid_by_name("wpa_supplicant-2.7");
#else 	// RTCONFIG_BRCM_HOSTAPD  	
  	pidlist = find_pid_by_name("nas");
#endif 	// RTCONFIG_BRCM_HOSTAPD  	
 	for (pl = pidlist; *pl; pl++)
  	{
  		p2 = *pl;
  		break;
  	}
  	free(pidlist);
  
  	if (p2 == -1)
  	{
  		DBG_PRINTF("p2 pid failed\n");
  		goto tc_apply_config_to_driver_fail;
  	}

  	if (p1 == p2)
  		SET_OUTMSG("FAIL");
  	else
  		SET_OUTMSG("OK");

  	return (p1 == p2) ? 0 : 1;
	
tc_apply_config_to_driver_fail:
	SET_OUTMSG("FAIL");
	return 0;
#endif	/* RTCONFIG_RALINK */	
}
#endif
