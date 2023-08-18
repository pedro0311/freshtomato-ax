include ../../common.mak
include $(SRCBASE)/router/.config

CFGMNT_DIR := $(TOP)/cfg_mnt

CFLAGS += -Wall -Os $(EXTRACFLAGS)
CFLAGS += -I. -I$(TOP)/shared -I$(SRCBASE) -I$(SRCBASE)/include -I./include -I$(TOP)/networkmap -I$(CFGMNT_DIR)
CFLAGS += -I./sysdeps
CFLAGS += -I$(TOP)/openssl/include
CFLAGS += -I$(TOP)/json-c

ifeq ($(TOP_PLATFORM),)
LDFLAGS += -L$(TOP)/nvram${BCMEX}${EX7} -lnvram -L$(TOP)/shared -lshared
else
LDFLAGS += -L$(TOP_PLATFORM)/nvram${BCMEX}${EX7} -lnvram -L$(TOP)/shared -lshared
endif
LDFLAGS += -L$(TOP)/openssl -lcrypto -lssl
LDFLAGS += -lpthread
LDFLAGS += -L$(TOP)/json-c/.libs -ljson-c
LDFLAGS += -lm

ifeq ($(RTCONFIG_AMAS),y)
CFLAGS += -I$(TOP)/amas-utils -I$(TOP)/shared/sysdeps/amas
LDFLAGS += -L$(TOP)/amas-utils -lamas-utils
LDFLAGS += -L$(TOP)/lldpd-0.9.8/src/lib/.libs -llldpctl -L$(STAGEDIR)/usr/lib/ -ljansson
ifeq ($(RTCONFIG_AVBLCHAN),y)
CFLAGS += -DAVBLCHAN
endif
CFLAGS += -DFREE_MEMORY_DOWNLOAD
endif

ifeq ($(HND_ROUTER),y)
CFLAGS += -DPTHREAD_STACK_SIZE_256K -DPTHREAD_EXIT
ifeq ($(TOP_PLATFORM),)
LDFLAGS += -L$(TOP)/wlcsm -lwlcsm
else
LDFLAGS += -L$(TOP_PLATFORM)/wlcsm -lwlcsm
endif
else ifeq ($(RTCONFIG_QCA),y)
CFLAGS += -DPTHREAD_STACK_SIZE_256K -DPTHREAD_EXIT
endif

ifeq ($(RTCONFIG_HND_ROUTER_AX),y)
CFLAGS +=-I$(SRCBASE)/../components/wlioctl/include -I$(SRCBASE)/../components/proto/include -I$(SRCBASE)/wl/sys
endif

ifeq ($(RTCONFIG_NOTIFICATION_CENTER),y)
CFLAGS  += -I$(TOP)/sqlite
CFLAGS  += -I$(TOP)/nt_center/lib
CFLAGS  += -I$(TOP)/nt_center/actMail
CFLAGS  += -I$(TOP)/wlc_nt
CFLAGS  += -DASUSWRT_SDK
LDFLAGS += -L$(TOP)/sqlite/.libs -lsqlite3 -lpthread -L$(TOP)/nt_center/lib -lnt
LDFLAGS += -L$(TOP)/wlc_nt -lwlc_nt_client
endif

ifeq ($(RTCONFIG_DHDAP), y)
CFLAGS += -DDEBUG_WL
endif

ifeq ($(RTCONFIG_INTEL),y)
CFLAGS += -fPIC
endif
ifeq ($(RTCONFIG_BCMARM),y)
CFLAGS += -I$(SRCBASE)/shared/bcmwifi/include -DTYPEDEF_FLOAT_T
CFLAGS += -I$(SRCBASE)/common/include
else ifeq ($(RTCONFIG_RALINK),y)
CFLAGS += -I$(TOP)/shared/sysdeps/ralink -I$(TOP)/wireless_tools
else ifeq ($(RTCONFIG_QCA),y)
CFLAGS += -fPIC
else ifeq ($(RTCONFIG_REALTEK),y)
CFLAGS += -fPIC
endif

