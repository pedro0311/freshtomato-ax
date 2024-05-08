/*
 * asus_rbd
 *
 * ASUSTeK Proprietary and Confidential. Copyright (C) 2023,
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTeK;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of ASUSTeK.
 *
 */

#include <stdarg.h>
#include <syslog.h>
#include <stdio.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <math.h>
#include <errno.h>
#include <bcmnvram.h>

#include <shared.h>
#include <list.h>
#include <sta.h>
#include <json.h>

#include <cfg_event.h>

#define DBG 0

#define RBD_LOG printf
#define MAX_LEN 1024

#define OP_ADD 0
#define OP_DEL 1

#define SINGLE_MAC_LEN 6
#define DOUBLE_MAC_LEN 14


#define MSG_FORMAT_ERR -1
#define STA_PAIR_NULL -2
#define MSG_PARSE_ERR -3
#define MSG_PROCESS_ERR -4
#define STA_NEW_ERR -5
#define STA_RECOVERY_ERR -6


#define CLONE_MARK 0x6
#define CLONE_TOS 0xb8


#define DEDUPTX_FILTER_IP_PATH "/proc/dedup/ip"

#define USE_IPTABLE_SKIPLOG  0

#define RB_STA_LIST_NV_NAME "rb_sta_list"

#define ROUTER_BOOST_STA_LIST_FILE "/tmp/rb_sta_list.json"

#define QUERY_IPTABLE_MANGLE_CMD "iptables -L -n -t mangle | grep TEE"
#define IPTABLE_MANGLE_FORMAT "%s %s %s %s %s %s %s %s %s"


int event_fd;
// IPC
static int thread_term = 0;
static sigset_t sigs_to_catch;

enum IPTABLE_ACTION {
	ADD_SKIPLOG = 0,
	DEL_SKIPLOG = 1,
	ADD_CLONE = 2,
	DEL_CLONE = 3
};

struct iptable_rule_template {
		int action;
		char command[100];
};

struct iptable_rule_template rules[] = {
	{ADD_SKIPLOG, "iptables -t mangle -I PREROUTING -p udp -s %s -j SKIPLOG"},
	{DEL_SKIPLOG, "iptables -t mangle -D PREROUTING -p udp -s %s -j SKIPLOG"},
#if 0
	{ADD_CLONE, "iptables -t mangle -I POSTROUTING -p udp -d %s -m mark --mark %d -j TEE --gateway %s"},
	{DEL_CLONE, "iptables -t mangle -D POSTROUTING -p udp -d %s -m mark --mark %d -j TEE --gateway %s"},	
#endif
	{ADD_CLONE, "iptables -t mangle -I POSTROUTING -p udp -d %s -m tos --tos 0x%x -j TEE --gateway %s"},
	{DEL_CLONE, "iptables -t mangle -D POSTROUTING -p udp -d %s -m tos --tos 0x%x -j TEE --gateway %s"},
};


enum FILTER_ACTION {
	ADD_IP = 0,
	DEL_IP = 1,
};

struct filter_template {
		int action;
		char command[100];
};

struct filter_template filter_actions[] = {
	{ADD_IP, "echo a %s>%s"},
	{DEL_IP, "echo d %s>%s"},
		
};


	
#if DBG
void dump_data(char *data, int len)
{
	char str[PATH_MAX], *ptr;
	int i;

	snprintf(str, sizeof(str), "dump data (%d)", len);
	ptr = str+strlen(str);

	for (i = 0; i < len; i++) {
		if (i%16 == 0){
			ptr[0] = '\n';
			++ptr;
		}
		snprintf(ptr, sizeof(str)-strlen(str), "%02X ", *(data+i));
		ptr = str+strlen(str);
	}

	RBD_LOG("%s\n", str);
}
#endif



LIST_HEAD(stalist);

