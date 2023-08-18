/*
**  obd_test.c
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
#include <obd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <libgen.h>
#include "obd_test.h"

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

struct tc_nvram_st {
	char *k; char *v;
};

struct cap_rssi {
	struct ether_addr BSSID;
 	unsigned char RSSI;
};

struct rssi_info_s {
  	unsigned char cap_count;
  	struct cap_rssi caprssi[64];
} rssi_info;

#define NVRAM_VALUE_CHECK(MAP, RESULT) do {\
	struct tc_nvram_st *PTR = &MAP[0];\
	RESULT = 1;\
	if (PTR)\
	{\
		while (PTR->k)\
		{\
			if (PTR->v)\
			{\
				if (!nvram_match(PTR->k, PTR->v))\
				{\
					RESULT = 0;\
					break;\
				}\
			}\
			else\
			{\
				if (nvram_get(PTR->k))\
				{\
					RESULT = 0;\
					break;\
				}\
			}\
			PTR++;\
		}\
	}\
}while(0)

int is_iface_up(
	char *ifname)
{
	int sock;
	struct ifreq ifr;

	if (!ifname)
		return 0;

	if ((sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP)) <= 0)
		return 0;

	memset(&ifr, 0, sizeof(ifr));
	strlcpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0)
		return 0;
	close(sock);
	
	return ((ifr.ifr_flags & IFF_UP) == IFF_UP) ? 1 : 0;
}	


int tc_obd_init(int argc, char *argv[], char *outmsg)
{
#ifdef RTCONFIG_RALINK
	char *ifname = NULL;
	int result = 0;

	if (!(ifname = get_staifname(0)))
	{
		DBG_PRINTF("call get_staifname(0) return NULL ...\n");
		SET_OUTMSG("FAIL");
		return 0;		
	}

	obd_init();
	if (is_iface_up(ifname))
		result = (nvram_match("wl0_vifs", ifname)) ? 1 : 0;
	else
		DBG_PRINTF("%s interface is down ...\n", ifname);

	if (result == 1)
		SET_OUTMSG("OK");
	else	
		SET_OUTMSG("FAIL");

	return result;	

#else	/* RTCONFIG_RALINK */
	obd_init();
	SET_OUTMSG("OK");
	return 1;
#endif	/* RTCONFIG_RALINK */	
}

