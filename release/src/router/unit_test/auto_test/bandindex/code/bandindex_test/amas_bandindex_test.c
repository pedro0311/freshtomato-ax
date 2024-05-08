/*
**  amas_bandindex_test.c
**
**
**
*/
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sysinfo.h>
#include <sys/un.h>
#include <shared.h>
#include <shutils.h>
#include <bcmnvram.h>
#ifdef RTCONFIG_ADV_RAST
#include <sys/un.h>
#endif
#include "encrypt_main.h"
#include "cfg_crc.h"
#include "cfg_common.h"
#include "cfg_param.h"
#include "cfg_mnt.h"
#include "cfg_slavelist.h"
#include "cfg_ipc.h"
#include "cfg_udp.h"
#include "cfg_wevent.h"
#include "cfg_upgrade.h"
#include "cfg_dencrypt.h"
#if defined(RTCONFIG_TCODE) && defined(RTCONFIG_CFGSYNC_LOCSYNC)
#include "cfg_loclist.h"	//cm_transloclist_Parameter()
#endif
/* init sta roaming check */
#ifdef LEGACY_ROAMING
#include "cfg_roaming.h"
#endif
#ifdef ROAMING_INFO
#include "cfg_roaminginfo.h"
#endif
#include "cfg_clientlist.h"
#include "cfg_event.h"
#ifdef ONBOARDING
#include "cfg_onboarding.h"
#endif
#ifdef RADAR_DET
#include "cfg_radardet.h"
#include "chmgmt.h"
#endif	/* RADAR_DET */

#include <wlioctl.h>
#include <wlutils.h>

#ifdef RTCONFIG_AMAS
#include <amas_path.h>
#else
#include <fcntl.h>
#endif
#include "cfg_sched.h"
#include "cfg_eventnotify.h"

#ifdef RTCONFIG_DWB
#include "cfg_dwb.h"
#endif

#include "cfg_chanspec.h"
#include "cfg_parammgnt.h"
#ifdef DUAL_BAND_DETECTION
#include "cfg_dualbandlist.h"
#endif
#include "cfg_capability.h"

#if defined(RTCONFIG_AMAS_WGN)
#include "cfg_capability.h"
#endif	// RTCONFIG_AMAS_WGN

#include "cfg_action.h"
#ifdef PRELINK
#include "cfg_prelink.h"
#endif

#ifdef RTCONFIG_NBR_RPT
#include "cfg_nbr_rpt.h"
#endif

#include "cfg_ctrllist.h"

#ifdef CONN_DIAG
#include "cfg_conndiag.h"
#endif

#include "cfg_bandindex.h"

#ifdef RTCONFIG_AMAS_CENTRAL_CONTROL
#include "cfg_centralcontrol.h"
#endif

#ifdef RTCONFIG_FRONTHAUL_DWB
#ifdef RTCONFIG_AMAS_ADTBW
#include "cfg_lib.h"
#endif
#endif
#include "amas_bandindex_test.h"

#define ENABLE_SHOW_OPTS
#define ENABLE_TC_LOGFILE
#define TC_LOGFILE_PATH		"/tmp/unittestlog"
#define TC_LOGFILE_NAME		"%s/%s.log"
#define TC_LOGFILE_NAME_FOR_APP	"%s/%s.json"

#define CAP_RESULT_OUTROOT_PATH    "/tmp/cap_outroot.json"
#define TESTCASE_RESULT_OUTROOT_PATH    "%s/testcase_%s_outroot.json"

