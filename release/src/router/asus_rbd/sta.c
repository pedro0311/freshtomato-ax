#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shared.h>
#include <pthread.h>
#include <dirent.h>
#include <ctype.h>
#include <typedefs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

#include "list.h"
//#include "log.h"
#include "sta.h"


#define ARP_CACHE       "/proc/net/arp"
#define ARP_BUFFER_LEN  512
#define IPLEN           16

#define MAX_TRY 30

oppo_sta_pair_t* sta_node_new()
{
    oppo_sta_pair_t *pair;

    pair = (oppo_sta_pair_t *)calloc(1, sizeof(oppo_sta_pair_t));
    if (!pair)
        return NULL;
	pair->count = 0;
	
    INIT_LIST_HEAD(&pair->list);

    return pair;
}

void sta_node_free(oppo_sta_pair_t *pair)
{
    if (pair)
        free(pair);

    return;
}

void sta_node_del(struct list_head *list, oppo_sta_pair_t *node)
{
    oppo_sta_pair_t *pair,*pairt;

    list_for_each_entry_safe(pair, pairt, list, list) {
        if(!memcmp(pair->sta[0].mac, node->sta[0].mac, 6))
		{
			list_del(&pair->list);
			sta_node_free(pair);
			return;
		}
    }

    return;
}


void sta_list_free(struct list_head *list)
{
    oppo_sta_pair_t *pair,*pairt;

    list_for_each_entry_safe(pair, pairt, list, list) {
        list_del(&pair->list);
        sta_node_free(pair);
    }

    return;
}

/* need 1/3/4/6 fields */
// 192.168.89.189 0x1 0x2 00:0e:c7:9e:27:93 * br0
#define ARP_LINE_FORMAT "%s %s %s %s %s %s"

int sta_set_attr_ip(struct oppo_sta *sta, unsigned char *p_mac)
{
    struct in_addr srcv4;
    char ipstr[20], flag[10];
    char mac[20], device[10];
    char hw_type[20], mask[20];
	unsigned char a_mac[6];
	int hit = 0;
	
    FILE *fp = fopen(ARP_CACHE, "r");
    if (!fp) {
        printf("cannot open arp cache");
        return;
    }

    char header[ARP_BUFFER_LEN];
    if (!fgets(header, sizeof(header), fp)) {
        fclose(fp);
        return hit;
    }

    while (fscanf(fp, ARP_LINE_FORMAT, ipstr, hw_type, flag, mac, mask, device) == 6) {
#if 1
		if (strtoul(flag, NULL, 0) == 0x00) // not complete
        {
	//		printf("mac:%s ip:%s flag:%x not ready\n", mac, ipstr, strtoul(flag, NULL, 0)); 
			continue;
		}
#endif
		if (!ether_atoe(mac, a_mac)) {
			printf("can't convert mac: %s\n", mac);
			break;     
		}
		if (!memcmp(p_mac, a_mac, 6))
		{
			
			strcpy(sta->ip, ipstr);
			hit = 1;
			printf("find mac:%s ip:%s\n", mac, sta->ip);
			break;
		}
    }

    fclose(fp);
	
    return hit;
}

int check_sta_valid(char *ip, unsigned char *p_mac)
{
    struct in_addr srcv4;
    char ipstr[20], flag[10];
    char mac[20], device[10];
    char hw_type[20], mask[20];
	unsigned char a_mac[6];
	int hit = 0;
    FILE *fp = fopen(ARP_CACHE, "r");
    if (!fp) {
        printf("cannot open arp cache");
        return hit;
    }

    char header[ARP_BUFFER_LEN];
    if (!fgets(header, sizeof(header), fp)) {
        fclose(fp);
        return hit;
    }

    while (fscanf(fp, ARP_LINE_FORMAT, ipstr, hw_type, flag, mac, mask, device) == 6) {
#if 1
		if (strtoul(flag, NULL, 0) == 0x00) // not complete
        {
	//		printf("mac:%s ip:%s flag:%x not ready\n", mac, ipstr, strtoul(flag, NULL, 0));
			continue;
		}
#endif
		if (!ether_atoe(mac, a_mac)) {
			printf("can't convert mac: %s\n", mac);
			break;
		}
		if (!memcmp(p_mac, a_mac, 6) && !strcmp(ipstr, ip))
		{
			hit = 1;
			break;
		}
    }
    fclose(fp);

    return hit;
}
int is_sta_node_valid(oppo_sta_pair_t *node)
{
	int i = 0;
	int ret = 0;
	if(!node)
		return 0;
	for(int i = 0; i< node->count; i++)
	{
		if(!check_sta_valid(node->sta[i].ip, node->sta[i].mac))
			return 0;
	}

	return 1;
}