int save_rb_sta_list_to_json_file(char *name, struct list_head *stalist)
{
	oppo_sta_pair_t *node;
	struct json_object *rootObj = json_object_new_object();
	struct json_object *json_obj = NULL;
	struct json_object *pair_ary_obj = NULL;
	struct json_object *ary_obj = NULL;  
	char mac[20]={0};
	
	ary_obj = json_object_new_array();
	list_for_each_entry(node, stalist, list) {
		pair_ary_obj = json_object_new_array();
		for(int i = 0; i < node->count; i++)
		{
			json_obj = json_object_new_object();
			ether_etoa(node->sta[i].mac, mac);
			json_object_object_add(json_obj, "ip", json_object_new_string(node->sta[i].ip));
			json_object_object_add(json_obj, "mac", json_object_new_string(mac));
			json_object_object_add(json_obj, "role", json_object_new_int(node->sta[i].role));
			json_object_object_add(json_obj, "status", json_object_new_int(node->sta[i].status));

			json_object_array_add(pair_ary_obj, json_obj);
		}
		json_object_array_add(ary_obj, pair_ary_obj);
	}
	json_object_object_add(rootObj, "contents", ary_obj);

	json_object_to_file(name, rootObj);
	json_object_put(rootObj);
	return 0;
}

int restore_rb_sta_list_from_json_file(char *file_path, struct list_head *stalist)
{
	int lock;
	int ret = -1;
	oppo_sta_pair_t *node;
	struct json_object *rootObj = json_object_new_object();
	struct json_object *json_obj = NULL;
	struct json_object *pair_ary_obj = NULL;
	struct json_object *ary_obj = NULL;
	struct json_object *obj, *obj_ip, *obj_mac, *obj_role, *obj_status;
	int sta_pair_len = 0, i = 0, j = 0, count = 0;
	char mac[20]={0};

	sta_list_free(stalist);
	lock = file_lock("rb_sta_list_lock");
	rootObj = json_object_from_file(file_path);
	if(rootObj){
		ary_obj = json_object_object_get(rootObj, "contents");
		sta_pair_len = json_object_array_length(ary_obj);
		for(i = 0; i < sta_pair_len; i++)
		{
			pair_ary_obj = json_object_array_get_idx(ary_obj, i);
			node = sta_node_new();
			if(!node)
				return STA_NEW_ERR;

			node->count = json_object_array_length(pair_ary_obj);
			for(j = 0; j < node->count; j++)
			{
				obj = json_object_array_get_idx(pair_ary_obj, j);
				obj_ip = json_object_object_get(obj, "ip");
				obj_mac = json_object_object_get(obj, "mac");
				obj_role = json_object_object_get(obj, "role");
				obj_status = json_object_object_get(obj, "status");
				strcpy(node->sta[j].ip, json_object_get_string(obj_ip));
				if (!ether_atoe(json_object_get_string(obj_mac), node->sta[j].mac))
					return STA_RECOVERY_ERR;
				node->sta[j].role = json_object_get_int(obj_role);
				node->sta[j].status = json_object_get_int(obj_status);
#if DBG
				printf("idx %d / %d ip=%s mac=%pM role=%d status=%d\n",
						j, node->count,
						node->sta[j].ip,
						node->sta[j].mac,
						node->sta[j].role,
						node->sta[j].status);
#endif
			}
			list_add_tail(&node->list, stalist);
		} // end of sta_pair_len
	}	// end of rootObj
	json_object_put(rootObj);
	file_unlock(lock);

	return 0;
}