int tc_obd_final(int argc, char *argv[], char *outmsg)
{
	int result = 1;

#ifdef RTCONFIG_RALINK
	struct tc_nvram_st NVRAM_VERIFY_MAP[] = 
	{
		{	"wps_enrollee", 	NULL 	},
		{	"wps_e_success", 	NULL 	}, 
		{	NULL, 				NULL  	},	
	};	
#else	/* RTCONFIG_RALINK */
	struct tc_nvram_st *NVRAM_VERIFY_MAP = NULL;
#endif	/* RTCONFIG_RALINK */	

	obd_final(NULL);
	NVRAM_VALUE_CHECK(NVRAM_VERIFY_MAP, result);
	if (result == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
}

int tc_obd_save_para(int argc, char *argv[], char *outmsg)
{
	int result = 1;

#ifdef RTCONFIG_RALINK
	struct tc_nvram_st NVRAM_VERIFY_MAP[] = 
	{
		{	"sw_mode", 			"3"		},
		{	"wlc_psta", 		"2"		},
		{	"wlc_dpsta", 		"1"		},
		{	"lan_proto", 		"dhcp"	},
		{	"lan_dnsenable_x", 	"1"		},
		{	"x_Setting", 		"1"		},
		{	"re_mode", 			"1"		},
		{	"cfg_group", 		NULL	},
		{	"wps_enrollee", 	NULL 	},
		{	"wps_e_success", 	NULL 	}, 
		{	NULL, 				NULL  	},	
	};
#else	/* RTCONFIG_RALINK */
	struct tc_nvram_st NVRAM_VERIFY_MAP[] = 
	{
		{	"sw_mode", 			"3"		},
		{	"wlc_psta", 		"2"		},
		{	"wlc_dpsta", 		"1"		},
		{	"lan_proto", 		"dhcp"	},
		{	"lan_dnsenable_x", 	"1"		},
		{	"x_Setting", 		"1"		},
		{	"re_mode", 			"1"		},
		{	"cfg_group", 		NULL	},
		{	NULL, 				NULL  	},	
	};
#endif	/* RTCONFIG_RALINK */

	obd_save_para();
	NVRAM_VALUE_CHECK(NVRAM_VERIFY_MAP, result);
	if (result == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
}

int tc_obd_start_active_scan(int argc, char *argv[], char *outmsg)
{
	obd_start_active_scan();
	SET_OUTMSG("OK");
	return 1;
}

int tc_obd_get_bss_scan_result(int argc, char *argv[], char *outmsg)
{
	int i, count = 0, result = 0;
	struct scanned_bss *bss = NULL, *bss_list = NULL;

	if (!(bss_list = obd_get_bss_scan_result()))
	{
		DBG_PRINTF("obd_get_bss_scan_result() return null ...\n");
		SET_OUTMSG("FAIL");
		return result;
	}

    for (i=0, bss = bss_list; bss; i++, bss = bss->next) 
    	count++;

    if (count > 0)
	{
    	SET_OUTMSG("OK");
		result = 1;
	}
    else
    {
    	DBG_PRINTF("bss_list is empty ...\n");
    	SET_OUTMSG("FAIL");
		result = 0;
    }

   	free(bss_list);
   	return result;
}

int verify_obd_start_wps_enrollee(char *str, pid_t pid)
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

int tc_obd_start_wps_enrollee(int argc, char *argv[], char *outmsg)
{
	int result = 0;
	pid_t pid = -1;
	int exit_status = 0;

#ifdef RTCONFIG_RALINK
	char find_str[] = {"start_wps_method"};
#else 	// RTCONFIG_RALINK
	char find_str[] = {"start_wps_enr"};
#endif	// RTCONFIG_RALINK	

	pid = fork();
	if (pid == -1)
	{
		DBG_PRINTF("fork() return fail ...\n");
		result = 0;
	}
	else if (pid == 0)
	{
		obd_start_wps_enrollee();
		exit(0);
	}
	else
	{
		waitpid(pid, &exit_status, 0);	
		result = verify_obd_start_wps_enrollee(find_str, pid);
	}

#ifdef RTCONFIG_RALINK
	if (result == 1)
	{
		if (nvram_match("wps_enrollee", "1"))
			result = 1;
		else
			result = 0;
	}
#endif	/* RTCONFIG_RALINK */

	SET_OUTMSG("%s", (result==1) ? "OK" : "FAIL");
	return result;
}

#ifdef RTCONFIG_RALINK
static int set_ie_buf(int type, unsigned char *dst, unsigned char *data)
{
	int len = 0;
	unsigned char c;

 	switch (type) {
     	case 1:
         	len = 1;
            break;
        case 3:
            len = 20;
            break;
        case 4:
            len = 6;
            break;
        case 5:
            len = strlen(get_productid());
           	break;
        case 6:
           	len = 1 + rssi_info.cap_count * 7;
          	break;
        case 15:
        case 16:
        case 17:
           	len = 2;
           	break;
  	}

   	if (len) {
       	c = type;
       	memcpy(dst, &c, 1);
        c = len;
        memcpy(dst+1, &c, 1);
      	memcpy(dst+2, data, len);
   	}
  	return len + 2;
}
#endif	/* RTCONFIG_RALINK */

#ifdef RTCONFIG_RALINK
unsigned char *gen_ie_data(size_t *out_len)
{
	unsigned char *b = NULL, *p = NULL;
	unsigned char ea[ETHER_ADDR_LEN];
  	short reboot_time = 60;
   	short connection_timeout = 15;
   	short traffic_timeout = 15;
   	unsigned char status[] = { 0x3 };
   	size_t len = 0;

   	if (out_len)
   		*out_len = 0;

   	if (!(b = malloc(sizeof(unsigned char) * 256)))
	{
		DBG_PRINTF("malloc failed ...\n");
		return NULL;
	}

	memset(b, 0, sizeof(unsigned char) * 256);
	p = b;
	p += set_ie_buf(1, p, status);
  	p += set_ie_buf(1, p, status);
  	ether_atoe(get_lan_hwaddr(), ea);
  	p += set_ie_buf(4, p, ea);

   	p += set_ie_buf(5, p, (unsigned char *) get_productid());
	memset(&rssi_info, 0, sizeof(rssi_info));
  	p += set_ie_buf(6, p, (unsigned char *) &rssi_info);
   	p += set_ie_buf(15, p, (unsigned char *) &reboot_time);
   	p += set_ie_buf(16, p, (unsigned char *) &connection_timeout);
   	p += set_ie_buf(17, p, (unsigned char *) &traffic_timeout);
	len = p - b;
	len += OUI_LEN;

	if (out_len)
		*out_len = len;

	return b;
}
#endif	/* RTCONFIG_RALINK */

int tc_obd_add_probe_req_vsie(int argc, char *argv[], char *outmsg)
{
	SET_OUTMSG("TBD");
	return 1;
#if 0	
#ifdef RTCONFIG_RALINK
	unsigned char *ie_data = NULL;
	size_t ie_data_len = 0, f_size = 0;
	int unit = 0;
	char vsie_str[1024], *b = NULL;
   	FILE *f = NULL;
   	int found = 0;

SET_OUTMSG("TBD");
return 1;

	if (argc < 2)
	{
		TC_PRINTF("usage:%s unit\n", argv[0]);
		TC_PRINTF("\tunit: wireless unit, 0:2g, 1:5g, 2:5g1\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	if (!(ie_data = gen_ie_data(&ie_data_len)))
	{
		DBG_PRINTF("gen_ie_data() return NULL...\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	msglevel = OBD_DEBUG_DETAIL;
	unit = atoi(argv[1]);

	memset(vsie_str, 0, sizeof(vsie_str));
	hex2str(ie_data, vsie_str, ie_data_len);

    obd_del_probe_req_vsie(unit, ie_data_len, ie_data);
    obd_add_probe_req_vsie(unit, ie_data_len, ie_data);

	if (!(f = fopen(OBD_DEBUG, "r")))
	{
		free(ie_data);
		DBG_PRINTF("%s file not found ...\n", OBD_DEBUG);
		SET_OUTMSG("FAIL");
		return 0;
	}

	fseek(f, 0, SEEK_END); // seek to end of file
	f_size = ftell(f); // get current file pointer
	fseek(f, 0, SEEK_SET);

	if (!(b = malloc(f_size)))
	{
		DBG_PRINTF("malloc fail ...\n");
		free(ie_data);
		fclose(f);
		SET_OUTMSG("FAIL");
		return 0;
	}

	memset(b, 0, f_size);
	if (fread(b, 1, f_size, f) <= 0)
	{
		DBG_PRINTF("fread fail ...\n");
		free(b);
		free(ie_data);
		fclose(f);
		SET_OUTMSG("FAIL");
		return 0;		
	}

	if (strstr(b, vsie_str))
		found = 1;
	else
		found = 0;

	free(b);
	free(ie_data);
	fclose(f);

	if (found == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return found;

#else 	/* RTCONFIG_RALINK */
	SET_OUTMSG("OK");
	return 1;
#endif	/* RTCONFIG_RALINK */
#endif
}

int tc_obd_del_probe_req_vsie(int argc, char *argv[], char *outmsg)
{
	SET_OUTMSG("TBD");
	return 1;	
#if 0	
#ifdef RTCONFIG_RALINK
	unsigned char *ie_data = NULL;
	size_t ie_data_len = 0, f_size = 0;
	int unit = 0;
	char vsie_str[1024], *b = NULL;
   	FILE *f = NULL;
   	int found = 0;

SET_OUTMSG("TBD");
return 1;

	if (argc < 2)
	{
		TC_PRINTF("usage:%s unit\n", argv[0]);
		TC_PRINTF("\tunit: wireless unit, 0:2g, 1:5g, 2:5g1\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	if (!(ie_data = gen_ie_data(&ie_data_len)))
	{
		DBG_PRINTF("gen_ie_data() return NULL...\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	msglevel = OBD_DEBUG_DETAIL;
	unit = atoi(argv[1]);
	memset(vsie_str, 0, sizeof(vsie_str));

	hex2str(ie_data, vsie_str, ie_data_len);

    nvram_set("obd_debug", "1");
    nvram_set("obd_dbg", "1");

    obd_del_probe_req_vsie(unit, ie_data_len, ie_data);

	if (!(f = fopen(OBD_DEBUG, "r")))
	{
		free(ie_data);
		DBG_PRINTF("%s file not found ...\n", OBD_DEBUG);
		SET_OUTMSG("FAIL");
		return 0;
	}

	fseek(f, 0, SEEK_END); // seek to end of file
	f_size = ftell(f); // get current file pointer
	fseek(f, 0, SEEK_SET);

	if (!(b = malloc(f_size)))
	{
		DBG_PRINTF("malloc fail ...\n");
		free(ie_data);
		fclose(f);
		SET_OUTMSG("FAIL");
		return 0;
	}

	memset(b, 0, f_size);
	if (fread(b, 1, f_size, f) <= 0)
	{
		DBG_PRINTF("fread fail ...\n");
		free(b);
		free(ie_data);
		fclose(f);
		SET_OUTMSG("FAIL");
		return 0;		
	}

	if (strstr(b, vsie_str))
		found = 1;
	else
		found = 0;

	free(b);
	free(ie_data);
	fclose(f);
    nvram_unset("obd_debug");
    nvram_unset("obd_dbg");	

	if (found == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return found;

#else 	/* RTCONFIG_RALINK */
	SET_OUTMSG("OK");
	return 1;
#endif	/* RTCONFIG_RALINK */
#endif
}

int tc_obd_led_blink(int argc, char *argv[], char *outmsg)
{
#ifdef RTCONFIG_RALINK	
	SET_OUTMSG("TBD");
#else 	/* RTCONFIG_RALINK */
	obd_led_blink();
	SET_OUTMSG("TBD");
#endif 	/* RTCONFIG_RALINK */	
	return 1;
}

int tc_obd_led_off(int argc, char *argv[], char *outmsg)
{
#ifdef RTCONFIG_RALINK	
	SET_OUTMSG("TBD");
#else 	/* RTCONFIG_RALINK */
	obd_led_off();
	SET_OUTMSG("TBD");
#endif 	/* RTCONFIG_RALINK */	
	return 1;
}