ifeq ($(RTCONFIG_SW_HW_AUTH),y)
CFLAGS  += -I$(TOP)/sw-hw-auth
endif

ifeq ($(RTCONFIG_LIBASUSLOG),y)
CFLAGS += -I$(TOP)/libasuslog
LDFLAGS += -L$(TOP)/libasuslog -lasuslog
endif

ifeq ($(RTCONFIG_RALINK),y)
COMMON_OBJS += $(CFGMNT_DIR)/ralink.o
vpath %.c sysdeps/ralink
else ifeq ($(RTCONFIG_QCA),y)
CFLAGS += -I$(TOP)/shared/sysdeps/qca -I$(TOP)/wireless_tools
COMMON_OBJS += $(CFGMNT_DIR)/qca.o
vpath %.c sysdeps/qca
else ifeq ($(RTCONFIG_REALTEK),y)
COMMON_OBJS += $(CFGMNT_DIR)/realtek.o
vpath %.c sysdeps/realtek
else ifeq ($(RTCONFIG_INTEL),y)
COMMON_OBJS += $(CFGMNT_DIR)/intel.o
vpath %.c sysdeps/intel
else ifeq ($(RTCONFIG_LANTIQ),y)
COMMON_OBJS += $(CFGMNT_DIR)/lantiq.o
CFLAGS += -fPIC
vpath %.c sysdeps/lantiq
else
COMMON_OBJS += $(CFGMNT_DIR)/broadcom.o
vpath %.c sysdeps/broadcom
endif

ifeq ($(RTCONFIG_AMAS),y)
COMMON_OBJS += $(CFGMNT_DIR)/chmgmt.o
endif
COMMON_OBJS += $(CFGMNT_DIR)/encrypt.o $(CFGMNT_DIR)/cfg_crc.o $(CFGMNT_DIR)/cfg_udp.o $(CFGMNT_DIR)/cfg_dencrypt.o $(CFGMNT_DIR)/cfg_common.o $(CFGMNT_DIR)/cfg_ipc.o $(CFGMNT_DIR)/cfg_wevent.o $(CFGMNT_DIR)/cfg_upgrade.o $(CFGMNT_DIR)/cfg_clientlist.o $(CFGMNT_DIR)/cfg_sched.o $(CFGMNT_DIR)/cfg_slavelist.o
COMMON_OBJS += $(CFGMNT_DIR)/cfg_capability.o  $(CFGMNT_DIR)/cfg_action.o $(CFGMNT_DIR)/cfg_mnt.o $(CFGMNT_DIR)/cfg_client.o $(CFGMNT_DIR)/cfg_eventnotify.o
COMMON_OBJS += $(CFGMNT_DIR)/cfg_ctrllist.o $(CFGMNT_DIR)/cfg_bandindex.o
ifeq ($(RTCONFIG_DWB),y)
COMMON_OBJS += $(CFGMNT_DIR)/cfg_dwb.o
endif
COMMON_OBJS += $(CFGMNT_DIR)/cfg_chanspec.o
COMMON_OBJS += $(CFGMNT_DIR)/cfg_parammgnt.o
ifeq ($(RTCONFIG_AMAS),y)
SERVER_OBJS += $(COMMON_OBJS) $(CFGMNT_DIR)/cfg_eventnotify.o
endif
SERVER_OBJS += $(COMMON_OBJS) $(CFGMNT_DIR)/cfg_mnt.o
CLIENT_OBJS += $(COMMON_OBJS) $(CFGMNT_DIR)/cfg_client.o
REPORTSTATUS_OBJS += $(CFGMNT_DIR)/cfg_common.o $(CFGMNT_DIR)/cfg_reportstatus.o