/*
	message from wlcevenetd ( RE->CAP if STA connects/disconnects with RE)

*/
int parse_rb_event(char *buf, int buf_len, oppo_sta_pair_t *node)
{
	json_object *root = NULL;
	json_object *rbObj = NULL;
	json_object *eidObj = NULL;
	json_object *priMacObj = NULL;
	json_object *slvMacObj = NULL;
	unsigned char p_mac[6], s_mac[6];
	int ret = MSG_PARSE_ERR;
	int eid = 0;
	int has_slave = 0;
	char *p = NULL;
	int role = 0;

	root = json_tokener_parse((char *)buf);
	if(!root) goto out;
	json_object_object_get_ex(root, ROUTERBOOST_PREFIX, &rbObj);
	if(!rbObj) goto out;

	json_object_object_get_ex(rbObj, EVENT_ID, &eidObj);
	json_object_object_get_ex(rbObj, PRI_MAC_ADDR, &priMacObj);
	json_object_object_get_ex(rbObj, SLV_MAC_ADDR, &slvMacObj);
	if(!eidObj || !priMacObj || !slvMacObj) goto out;

	if(!ether_atoe(json_object_get_string(priMacObj), p_mac)) {
		goto out;
	}
	if(json_object_get_string(slvMacObj) != NULL &&
		ether_atoe(json_object_get_string(slvMacObj), s_mac)) {
		has_slave = 1;
	}

	if (eidObj) {
		eid = atoi(json_object_get_string(eidObj));
		if(eid == EID_RB_STA_DISCONN) {
		  // only one mac for disconnect
		  memcpy(node->sta[STA_IS_PRIMARY].mac, p_mac , 6);
		  node->count = 1;
		  ret = OP_DEL;
		  goto out;
		}
		else if(eid == EID_RB_STA_CONN) {
		  // connect , might 1 p_mac or 2 (p_mac and s_mac)
		  if(!has_slave) {
			// p_mac only
			memcpy(node->sta[STA_IS_PRIMARY].mac, p_mac , 6);
			node->count = 1;
			ret = OP_ADD;
			goto out;
		  }
		  else {
			// p_mac and s_mac
			node->count = 2;
			memcpy(node->sta[STA_IS_PRIMARY].mac, p_mac , 6);
			node->sta[0].role = STA_IS_PRIMARY;
			memcpy(node->sta[STA_IS_SLAVE].mac, s_mac , 6);
			node->sta[1].role = STA_IS_SLAVE;
			ret = OP_ADD;
			goto out;
		  }
		}
	}
out:
	json_object_put(root);
	return ret;
}

void run_system_cmd(int action, char *ip, int mark, char *gateway)
{
	char command[256];
	int i = action;

#if USE_IPTABLE_SKIPLOG
	if(rules[i].action == ADD_SKIPLOG || rules[i].action == DEL_SKIPLOG)
		sprintf(command, rules[i].command, ip);
#endif
	if(rules[i].action == ADD_CLONE || rules[i].action == DEL_CLONE)
			sprintf(command, rules[i].command, ip, mark, gateway);
#if DBG		
	printf("action=%d run system command =%s\n", action, command);
#endif

	system(command);
	sleep(1);
	
}

void run_filter_ip_cmd(int action, char *ip)
{
	char command[100];
	int i = action;
	if(filter_actions[i].action == ADD_IP || rules[i].action == DEL_IP)
		sprintf(command, filter_actions[i].command, ip, DEDUPTX_FILTER_IP_PATH);

#if DBG		
	printf("action=%d run system command =%s\n", action, command);
#endif

	system(command);
	sleep(1);
	
}


