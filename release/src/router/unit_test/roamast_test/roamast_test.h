/*
**  roamast_test.h
**
**
**
*/
#ifndef __ROAMAST_TESTH__
#define __ROAMAST_TESTH__

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

int tc_get_stainfo(int argc, char *argv[], char *outmsg);
int tc_rast_stamon_get_rssi(int argc, char *argv[], char *outmsg);
int tc_rast_retrieve_static_maclist(int argc, char *argv[], char *outmsg);
int tc_rast_set_maclist(int argc, char *argv[], char *outmsg);

static struct testcase_t testcases[] =
{
	{	"get_stainfo\0",				tc_get_stainfo,     				"Test get_stainfo()\0"       		 	},
  	{  	"stamon_get_rssi\0",  			tc_rast_stamon_get_rssi,    		"Test rast_stamon_get_rssi()\0"      	},
  	{  	"retrieve_static_maclist\0",	tc_rast_retrieve_static_maclist,    "Test rast_retrieve_static_maclist()\0"	},
  	{  	"set_maclist\0",  				tc_rast_set_maclist,    			"Test rast_set_maclist()\0"      		},
  	{  	NULL, NULL, NULL },
};

#endif	/* __ROAMAST_TESTH__ */