static char* prog_name = NULL;
static int tc_logfile = 0;
static char logfname[132];
static char* case_name = NULL;
static char* test_title = NULL;
static char testcase_result_logfname[132];
static char testcase_fwver[132];

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
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
		for (found=0, tc=testcases; tc->name; ++tc)
		{
			DBG_PRINTF("(%s,%d)tc->name=%s\n",__func__,__LINE__,tc->name);
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
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
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
	json_object *outRoot_for_app_result = NULL;
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
				
		if (test_title){			
			TC_PRINTF("Title: %s\n", test_title);	
		}	
		TC_PRINTF("Test Module: %s\n", prog_name);
		TC_PRINTF("LOG File: %s\n", logfname);

#ifdef ENABLE_SHOW_OPTS
		TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
		if (tc_options != 0)
		{
			TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
			TC_PRINTF("Options:\n");
			if ((tc_options & TC_OPT_FAIL_STOP) == TC_OPT_FAIL_STOP)
				TC_PRINTF("\t1: enable termination test-case when test-case fails\n");
			if ((tc_options & TC_OPT_FAIL_STOP_ALL) == TC_OPT_FAIL_STOP_ALL)
				TC_PRINTF("\t2: enable termination all test-cases when the test-case fails\n");
		} 
		TC_PRINTF("(%s,%d)\n", __func__,__LINE__);	
#endif	/* ENABLE_SHOW_OPTS */		
		TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
#ifdef ENABLE_SHOW_ARGS	
		TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
		memset(s, 0, sizeof(s));
		TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
		for (i=1; i<argc; i++)
		{
			strlcat(s, argv[i], sizeof(s));
			strlcat(s, " ", sizeof(s));
		}
		TC_PRINTF("Args: %s\n", s);
#endif	/* ENABLE_ARGS_SHOW */
		TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
		(void)testcase_start(argc, argv);
		TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
		for (i=0; i<tc_count && !tc_stop; i++)
		{
			TC_PRINTF("\n");
			case_name = tc_param[i].tc->name;
			memset(s1, 0, sizeof(s1));
			// print test-case
			memset(s2, 0, sizeof(s2));
			TC_PRINTF("(%s,%d)*\n", __func__,__LINE__);
			TC_PRINTF("(%s,%d),tc_param[i].tc->name=%s,tc_param[i].test_cycle=%d\n", __func__,__LINE__,tc_param[i].tc->name,tc_param[i].test_cycle);
			TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
			snprintf(s2, sizeof(s2), "test-case: %s, cycle: %d ", tc_param[i].tc->name, tc_param[i].test_cycle);
			TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
			strlcat(s1, s2, sizeof(s1));
			TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
			json_object *outRoot_for_app = NULL;
			outRoot_for_app = json_object_new_object();
			TC_PRINTF("(%s,%d)outRoot_for_app=%d\n", __func__,__LINE__,outRoot_for_app);
			if(outRoot_for_app){
				TC_PRINTF("(%s,%d),ccc=%s\n", __func__,__LINE__,test_title==NULL?"":test_title );
				json_object_object_add(outRoot_for_app, "TEST_CASE_TITLE",json_object_new_string(test_title==NULL?"":test_title));			
				TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
				json_object_object_add(outRoot_for_app, "TEST_CASE_NAME",json_object_new_string(tc_param[i].tc->name));
				TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
				json_object_object_add(outRoot_for_app, "TEST_Model",json_object_new_string( nvram_safe_get("model")));
				TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
				json_object_object_add(outRoot_for_app, "TEST_FWVER",json_object_new_string(testcase_fwver));
				TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
				if(outRoot_for_app)
				{
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					outRoot_for_app_result = json_object_new_object();
				}
			}
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
			DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
			TC_PRINTF("%s,tc_param[i].test_cycle=%d\n", s1,tc_param[i].test_cycle);
			for (j=0; j<tc_param[i].test_cycle; j++)
			{
				DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
				memset(tc_param[i].outmsg, 0, MAX_OUTMSG_BUFFER_SIZE);
				result = tc_param[i].tc->fn(tc_param[i].argc, tc_param[i].argv, &tc_param[i].outmsg[0]);
				DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
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
			TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
			if(outRoot_for_app)
			{
				TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
				if(outRoot_for_app_result){
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					json_object_object_add(outRoot_for_app_result, "cycle",json_object_new_int(tc_param[i].test_cycle));
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					json_object_object_add(outRoot_for_app_result, "pass",json_object_new_int(tc_param[i].test_passed));
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					json_object_object_add(outRoot_for_app_result, "fail",json_object_new_int(tc_param[i].test_failed));
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					json_object_object_add(outRoot_for_app_result, "Description",json_object_new_string(tc_param[i].outmsg));
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					json_object_object_add(outRoot_for_app, "TEST_CASE_RESULT",outRoot_for_app_result);
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					memset(testcase_result_logfname, 0, sizeof(testcase_result_logfname));
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					snprintf(testcase_result_logfname, sizeof(testcase_result_logfname), TESTCASE_RESULT_OUTROOT_PATH, TC_LOGFILE_PATH, tc_param[i].tc->name);
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					json_object_to_file(testcase_result_logfname, outRoot_for_app);
					TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					if(outRoot_for_app_result){
						TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
						json_object_put(outRoot_for_app_result);
						TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
					}
				TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
				json_object_put(outRoot_for_app);
				TC_PRINTF("(%s,%d)\n", __func__,__LINE__);
				}
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
	memset(testcase_fwver, 0, sizeof(testcase_fwver));
	snprintf(testcase_fwver, sizeof(testcase_fwver), "%s.%s_%s",nvram_safe_get("firmver"), nvram_safe_get("buildno"), nvram_safe_get("extendno"));
	return;
}

void testcase_end(void)
{
	// TODO
	return;
}

struct tc_nvram_st {
	char *k; char *v;
};


int check_re_sync_wireless(json_object *cfgRoot)
{
	json_object *wirelssObj = NULL;
	
	json_object_object_get_ex(cfgRoot, "wireless", &wirelssObj);
	
	int num_mismatches =0;

	if(wirelssObj==NULL)
	{
		DBG_PRINTF("wirelssObj is null\n");
		return 0;
	}
	else{
		json_object_object_foreach(wirelssObj, key, val)
		{
			DBG_PRINTF("key(%s), val(%s)\n", key, json_object_get_string(val));
			if(strncmp(nvram_safe_get(key),json_object_get_string(val),strlen(nvram_safe_get(key))))
			{
				DBG_PRINTF("param %s =%s  not set successfully  , the correct value should be %s\n",key, nvram_safe_get(key), json_object_get_string(val));
				num_mismatches ++;
			}
		}
		if(num_mismatches)
		{
			DBG_PRINTF("re wireless sync fail  %d\n",num_mismatches);
			return 0;
		}
		else{
			DBG_PRINTF("re wireless sync successfully  %d\n");
			return 1;
			}
	}
	

}

int check_cap_sync_wireless(json_object *cfgRoot)
{
	json_object *wirelssObj = NULL;
	json_object_object_get_ex(cfgRoot, "wireless", &wirelssObj);
	
	json_object *needcheckObj = NULL;
	json_object *wirelss_now_Obj = NULL;
	char checkparam_value[256] = {0};

	int num_mismatches =0;

	if ((needcheckObj = json_object_from_file(CAP_RESULT_OUTROOT_PATH)) == NULL) {
			DBG_PRINTF("%s is null\n",CAP_RESULT_OUTROOT_PATH);
			return 0;
	}
	else{
		DBG_PRINTF("needcheckObj is %s\n", json_object_to_json_string(needcheckObj));
		json_object_object_get_ex(needcheckObj, "wireless", &wirelss_now_Obj);
		if(wirelssObj==NULL || wirelss_now_Obj==NULL)
		{
			DBG_PRINTF("wirelssObj is null\n");
			json_object_put(needcheckObj);
			return 0;
		}
		else{
			json_object_object_foreach(wirelssObj, key, val)
			{
				json_object *checkparamObj = NULL;
				DBG_PRINTF("key(%s), val(%s)\n", key, json_object_get_string(val));
				//DBG_PRINTF("(%s,%d),wirelss_now_Obj=%s\n",__func__,__LINE__,json_object_get_string(wirelss_now_Obj));
				json_object_object_get_ex(wirelss_now_Obj, key, &checkparamObj);
                if(strstr(key,"_bsd_if_select_policy")){
                    continue;
                }
				//DBG_PRINTF("(%s,%d),checkparamObj=%s\n",__func__,__LINE__,json_object_get_string(checkparamObj));
				if(checkparamObj){
					DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
					memset(checkparam_value, 0, sizeof(checkparam_value));
					snprintf(checkparam_value,sizeof(checkparam_value),"%s",json_object_get_string(checkparamObj));
					DBG_PRINTF("(%s,%d),checkparam_value=%s\n",__func__,__LINE__,checkparam_value);
				}
				DBG_PRINTF("(%s,%d),checkparam_value=%s,json_object_get_string(val)=%s,strlen(checkparam_value)=%d\n",__func__,__LINE__,checkparam_value,json_object_get_string(val),strlen(checkparam_value));
				if(strncmp(checkparam_value,json_object_get_string(val),strlen(checkparam_value)))
				{
					DBG_PRINTF("param %s =%s  not send successfully  , the correct value should be %s\n",key, nvram_safe_get(key), json_object_get_string(val));
					num_mismatches ++;
				}	
			}
			if(num_mismatches)
			{
				DBG_PRINTF("cap wireless sync set to re fail  %d\n",num_mismatches);
				json_object_put(needcheckObj);
				return 0;
			}
			else{
				DBG_PRINTF("cap wireless sync set to re  successfully  %d\n");
				json_object_put(needcheckObj);
				return 1;
				}
		}
	}
	

}


int check_re_sync_result(int cap_info_filetype, char *cap_json_file_name_dir)
{
	int result =0 , check_wireless_result=0 ,check_action_result=1;
	if(cap_info_filetype==0)
	{
		json_object *cfgRoot = NULL;
		char cap_json_file_name[128] = {0};
		memset(cap_json_file_name, 0, sizeof(cap_json_file_name));
		snprintf(cap_json_file_name,sizeof(cap_json_file_name),"%s/cfg_re_result.json",cap_json_file_name_dir);
		if ((cfgRoot = json_object_from_file(cap_json_file_name)) == NULL) {
			DBG_PRINTF("cfg_re_result.json is null\n");
            DBG_PRINTF("(%s,%d)cap_json_file_name=%s\n",__func__,__LINE__,cap_json_file_name);
			return -1;
		}
		else{
			check_wireless_result=check_re_sync_wireless(cfgRoot);
			//check_action_result = check_sync_action(cfgRoot);
			json_object_put(cfgRoot);
			if(check_wireless_result==0 || check_action_result==0)
			{
				return -1;
			}
		}
	}
	return 1;
}


int check_cap_sync_result(int cap_info_filetype, char *cap_json_file_name_dir)
{
	int result =0 , check_wireless_result=0 ,check_action_result=1;
	if(cap_info_filetype==0)
	{
		json_object *cfgRoot = NULL;
		char cap_json_file_name[128] = {0};
		memset(cap_json_file_name, 0, sizeof(cap_json_file_name));
		snprintf(cap_json_file_name,sizeof(cap_json_file_name),"%s/cfg_cap_result.json",cap_json_file_name_dir);
		DBG_PRINTF("(%s,%d),cap_json_file_name=%s\n",__func__,__LINE__,cap_json_file_name);
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
		if ((cfgRoot = json_object_from_file(cap_json_file_name)) == NULL) {
			DBG_PRINTF("cfg_cap_result is null\n");
			return -1;
		}
		else{
			check_wireless_result=check_cap_sync_wireless(cfgRoot);
			//check_dwb_result = check_dwb_action(cfgRoot);
			//check_action_result = check_sync_action(cfgRoot);
			json_object_put(cfgRoot);
			if(check_wireless_result==0 || check_action_result==0)
			{
				return -1;
			}
		}
	}
	return 1;
}

int test_cm_transformCfgParam(int cap_info_filetype, char *cap_json_file_name_dir,int private)
{
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	json_object *decryptedRoot = NULL;
	json_object *outRoot = NULL;
	json_object *uMacObj = NULL;
	char cap_json_file_name[128] = {0};
	memset(cap_json_file_name, 0, sizeof(cap_json_file_name));
	snprintf(cap_json_file_name,sizeof(cap_json_file_name),"%s/cfg_cap_inRoot.json",cap_json_file_name_dir);
	DBG_PRINTF("(%s,%d),cap_json_file_name=%s\n",__func__,__LINE__,cap_json_file_name);
	if ((decryptedRoot = json_object_from_file(cap_json_file_name)) == NULL) {
			DBG_PRINTF("cfg_cap_inRoot.json is null\n");
			return -1;
	}
	else{
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
		outRoot = json_object_new_object();
		DBG_PRINTF("(%s,%d),outRoot=%s\n",__func__,__LINE__,json_object_to_json_string(outRoot));
		if (!outRoot) {
			json_object_put(decryptedRoot);
			DBG_PRINTF("outRoot is NULL");
			return -1;
		}
		else{
			DBG_PRINTF("will call  cm_transformCfgParam");
			DBG_PRINTF("decryptedRoot is %s**********\n", json_object_to_json_string(decryptedRoot));
			DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
			char uniqueMac[18] = {0};
			
			/* get unique mac for slave */
			json_object_object_get_ex(decryptedRoot, CFG_STR_MAC, &uMacObj);
			if (uMacObj)
				snprintf(uniqueMac, sizeof(uniqueMac), "%s", json_object_get_string(uMacObj));
			cm_transformCfgParam(decryptedRoot, outRoot, private);
			cm_addPrivateCfg(uniqueMac, outRoot);
			
		}
		DBG_PRINTF("(%s,%d)*\n",__func__,__LINE__);
		json_object_put(decryptedRoot);
		DBG_PRINTF("(%s,%d)*\n",__func__,__LINE__);
		json_object_to_file(CAP_RESULT_OUTROOT_PATH, outRoot);
		DBG_PRINTF("(%s,%d)*\n",__func__,__LINE__);
		json_object_put(outRoot);
		DBG_PRINTF("(%s,%d)*\n",__func__,__LINE__);
	}


}

int tc_cm_bandindex_test(int argc, char *argv[], char *outmsg)
{
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	int action = 0, result = 0 ,verify =0;
	char **av = NULL, *as = NULL;
	char cap_json_file_name_dir[128] = {0};
	char sync_type[32] = {0} ,sync_type_x[32] = {0};
	memset(cap_json_file_name_dir, 0, sizeof(cap_json_file_name_dir));
	memset(sync_type, 0, sizeof(sync_type));
	memset(sync_type_x, 0, sizeof(sync_type_x));
	char s[256] = {0};

	
	as = argv[1];
	av = args_parsed(as, &as, ',');
	if (!av)
	{
		DBG_PRINTF("args_parsed() return fail\n");
		SET_OUTMSG("FAIL");
		return 0;		
	}
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	switch(atoi(av[0]))
		{
			case 1:
				snprintf(sync_type,sizeof(sync_type),"CAP");
				DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
				break;
			case 0:
			default:
				snprintf(sync_type,sizeof(sync_type),"RE");
				break;
		}
		
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	switch(atoi(av[1]))
		{
			case 3:
				snprintf(sync_type_x,sizeof(sync_type_x),"QUADBAND");
				break;
			case 2:
				snprintf(sync_type_x,sizeof(sync_type_x),"TRIBAND6G");
				break;
			case 1:
				snprintf(sync_type_x,sizeof(sync_type_x),"TRIBAND");
				break;
			case 0:
			default:
				snprintf(sync_type_x,sizeof(sync_type_x),"DUALBAND");
				break;
		}
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	snprintf(cap_json_file_name_dir,sizeof(cap_json_file_name_dir),"/jffs/%s/%s_v%s",sync_type,sync_type_x,av[2]);
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	nvram_set("cfg_syslog","1");
	nvram_set("cfg_bandindex_auto_test","1");
	memset(s, 0, sizeof(s));
	snprintf(s, sizeof(s), "cp -rf  %s/* /tmp/", cap_json_file_name_dir);
	system(s);
	DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	if(!strncmp(sync_type,"RE",2))
	{
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
		nvram_set("re_mode","1");
		action = atoi(av[3]);	
		cm_applyCfgAction(action);
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
		sleep(15);
		verify = check_re_sync_result(0,cap_json_file_name_dir);
	}
	else if(!strncmp(sync_type,"CAP",3)){
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
		if (f_exists(CAP_RESULT_OUTROOT_PATH)) {
			unlink(CAP_RESULT_OUTROOT_PATH);
		}
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
		test_cm_transformCfgParam(0,cap_json_file_name_dir,atoi(av[3]));
		sleep(5);
		DBG_PRINTF("(%s,%d)cap_json_file_name_dir=%s\n",__func__,__LINE__,cap_json_file_name_dir);
		verify = check_cap_sync_result(0,cap_json_file_name_dir);
		DBG_PRINTF("(%s,%d)\n",__func__,__LINE__);
	}

	if (verify == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	nvram_unset("cfg_syslog");
	nvram_unset("cfg_bandindex_auto_test");
	return 1;	
}