int process_event(char *buf, int buf_len)
{
	int ret = 0;
	oppo_sta_pair_t* p_new_node = NULL,  tmp_node = {0}, *node;
	char ip[50] = {0};
	
	ret = parse_rb_event(buf, buf_len, &tmp_node);
	if(ret < 0)
	{
		RBD_LOG("asus_rbd: ERROR parse_msg=%d\n", ret);	
		return ret;
	}

#if DBG
	RBD_LOG("++ prepare tmp_node action:%d !\n", ret);	
	sta_pair_node_dump(&tmp_node);
#endif
	
	
	if(ret == OP_ADD)
	{
		// find tmp_node primary to compare existing primary or primary/slave
		node = sta_list_find(tmp_node.sta[STA_IS_PRIMARY].mac, &stalist);
		if(!node)
		{
			if(tmp_node.count == 1)
			{	// primary assoc
				RBD_LOG("add a total new count=1 node\n");
				p_new_node = sta_node_new();
				if(!p_new_node) 
				{
					RBD_LOG("add a total new count=1 new node fail\n");
					return STA_NEW_ERR;
				
				}
				sta_set_attr_primary(p_new_node, tmp_node.sta[STA_IS_PRIMARY].mac, STA_CONNECTED);
				list_add_tail(&p_new_node->list, &stalist);
#if USE_IPTABLE_SKIPLOG
				run_system_cmd(ADD_SKIPLOG, p_new_node->sta[STA_IS_PRIMARY].ip, 0, NULL);
#endif
				// add primary to deDupTx module filter
				strcpy(ip,  p_new_node->sta[STA_IS_PRIMARY].ip);
				run_filter_ip_cmd(ADD_IP, ip);
				// not to free
				save_rb_sta_list_to_json_file(ROUTER_BOOST_STA_LIST_FILE, &stalist);
				return 0;
			}
			else if(tmp_node.count == 2)
			{	
				RBD_LOG("asus_rbd: ERROR can't find primary with slave assoc\n");
				ret = MSG_PROCESS_ERR;
				return ret;
			}				
		}
		// node already here
		else
		{
			if(tmp_node.count == 1)
			{	
				RBD_LOG("asus_rbd: ERROR add same primary twice\n");
				ret = MSG_PROCESS_ERR;
				return ret;
		
			}
			else if(tmp_node.count == 2)
			{	// slave assoc
				RBD_LOG("asus_rbd: add a slave node into pri\n");
			
				sta_set_attr_pair(node, tmp_node.sta[STA_IS_PRIMARY].mac, tmp_node.sta[STA_IS_SLAVE].mac);
#if USE_IPTABLE_SKIPLOG
				run_system_cmd(ADD_SKIPLOG, node->sta[STA_IS_SLAVE].ip, 0, NULL);
#endif
				run_system_cmd(ADD_CLONE, node->sta[STA_IS_PRIMARY].ip, CLONE_TOS, node->sta[STA_IS_SLAVE].ip);
				save_rb_sta_list_to_json_file(ROUTER_BOOST_STA_LIST_FILE, &stalist);

				return 0;
			}
		}	
	}
	else
	{
		//delete one 
		node = sta_list_find(tmp_node.sta[0].mac, &stalist);
			
		if(!node)
		{	
			RBD_LOG("asus_rbd: ERROR delete non-exist mac\n");
			ret = MSG_PROCESS_ERR;
			return ret;			
		}
		if(node->count == 1)
		{	
			// remove single primary
			RBD_LOG("asus_rbd: delete count=1 node pri\n");
			// del primary to deDupTx module filter
			strcpy(ip, node->sta[STA_IS_PRIMARY].ip);
			run_filter_ip_cmd(DEL_IP, ip);
#if USE_IPTABLE_SKIPLOG
			run_system_cmd(DEL_SKIPLOG, node->sta[STA_IS_PRIMARY].ip, 0, NULL);
#endif
			sta_node_del(&stalist, node);
			save_rb_sta_list_to_json_file(ROUTER_BOOST_STA_LIST_FILE, &stalist);

			return 0;
			
		}
		else if(node->count == 2)
		{
			RBD_LOG("asus_rbd: delete count=2 node\n");
			// remove slave
			if(!memcmp(tmp_node.sta[STA_IS_PRIMARY].mac, node->sta[STA_IS_SLAVE].mac, 6))
			{
				RBD_LOG("asus_rbd: delete one count=2 node - slave\n");	
#if USE_IPTABLE_SKIPLOG
				run_system_cmd(DEL_SKIPLOG, node->sta[STA_IS_SLAVE].ip, 0, NULL);
#endif
				run_system_cmd(DEL_CLONE, node->sta[STA_IS_PRIMARY].ip, CLONE_TOS, node->sta[STA_IS_SLAVE].ip);
				memset(node->sta[STA_IS_SLAVE].mac, 0, 6);
				node->sta[STA_IS_SLAVE].status = STA_DISCONNECTED;
				node->count = 1;
				save_rb_sta_list_to_json_file(ROUTER_BOOST_STA_LIST_FILE, &stalist);
				
				return 0;
				
				
			} else if(!memcmp(tmp_node.sta[STA_IS_PRIMARY].mac, node->sta[STA_IS_PRIMARY].mac, 6))
			{
				// remove primary, slave will off later , clean all
				RBD_LOG("asus_rbd: delete one count=2 node - pri\n");
				// del primary to deDupTx module filter				
				strcpy(ip, node->sta[STA_IS_PRIMARY].ip);
				run_filter_ip_cmd(DEL_IP, ip);
#if USE_IPTABLE_SKIPLOG
				run_system_cmd(DEL_SKIPLOG, node->sta[STA_IS_PRIMARY].ip, 0, NULL);
				run_system_cmd(DEL_SKIPLOG, node->sta[STA_IS_SLAVE].ip, 0, NULL);
#endif
				run_system_cmd(DEL_CLONE, node->sta[STA_IS_PRIMARY].ip, CLONE_TOS, node->sta[STA_IS_SLAVE].ip);
				sta_node_del(&stalist, node);
				save_rb_sta_list_to_json_file(ROUTER_BOOST_STA_LIST_FILE, &stalist);

				return 0;
			}
							
		}
		
	}	
	return ret;
}