void sta_set_attr_primary(oppo_sta_pair_t *pair, unsigned char *mac, int status)
{
	int hit = 0;
	int retry = MAX_TRY;
	memcpy(pair->sta[STA_IS_PRIMARY].mac, mac, 6);
	pair->sta[STA_IS_PRIMARY].status = STA_CONNECTED; 
	pair->sta[STA_IS_PRIMARY].role = STA_IS_PRIMARY;
	while(retry>0 && !hit)
	{
		hit = sta_set_attr_ip(&pair->sta[STA_IS_PRIMARY], mac);
		sleep(1);
		retry --;
	}
	memset(pair->sta[STA_IS_SLAVE].mac, 0, 6);
	pair->sta[STA_IS_SLAVE].status = STA_DISCONNECTED; 
	pair->sta[STA_IS_SLAVE].role = STA_IS_SLAVE;
		
	pair->count = 1;
}

void sta_set_attr_pair(oppo_sta_pair_t *pair, unsigned char *p_mac, unsigned char *s_mac)
{
	int hit = 0;
	int retry = MAX_TRY;

/*	
	memcpy(pair->sta[0].mac, p_mac, 6);
	pair->sta[STA_IS_PRIMARY].status = STA_CONNECTED; 
	pair->sta[STA_IS_PRIMARY].role = STA_IS_PRIMARY;
	while(retry>0 && !hit)
	{
		hit = sta_set_attr_ip(&pair->sta[STA_IS_PRIMARY], p_mac);
		retry --;
	}
	
	retry = 10;
	hit = 0;
*/	
	memcpy(pair->sta[STA_IS_SLAVE].mac, s_mac, 6);
	pair->sta[STA_IS_SLAVE].status = STA_CONNECTED; 
	pair->sta[STA_IS_SLAVE].role = STA_IS_SLAVE;
	while(retry>0 && !hit)
	{
		hit = sta_set_attr_ip(&pair->sta[STA_IS_SLAVE], s_mac);
		sleep(1);
		retry --;
	}	
	pair->count = 2;
}

oppo_sta_pair_t *sta_list_find(unsigned char *mac, struct list_head *stalist)
{
    oppo_sta_pair_t *pair;

    list_for_each_entry(pair, stalist, list) {
        if(!memcmp(pair->sta[STA_IS_PRIMARY].mac, mac, 6) || !memcmp(pair->sta[STA_IS_SLAVE].mac, mac, 6))
            return pair;
    }

    return NULL;
}

void sta_pair_node_dump(oppo_sta_pair_t *node)
{
	char mac[20]={0}, *p=NULL;
    printf("===============# of node=%d\n", node->count);
	for(int i = 0; i < node->count; i++)
	{
		p = ether_etoa(node->sta[i].mac, mac);
		printf("sta[%d] ip:%s mac:%s status:%d role:%d\n",
		 i,
		 node->sta[i].ip,
		 p,
		 node->sta[i].status,
		 node->sta[i].role);
    
	}
	
}

void sta_list_dump(struct list_head *stalist)
{
    oppo_sta_pair_t *pair;

    list_for_each_entry(pair, stalist, list) {
        sta_pair_node_dump(pair);
    }
}





