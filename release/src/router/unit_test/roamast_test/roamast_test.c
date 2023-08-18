/*
**  roamast_test.c
**
**
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <shared.h>
#include <rc.h>
#if defined(RTCONFIG_RALINK)
#include <bcmnvram.h>
#include <ralink.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include  <float.h>
#ifdef RTCONFIG_WIRELESSREPEATER
#include <ap_priv.h>
#endif
#elif defined(RTCONFIG_QCA)
#include <bcmnvram.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#elif defined(RTCONFIG_ALPINE)
#include <bcmnvram.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#else
#include <wlioctl.h>
#include <bcmendian.h>
#endif
#include <wlutils.h>
#ifdef RTCONFIG_ADV_RAST
#include <sys/un.h>
#include <sys/stat.h>
#include <json.h>
#include <math.h>
#include <pthread.h>
#ifdef RTCONFIG_CFGSYNC
#include <cfg_ipc.h>
#endif
#endif
#include <roamast.h>
#include <libgen.h>
#include <sys/wait.h>
#include "roamast_test.h"

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

int get_iface_count(void)
{
	int count = 0;
	char ifname[64], *next = NULL;

	foreach(ifname, nvram_safe_get("wl_ifnames"), next)
	{
		SKIP_ABSENT_FAKE_IFACE(ifname);
		count++;		
	}

	return count;
}

void rast_free_bssinfo(void)
{
        /* free assoclist */
        int i, vi, wlif_count = get_iface_count;
        rast_sta_info_t *assoclist, *next;
#ifdef RTCONFIG_ADV_RAST
        rast_maclist_t *maclist_r, *m_next;
#endif
        for(i = 0; i < wlif_count; i++) {
#ifdef RTCONFIG_ADV_RAST
DBG_TRACE_LINE;        	
                maclist_r = bssinfo[i].static_client;
                while(maclist_r) {
DBG_TRACE_LINE;        	

                        m_next = maclist_r->next;
                        free(maclist_r);
                        maclist_r = m_next;
                }
DBG_TRACE_LINE;        	

#endif
DBG_TRACE_LINE;        	

                for(vi = 0; vi < MAX_SUBIF_NUM; vi++) {
DBG_TRACE_LINE;        	

                        assoclist = bssinfo[i].assoclist[vi];
                        while(assoclist) {
                                next = assoclist->next;
                                free(assoclist);
                                assoclist = next;
DBG_TRACE_LINE;        	

                        }
#ifdef RTCONFIG_ADV_RAST
DBG_TRACE_LINE;        	

                        maclist_r = bssinfo[i].maclist[vi];
DBG_TRACE_LINE;        	

                        while(maclist_r) {
                                m_next = maclist_r->next;
                                free(maclist_r);
                                maclist_r = m_next;
                        }

                        bssinfo[i].static_maclist[vi] = NULL;
DBG_TRACE_LINE;        	

#endif
                }
        }
DBG_TRACE_LINE;        	

        return;
}


int tc_get_stainfo(int argc, char *argv[], char *outmsg)
{	
	int update = 0, result = 0, unit, subunit, wlif_count = 0; 
	char ifname[64], *next = NULL;
	struct rast_sta_info *staInfo = NULL;

#ifdef RTCONFIG_RALINK
	if (argc < 2)
	{
		TC_PRINTF("usage:%s sta_mac\n", argv[0]);
		TC_PRINTF("\tsta_mac: client mac, ex.00:12:9e:ff:ff:01\n");
		SET_OUTMSG("FAIL");
		return 0;
	}
#endif	/* RTCONFIG_RALINK */	
	
	rast_init_bssinfo();	// init bssinfo global variable
	foreach(ifname, nvram_safe_get("wl_ifnames"), next)
	{
		unit = subunit = 0;
		SKIP_ABSENT_FAKE_IFACE(ifname);
		if (get_ifname_unit(ifname, &unit, &subunit) < 0)
			continue;
		for (subunit=0; subunit<MAX_SUBIF_NUM; subunit++)
		{
			get_stainfo(unit, subunit);
			update = 1;
		}

		wlif_count++;
	}

	if (update == 1)
	{
#ifndef RTCONFIG_RALINK
		result = 1;
#else	/* !RTCONFIG_RALINK */
		for (unit=0; unit<wlif_count; unit++)
		{
			for (subunit=0; subunit<MAX_SUBIF_NUM; subunit++)
			{
				staInfo = bssinfo[unit].assoclist[subunit];
				if (!staInfo)
					continue;
				if ((result = (strncmp(str_lowercase(staInfo->mac_addr), str_lowercase(argv[1]), strlen(argv[1])) == 0)))
					break;
			}

			if (result==1)
				break;
		}
#endif	/* !RTCONFIG_RALINK */			
	}

	// rast_free_bssinfo();
	if (result == 1)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
}