static void rbd_ipc_receive(int sockfd){
	int length = 0;
	char buf[MAX_LEN];
	int lock; // file lock
    

	memset(buf, 0, sizeof(buf));
	if ((length = read(sockfd, buf, sizeof(buf))) <= 0) {
		RBD_LOG("ipc read socket error!\n");
		return;
	}

#if DBG
	dump_data(buf, length);
#endif

#if DBG
	RBD_LOG("++process!\n");	
	sta_list_dump(&stalist);
#endif
	lock = file_lock("asus_rbd_lock");
	process_event(buf, length);
	file_unlock(lock);
#if DBG
	RBD_LOG("--process!\n");
	sta_list_dump(&stalist);
#endif
	return;
}

static int asus_rbd_start_ipc_socket(void)
{
	struct sockaddr_un addr;
	fd_set readFds;
	int selectRet;
	struct timeval timeout = {2, 0};
	int sockfd, newsockfd;

	if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		RBD_LOG("ipc create socket error!\n");
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, RBD_IPC_SOCKET_PATH, sizeof(addr.sun_path)-1);

	unlink(RBD_IPC_SOCKET_PATH);

	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		RBD_LOG("ipc bind socket error!\n");
		exit(-1);
	}

	if (listen(sockfd, CD_IPC_MAX_CONNECTION) == -1) {
		RBD_LOG("ipc listen socket error!\n");
		exit(-1);
	}

	while (!thread_term) {

		FD_ZERO(&readFds);
		FD_SET(sockfd, &readFds);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		selectRet = select(sockfd + 1, &readFds, NULL, NULL, &timeout);
		//Check return, -1 is error, 0 is timeout
		if (selectRet == -1)  {
			//RBD_LOG(" socket select error : %s\n", strerror(errno));
			if (errno == EINTR || errno == ENOENT) continue;
			goto err;
		} else if (selectRet == 0) {
			continue;
		}

		//RBD_LOG("ipc accept socket...");
		if ( (newsockfd = accept(sockfd, NULL, NULL)) == -1) {
			RBD_LOG("ipc accept socket error!\n");
			continue;
		}

		rbd_ipc_receive(newsockfd);
		close(newsockfd);
	}

err:
	RBD_LOG("%s : ipc close socket!\n", __func__);
	close(sockfd);
	return 0;
}

static void rbd_exit(int sig){

	nvram_set("rbd_ready","0");
	thread_term = 1;
	remove("/var/run/rbd.pid");
	exit(0);
}

