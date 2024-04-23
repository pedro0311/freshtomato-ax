#ifndef __STA_H__
#define __STA_H__

//#include "log.h"
#define STA_CONNECTED 0
#define STA_DISCONNECTED 1

#define STA_IS_PRIMARY 0
#define STA_IS_SLAVE 1

struct oppo_sta {
	unsigned char mac[6];
	char ip[INET_ADDRSTRLEN];
	int role; /* 0 primary 1 slave */
	int status; /* 0 connected 1 disconnected */
};

typedef struct oppo_sta_pair_s {
	struct oppo_sta sta[2];
	int count;
	struct list_head list;
} oppo_sta_pair_t;

extern oppo_sta_pair_t* sta_node_new();
extern void sta_node_free(oppo_sta_pair_t *pair);
extern void sta_list_free(struct list_head *list);
extern void sta_node_del(struct list_head *list, oppo_sta_pair_t *node);
extern void sta_set_attr_primary(oppo_sta_pair_t *pair, unsigned char *mac, int status);
extern void sta_set_attr_pair(oppo_sta_pair_t *pair, unsigned char *p_mac, unsigned char *s_mac);
extern oppo_sta_pair_t *sta_list_find(unsigned char *mac, struct list_head *stalist);
extern int is_sta_node_valid(oppo_sta_pair_t *node);

extern void sta_pair_node_dump(oppo_sta_pair_t *node);
extern void sta_list_dump(struct list_head *stalist);

#endif // __STA_H__