ifeq ($(RTCONFIG_WIFI_SON),y)
CFLAGS += -DREPORT_PAP_INFO
endif
ifeq ($(RTCONFIG_AMAS),y)
COMMON_OBJS += $(CFGMNT_DIR)/cfg_roaming.o
CFLAGS += -DLEGACY_ROAMING
COMMON_OBJS += $(CFGMNT_DIR)/cfg_onboarding.o $(CFGMNT_DIR)/cfg_ethevent.o
CFLAGS += -DONBOARDING -DRSSI_LIST
# for wireless channel sync
CFLAGS += -DSYNC_WCHANNEL
# for roaming info for sta
#COMMON_OBJS += cfg_roaminginfo.o
#CFLAGS += -DROAMING_INFO
# for master detection
#CFLAGS += -DMASTER_DET
# for radar detected
COMMON_OBJS += $(CFGMNT_DIR)/cfg_radardet.o
CFLAGS += -DRADAR_DET
COMMON_OBJS += $(CFGMNT_DIR)/cfg_conndiag.o
CFLAGS += -DCONN_DIAG
endif

ifeq ($(RTCONFIG_AMASDB),y)
CFLAGS += -DDUAL_BAND_DETECTION
COMMON_OBJS += $(CFGMNT_DIR)/cfg_dualbandlist.o
endif

ifeq ($(RTCONFIG_TCODE),y)
ifeq ($(RTCONFIG_CFGSYNC_LOCSYNC),y)
COMMON_OBJS += $(CFGMNT_DIR)/cfg_loclist.o
endif
endif

ifeq ($(BUILD_NAME), $(filter $(BUILD_NAME), RT-AC5300 GT-AC5300 MAP-AC2200 RT-AC92U RT-AX92U GT-AX11000 RT-AX95Q))
CFLAGS += -DSUPPORT_TRI_BAND
endif

ifeq ($(RTCONFIG_MASTER_DET),y)
CFLAGS += -DMASTER_DET
endif

ifeq ($(RTCONFIG_PRELINK),y)
CFLAGS += -DPRELINK
COMMON_OBJS += $(CFGMNT_DIR)/cfg_prelink.o
endif

ifeq ($(RTCONFIG_LANTIQ),y)
# LDFLAGS += -L$(TOP)/fapi_wlan_common-1.0.0.1 -lfapiwlancommon -ldl -L$(TOP)/libhelper-1.4.0.2 -lhelper -lrt
LDFLAGS += -L$(TOP)/fapi_wlan_common-05.04.00.131 -lfapiwlancommon -ldl -L$(TOP)/libhelper-1.4.0.2 -lhelper -L$(TOP)/libfapi-0.1 -lfapi
LDFLAGS += -L$(TOP)/wlc_nt -lwlc_nt_client
endif

ifeq ($(RTCONFIG_LANTIQ),y)
CFLAGS += -I$(TOP)/shared/sysdeps/lantiq -I$(TOP)/wireless_tools -I$(TOP)/fapi_wlan_common-05.04.00.131/include -I$(TOP)/libhelper-1.4.0.2/include -DPACKAGE_ID=\"fapi_wlan_common\" -DLOGGING_ID="fapi_wlan_common"
CFLAGS  += -I$(TOP)/wlc_nt
endif

ifeq ($(RTCONFIG_BCMBSD),y)
CFLAGS += -DSMART_CONNECT
endif

COMMON_OBJS += cfg_mnt_test.o

all: cfg_mnt_test

cfg_mnt_test: $(COMMON_OBJS)
	@echo " [cfg_mnt_test] CC $@"
	@$(CC) -o $@ $^ -Wl,-allow-multiple-definition $(LDFLAGS) $(CFLAGS)
	@$(CROSS_COMPILE)objcopy  --redefine-sym entry=main $@.o                
	@$(CC) $@.o -o $@ $^ -Wl,-allow-multiple-definition $(LDFLAGS) $(CFLAGS)  

	$(SIZECHECK)
	$(CPTMP)


clean:
	@echo clean ...
	@rm -rf *.o
	@rm -rf cfg_mnt_test