int keep_iptables_sanity(void)
{
	char target[20], pro[20], opt[20], src[20], dst[20], tos[20], match[20], tee[20], gw_str[20], gw[20];
	char line[128] = {0};
	int ret = 0;
	int tokens = 0;
	FILE *fp = NULL;
	char *delim = ":";
	char *pch;
        
	fp = popen(QUERY_IPTABLE_MANGLE_CMD, "r");
	if(!fp) {
		perror("can't query iptable mangle table");
		return -1;
	}
		
	while (fgets(line, sizeof(line), fp) != NULL) {
		/*
		target     prot opt source               destination
		TEE        udp  --  0.0.0.0/0            192.168.51.40        tos match0xb8/0xff TEE gw:192.168.51.178
		*/		
		tokens = sscanf(line, IPTABLE_MANGLE_FORMAT, target, pro, opt, src, dst, tos, match, tee, gw_str);			
		if(tokens != 9)
		{
			memset(target, 0, sizeof(target));
			memset(pro, 0, sizeof(pro));
			memset(opt, 0, sizeof(opt));
			memset(src, 0, sizeof(src));
			memset(dst, 0, sizeof(dst));
			memset(tos, 0, sizeof(tos));
			memset(match, 0, sizeof(match));			
			memset(tee, 0, sizeof(tee));
			memset(gw_str, 0, sizeof(gw_str));
			continue;
		}
		
		pch = strtok(gw_str, delim);
		while (pch != NULL) {
			strcpy(gw, pch);
			pch = strtok(NULL, delim);
		}
		
		run_system_cmd(DEL_CLONE, dst, CLONE_TOS, gw);
	}
		
	pclose(fp);
	
	return ret;
}

/* service main entry */
int main(int argc, char *argv[])
{
	int err = 0;
	FILE *fp;
	oppo_sta_pair_t *pair,*pairt;
	
	RBD_LOG("asus_rbd....\n");
	sigaddset(&sigs_to_catch, SIGTERM);
	sigprocmask(SIG_UNBLOCK, &sigs_to_catch, NULL);
	signal(SIGTERM, rbd_exit);
	
	/* write pid */
	if((fp = fopen("/var/run/rbd.pid", "w")) != NULL){
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}
	restore_rb_sta_list_from_json_file(ROUTER_BOOST_STA_LIST_FILE, &stalist);
#if DBG
	sta_list_dump(&stalist);
#endif
	keep_iptables_sanity();
	//remove invalid node
    list_for_each_entry_safe(pair, pairt, &stalist, list) {
		sta_pair_node_dump(pair);
		if(!is_sta_node_valid(pair))
		{
			list_del(&pair->list);
			sta_node_free(pair);
		}
		else if(pair->count == 1 && pair->sta[STA_IS_PRIMARY].role == STA_IS_PRIMARY)
		{
#if USE_IPTABLE_SKIPLOG
			run_system_cmd(ADD_SKIPLOG, pair->sta[STA_IS_PRIMARY].ip, 0, NULL);
#endif
			run_filter_ip_cmd(ADD_IP, pair->sta[STA_IS_PRIMARY].ip);
		}
		else if(pair->count == 2 && pair->sta[STA_IS_SLAVE].role == STA_IS_SLAVE)
		{
			run_system_cmd(ADD_CLONE, pair->sta[STA_IS_PRIMARY].ip, CLONE_TOS, pair->sta[STA_IS_SLAVE].ip);

		}
    }
#if DBG
	sta_list_dump(&stalist);
#endif	
	nvram_set("rbd_ready","1");

#if 0
    char command[256];
	for(int i=0; i< sizeof(rules)/sizeof(rules[0]); i++)
	{
		if(rules[i].action == ADD_SKIPLOG || rules[i].action == DEL_SKIPLOG)
			sprintf(command, rules[i].command, "1.2.3.4");
		
		if(rules[i].action == ADD_CLONE || rules[i].action == DEL_CLONE)
			sprintf(command, rules[i].command, "1.2.3.4", 6, "1.2.3.5");
		
		printf("action=%d command=%s\n", rules[i].action, command);
	}

#endif

	asus_rbd_start_ipc_socket();
	
	rbd_exit(0);
	return err;
}