int tc_rast_stamon_get_rssi(int argc, char *argv[], char *outmsg)
{
	int result = 0, sta_rssi = 0, unit = 0, subunit = 0; 
	char ifname[64], *next = NULL;
	struct ether_addr ea_tmp;

	if (argc < 2)
	{
		TC_PRINTF("usage:%s sta_mac\n", argv[0]);
		TC_PRINTF("\tsta_mac: client mac, ex.00:12:9e:ff:ff:01\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	foreach(ifname, nvram_safe_get("wl_ifnames"), next)
	{
		sta_rssi = unit = subunit = 0;
		SKIP_ABSENT_FAKE_IFACE(ifname);
		if (get_ifname_unit(ifname, &unit, &subunit) < 0)
			continue;
		if ((sta_rssi = rast_stamon_get_rssi(unit, rast_ether_atoe(argv[1],&ea_tmp))) != 0)
			break;
	}

	if (sta_rssi == 0)
		result = 0;
	else
	{
#ifdef RTCONFIG_RALINK
		if (sta_rssi <= -100 || sta_rssi >= 0)
			result = 0;
		else
			result = 1;
#else	/* RTCONFIG_RALINK */
		result = 1;
#endif	/* RTCONFIG_RALINK */	
	}

	DBG_PRINTF("sta_rssi: %d\n", sta_rssi);
	if (result)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
}


int tc_rast_retrieve_static_maclist(int argc, char *argv[], char *outmsg)
{

	int update = 0, result = 0, unit, subunit, wlif_count = 0, size = 0; 
	char ifname[64], *next = NULL, mac[32];
	struct maclist *maclist = NULL;

	if (argc < 2)
	{
		TC_PRINTF("usage:%s sta_mac\n", argv[0]);
		TC_PRINTF("\tsta_mac: client mac, ex.00:12:9e:ff:ff:01\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	rast_init_bssinfo();	// init bssinfo global variable
	foreach(ifname, nvram_safe_get("wl_ifnames"), next)
	{
		unit = subunit = 0;
		SKIP_ABSENT_FAKE_IFACE(ifname);
		if (get_ifname_unit(ifname, &unit, &subunit) < 0)
			continue;
		for (subunit=0; subunit<MAX_SUBIF_NUM; subunit++)
		{
			rast_retrieve_static_maclist(unit, subunit);
			update = 1;
		}

		wlif_count++;
	}

	if (update == 1)
	{
		for (unit=0; unit<wlif_count; unit++)
		{
			for (subunit=0; subunit<MAX_SUBIF_NUM; subunit++)
			{
				maclist = bssinfo[unit].static_maclist[subunit];
				if (maclist)
				{
	               for (size = 0; size < maclist->count; size++) 
	               {
	               		memset(mac, 0, sizeof(mac));
	               		snprintf(mac, sizeof(mac),  ""MACF"", ETHER_TO_MACF(maclist->ea[size]));
	               		if ((result = (strncmp(str_lowercase(mac), str_lowercase(argv[1]), strlen(mac)) == 0)))
	               			break;
	               }					
				}

				if (result == 1)
					break;
			}

			if (result == 1)
				break;
		}		
	}

	if (result)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
}

static void rast_add_to_maclist(int bssidx, int vifidx, struct ether_addr *addr)
{
        char prefix[32];
        rast_maclist_t *ptr;
        ptr = bssinfo[bssidx].maclist[vifidx];

        snprintf(prefix, sizeof(prefix), "wl%d.%d", bssidx, vifidx);

        while(ptr) {
#if (defined(RTCONFIG_RALINK) || defined(RTCONFIG_QCA) || defined(RTCONFIG_LANTIQ))
                if(memcmp(&(ptr->addr), addr, ETHER_ADDR_STR_LEN/3) == 0)
#else
                if (eacmp(&(ptr->addr), addr) == 0)
#endif
                        break;
                ptr = ptr->next;
        }

        if(!ptr) {
                // add sta to maclist
                ptr = malloc(sizeof(rast_maclist_t));
                if(!ptr) {
                        RAST_INFO("[%s]: Err: Exiting %d malloc failure", __FUNCTION__, __LINE__);
                        return;
                }
                memset(ptr, 0, sizeof(rast_maclist_t));
                ptr->addr = *addr;
                ptr->timestamp = uptime();
                ptr->mesh_node = 0;
                ptr->next = bssinfo[bssidx].maclist[vifidx];
                bssinfo[bssidx].maclist[vifidx] = ptr;

                RAST_DBG("[%s] add mac: "MACF" to maclist (timestamp:%ld)\n",
                        vifidx > 0 ? prefix : bssinfo[bssidx].wlif_name,
                        ETHER_TO_MACF(ptr->addr),
                        ptr->timestamp);

                rast_set_maclist(bssidx, vifidx);
        }

        return;

}

int tc_rast_set_maclist(int argc, char *argv[], char *outmsg)
{
	int result = 0, unit = 0, subunit = 0; 
	char ifname[64], *next = NULL;
	struct ether_addr ea_tmp;

	if (argc < 2)
	{
		TC_PRINTF("usage:%s sta_mac\n", argv[0]);
		TC_PRINTF("\tsta_mac: client mac, ex.00:12:9e:ff:ff:01\n");
		SET_OUTMSG("FAIL");
		return 0;
	}

	rast_init_bssinfo();	// init bssinfo global variable
	foreach(ifname, nvram_safe_get("wl_ifnames"), next)
	{
		unit = subunit = 0;
		SKIP_ABSENT_FAKE_IFACE(ifname);
		if (get_ifname_unit(ifname, &unit, &subunit) < 0)
			continue;
		for (subunit=0; subunit<MAX_SUBIF_NUM; subunit++)
		{
			rast_add_to_maclist(unit, subunit, rast_ether_atoe(argv[1],&ea_tmp));
			result = 1;
		}
	}

	// rast_free_bssinfo();
	if (result)
		SET_OUTMSG("OK");
	else
		SET_OUTMSG("FAIL");

	return result;
}

