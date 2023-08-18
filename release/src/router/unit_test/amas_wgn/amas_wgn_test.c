/*
**  amas_wgn_test.c
**
**
**
*/
#include <rc.h>
#include <shared.h>
#include <rtconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>
#include "amas_wgn_test.h"

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

int verify_robocfg(int vid)
{
#define ROBOCFG_RESULT_FORMAT	"%d: vlan%d: %s"	
#define ROBOCFG_RESULT_FNAME	"/tmp/robocfg.txt"
	char s[128], b[1024], tmp[81];
	int gmac3_enable = 0;
	FILE *f = NULL;

#ifdef RTCONFIG_GMAC3
	gmac3_enable = nvram_get_int("gmac3_enable");
#endif	/* RTCONFIG_GMAC3 */		

	memset(s, 0, sizeof(s));
	switch (get_model()) 
	{
		/* P0  P1 P2 P3 P4 P5 */
		/* WAN L1 L2 L3 L4 CPU */
		case MODEL_RTAC68U: 
			snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "0t 1t 2t 3t 4t 5t");
			break;

		case MODEL_RTAC5300:
			/* If enable gmac3, CPU port is 8 */
			if (gmac3_enable)
			{
#ifdef RTCONFIG_EXT_RTL8365MB
				/* P0  P1 P2 P3 P4 P5 		P7 */
				/* WAN L1 L2 L3 L4 L5 L6 L7 L8 	CPU*/
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "0t 1t 2t 3t 4t 5t 7t 8t");
#else	/* RTCONFIG_EXT_RTL8365MB */
				/* P0  P1 P2 P3 P4 P5 */
				/* WAN L1 L2 L3 L4 CPU*/
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "0t 1t 2t 3t 4t 7t 8t");
#endif	/* RTCONFIG_EXT_RTL8365MB */		
			}
			else
			{
#ifdef RTCONFIG_EXT_RTL8365MB
				/* P0  P1 P2 P3 P4 P5 		P7 */
				/* WAN L1 L2 L3 L4 L5 L6 L7 L8 	CPU*/
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "0t 1t 2t 3t 4t 5t 7t");
#else	/* RTCONFIG_EXT_RTL8365MB */
				/* P0  P1 P2 P3 P4 P5 */
				/* WAN L1 L2 L3 L4 CPU*/
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "0t 1t 2t 3t 4t 7t");
#endif	/* RTCONFIG_EXT_RTL8365MB */		
			}
			break;

		case MODEL_RTAC3100:
		case MODEL_RTAC88U:
			/* If enable gmac3, CPU port is 8 */
#ifdef RTCONFIG_RGMII_BRCM5301X
			/* P4  P3 P2 P1 P0 P5 		P7 */
			/* WAN L1 L2 L3 L4 L5 L6 L7 L8 	CPU*/
#else	/* RTCONFIG_RGMII_BRCM5301X */
			/* P4  P3 P2 P1 P0 P5 */
			/* WAN L1 L2 L3 L4 CPU*/
#endif	/* RTCONFIG_RGMII_BRCM5301X */
			
			if (gmac3_enable) 
			{
#ifdef RTCONFIG_RGMII_BRCM5301X
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "4t 3t 2t 1t 0t 5t 7t 8t");	
#else	/* RTCONFIG_RGMII_BRCM5301X */
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "4t 3t 2t 1t 0t 5t 8t");	
#endif	/* RTCONFIG_RGMII_BRCM5301X */
			}
			else 
			{
#ifdef RTCONFIG_RGMII_BRCM5301X			
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "4t 3t 2t 1t 0t 5t 7t");	
#else	/* RTCONFIG_RGMII_BRCM5301X */
				snprintf(s, sizeof(s), ROBOCFG_RESULT_FORMAT, vid, vid, "4t 3t 2t 1t 0t 5t");	
#endif	/* RTCONFIG_RGMII_BRCM5301X */
			}
			break;
	}

	if (strlen(s) <= 0)
		return -1;

	memset(tmp, 0, sizeof(tmp));
	snprintf(tmp, sizeof(tmp), "robocfg show > %s", ROBOCFG_RESULT_FNAME);
	system(tmp);

	if (!(f = fopen(ROBOCFG_RESULT_FNAME, "r")))
		return -1;

	memset(b, 0, sizeof(b));
	if (fread(b, 1, sizeof(b), f) <= 0)
	{
		fclose(f);
		return -1;
	}

	return (strstr(b, s)) ? 1 : 0;
}

int tc_switch_set(int argc, char *argv[], char *outmsg)
{
	int vid = 0, result = 0;

	if (argc > 0)
		vid = atoi(argv[1]);

	if (vid > 0)
	{
		wgn_sysdep_swtich_set(vid);
		result = (verify_robocfg(vid) == 1) ? 1 : 0;
	}

	if (result==1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result; 
}

int tc_switch_unset(int argc, char *argv[], char *outmsg)
{
	int vid = 0, result = 0;

	if (argc > 0)
		vid = atoi(argv[1]);

	if (vid > 0)
	{
		wgn_sysdep_swtich_unset(vid);
		result =  (verify_robocfg(vid) == 0) ? 1 : 0;		
	}

	if (result==1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
}


