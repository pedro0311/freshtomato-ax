include ../../common.mak
include $(SRCBASE)/.config

CFLAGS += -Wall -Os $(EXTRACFLAGS)
CFLAGS += -I. -I$(TOP)/shared -I$(SRCBASE)/include -I$(TOP)/rc -I$(TOP)/libasuslog 
##LDFLAGS = -L$(TOP)/nvram${BCMEX}${EX7} -lnvram -L$(TOP)/shared -lshared -lcrypt -ldl
ifneq ($(HND_ROUTER),y)
CFLAGS += -idirafter$(LINUXDIR)/include
endif
ifneq ($(or $(findstring linux-3.10.14,$(LINUXDIR))),)
CFLAGS += -idirafter$(LINUXDIR)/include/uapi
endif
ifneq ($(or $(findstring linux-3.10.108,$(LINUXDIR))),)
CFLAGS += -idirafter$(LINUXDIR)/include/uapi
endif
CFLAGS += $(PRIVATE_EXTRACFLAGS)
CFLAGS += -I. -I$(TOP)/shared -I$(SRCBASE)/include
ifneq ($(HND_ROUTER),y)
CFLAGS += -idirafter$(LINUXDIR)/include
endif

ifeq ($(RTCONFIG_DPSTA),y)
CFLAGS += -I$(TOP)/dpsta
endif
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -DDLM -DASUS_DDNS -DASUS_EXT -DBTN_SETUP -DWEB_REDIRECT

CFLAGS += -DDEBUG_RCTEST

ifeq ($(RTCONFIG_WPS),y)
CFLAGS += -I$(SRCBASE)/wps/brcm_apps/include
endif

ifeq ($(RTCONFIG_REALTEK),y)
CFLAGS += -Wl,--gc-sections -ldl
ifneq ($(findstring CONFIG_RTL_USERSPACE_WTDOG=, $(shell cat $(LINUXDIR)/.config)),)
export CFLAGS += -DRTL_WTDOG
endif
else ifeq ($(RTCONFIG_RALINK),y)
CFLAGS += -I$(TOP)/shared/sysdeps/ralink -I$(TOP)/wireless_tools
ifneq ($(findstring MAC_REPEATER=, $(shell cat $(LINUXDIR)/.config)),)
export CFLAGS += -DMAC_REPEATER
endif
ifneq ($(findstring CONFIG_RT2860V2_AP_EDCCA_MONITOR=, $(shell cat $(LINUXDIR)/.config)),)
export CFLAGS += -DCE_ADAPTIVITY
else
ifeq (y,$(filter y,$(RTN56UB1) $(RTN56UB2)))
export CFLAGS += -DCE_ADAPTIVITY
endif
endif
ifeq ($(RTCONFIG_RALINK_EDCCA),y)
export CFLAGS += -DCE_ADAPTIVITY
endif
endif

ifeq ($(RTCONFIG_QCA),y)
CFLAGS += -I$(TOP)/shared/sysdeps/qca -I$(TOP)/wireless_tools
CFLAGS += $(if $(or $(RTCONFIG_SOC_IPQ8064),$(RTCONFIG_SOC_IPQ8074)),-DSMP)
endif
ifeq ($(RTCONFIG_WBD),y)
CFLAGS += -D__CONFIG_WBD__
endif

ifeq ($(RTCONFIG_ALPINE),y)
CFLAGS += -I$(TOP)/shared/sysdeps/alpine -I$(TOP)/wireless_tools -I$(TOP)/mtd-utils/include/
endif

ifeq ($(RTCONFIG_LANTIQ),y)
CFLAGS += -I$(TOP)/shared/sysdeps/lantiq -I$(TOP)/wireless_tools -I$(TOP)/fapi_wlan_common-05.04.00.131/include -I$(TOP)/libhelper-1.4.0.2/include -DPACKAGE_ID=\"fapi_wlan_common\" -DLOGGING_ID="fapi_wlan_common"
CFLAGS  += -I$(TOP)/wlc_nt
endif

ifeq ($(RTCONFIG_REALTEK),y)
LDFLAGS = -L$(TOP)/nvram${BCMEX} -lnvram -L$(TOP)/shared -lshared -lcrypt -ldl
else
ifeq ($(TOP_PLATFORM),)
LDFLAGS += -L$(TOP)/nvram${BCMEX}${EX7} -lnvram -L$(TOP)/shared -lshared -lcrypt -ldl
else
LDFLAGS += -L$(TOP_PLATFORM)/nvram${BCMEX}${EX7} -lnvram -L$(TOP)/shared -lshared -lcrypt -ldl
endif
endif

ifeq ($(HND_ROUTER),y)
CFLAGS += -I$(HND_SRC)/userspace/public/include -I$(HND_SRC)/userspace/public/include/linux -I$(HND_SRC)/userspace/private/include
ifeq ($(TOP_PLATFORM),)
LDFLAGS += -L$(TOP)/wlcsm -lwlcsm -L$(TOP)/bcm_flashutil -lbcm_flashutil -L$(TOP)/bcm_util -lbcm_crc -L$(TOP)/bcm_boardctl -lbcm_boardctl
else
ifeq ($(HND_ROUTER_AX_675X),y)
LDFLAGS += -L$(TOP_PLATFORM)/wlcsm -lwlcsm -L$(TOP_PLATFORM)/bcm_flashutil -lbcm_flashutil -L$(TOP_PLATFORM)/gen_util -lgen_util -L$(TOP_PLATFORM)/sys_util -lsys_util -L$(TOP_PLATFORM)/bcm_util -lbcm_util -L$(TOP_PLATFORM)/bcm_boardctl -lbcm_boardctl
else
LDFLAGS += -L$(TOP_PLATFORM)/wlcsm -lwlcsm -L$(TOP_PLATFORM)/bcm_flashutil -lbcm_flashutil -L$(TOP_PLATFORM)/bcm_util -lbcm_crc -L$(TOP_PLATFORM)/bcm_boardctl -lbcm_boardctl
endif
endif
endif

ifeq ($(RTCONFIG_BCMARM),y)
CFLAGS += -I$(SRCBASE)/shared/bcmwifi/include -DTYPEDEF_FLOAT_T
CFLAGS += -I$(SRCBASE)/common/include
ifneq ($(findstring CONFIG_DUMP_PREV_OOPS_MSG=, $(shell cat $(LINUXDIR)/.config)),)
CFLAGS += -DDUMP_PREV_OOPS_MSG
endif
LDFLAGS += -L$(TOP)/libbcm -lbcm
LDFLAGS += -lgcc_s
ifeq ($(RTCONFIG_QTN),y)
CFLAGS += -I$(TOP)/libqcsapi_client -I$(TOP)/shared/sysdeps/qtn
LDFLAGS += -L$(TOP)/libqcsapi_client -lqcsapi_client
endif
endif

ifeq ($(RTCONFIG_QSR10G),y)
CFLAGS += -I$(TOP)/libqcsapi_client_10g
LDFLAGS += -L$(TOP)/libqcsapi_client_10g -lqcsapi_client
endif

ifeq ($(RTCONFIG_LANTIQ),y)
LDFLAGS += -L$(TOP)/fapi_wlan_common-05.04.00.131 -lfapiwlancommon -ldl -L$(TOP)/libhelper-1.4.0.2 -lhelper -L$(TOP)/libfapi-0.1 -lfapi
LDFLAGS += -L$(TOP)/wlc_nt -lwlc_nt_client
endif

ifeq ($(RTCONFIG_HTTPS),y)
CFLAGS += -I$(TOP)/openssl/include
LDFLAGS += -L$(TOP)/openssl -lcrypto -lssl $(if $(RTCONFIG_OPENSSL11),-lpthread,)
endif

ifeq ($(RTCONFIG_BWDPI),y)
CFLAGS += -I$(TOP)/bwdpi_source/asus_include -I$(TOP)/bwdpi_source/src/tdts_udb_sample/linux
CFLAGS += -I$(TOP)/bwdpi_source/src/tdts_udb_sample
CFLAGS += -I$(TOP)/bwdpi_source/include
CFLAGS += -I$(TOP)/httpd/
CFLAGS += -I$(TOP)/util-linux/include
LDFLAGS += -L$(TOP)/bwdpi_source/asus -lbwdpi
endif

ifeq ($(RTCONFIG_SPEEDTEST),y)
LDFLAGS += -lm
LDFLAGS += -lpthread
endif

ifeq ($(RTCONFIG_BCMSMP),y)
CFLAGS += -DSMP
endif

ifeq ($(RTCONFIG_4352_5G),y)
CFLAGS += -DBCM4352
endif

ifeq ($(RTCONFIG_USB),y)
CFLAGS += -I$(TOP)/libdisk
CFLAGS += -DU2EC
LDFLAGS += -L$(TOP)/libdisk -ldisk
ifeq ($(BCM_MMC),y)
CFLAGS += -DBCM_MMC
endif
ifeq ($(RTCONFIG_USB_SMS_MODEM),y)
CFLAGS += -I$(TOP)/smspdu
LDFLAGS += -L$(TOP)/smspdu -lsmspdu
endif

ifeq ($(RTCONFIG_USB_MODEM),y)
CFLAGS += -DSET_USB_MODEM_MTU_ETH
CFLAGS += -DSET_USB_MODEM_MTU_PPP
ifeq ($(RTCONFIG_USB_SMS_MODEM),y)
CFLAGS += -I$(TOP)/smspdu
LDFLAGS += -L$(TOP)/smspdu -lsmspdu
endif
endif
endif

ifeq ($(RTCONFIG_HTTPS),y)
ifeq ($(RTCONFIG_FRS_FEEDBACK),y)
CFLAGS  += -I$(TOP)/openssl/include
LDFLAGS += -L$(TOP)/openssl -lcrypto -lssl
endif
endif

ifeq ($(RTCONFIG_FBWIFI),y)
CFLAGS  += -I$(TOP)/fb_wifi
LDFLAGS += -L$(TOP)/fb_wifi -lfbwifi
endif

ifeq ($(RTCONFIG_NOTIFICATION_CENTER),y)
CFLAGS  += -I$(TOP)/sqlite
CFLAGS  += -I$(TOP)/nt_center/lib
CFLAGS  += -I$(TOP)/nt_center/actMail
CFLAGS  += -DASUSWRT_SDK
endif

ifeq ($(RTCONFIG_PROTECTION_SERVER),y)
CFLAGS  += -I$(TOP)/protect_srv/include
endif

ifeq ($(RTCONFIG_BT_CONN),y)
CFLAGS  += -I$(TOP)/bluez-5.41/lib
endif


ifneq ($(findstring NET_CLS_POLICE=, $(shell cat $(LINUXDIR)/.config)),)
else
ifneq ($(findstring IMQ=, $(shell cat $(LINUXDIR)/.config)),)
export CFLAGS += -DCLS_ACT
endif
endif

ifneq ($(findstring DEVTMPFS=, $(shell cat $(LINUXDIR)/.config)),)
export CFLAGS += -DDEVTMPFS
endif

ifeq ($(RA_SKU),y)
export CFLAGS += -DRA_SINGLE_SKU
endif

ifeq ($(RA_SKU_IN_DRV),y)
export CFLAGS += -DRA_SINGLE_SKU
endif

ifeq ($(or $(RTAC68A),$(RT4GAC68U)),y)
CFLAGS += -DRTAC68U
endif

ifeq ($(or $(RTAC85U),$(RTAC85P),$(RTACRH26),$(TUFAC1750)),y)
TARGET_MODEL = $(subst -,,$(subst +,P,$(BUILD_NAME)))
CFLAGS += -D$(TARGET_MODEL)
endif

ifeq ($(RTCONFIG_PERMISSION_MANAGEMENT),y)
CFLAGS += -I$(TOP)/sqlCipher
CFLAGS += -I$(TOP)/PMS_DBapis
LDFLAGS += -L $(TOP)/sqlCipher/.libs -lsqlcipher
LDFLAGS += -L$(TOP)/PMS_DBapis -lpms_sql
endif

LDFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections
ifeq ($(STATIC),1)
LDFLAGS += -static
endif
LDFLAGS += $(EXTRA_LDFLAGS)

ifeq ($(RTCONFIG_TRACEROUTE),y)
CFLAGS += -DRTCONFIG_TRACEROUTE
endif

ifeq ($(HND_ROUTER),y)
CFLAGS += -DNON_P_THREAD -DWATCHDOG_PERIOD2
endif

ifeq ($(RTCONFIG_CFGSYNC),y)
CFLAGS += -I$(TOP)/cfg_mnt
LDFLAGS += -L$(TOP)/cfg_mnt -lcfgmnt
endif

ifeq ($(RTCONFIG_SW_HW_AUTH), y)
CFLAGS += -I$(TOP)/sw-hw-auth
endif

ifeq ($(RTCONFIG_LETSENCRYPT),y)
CFLAGS += -DRTCONFIG_CROND
CFLAGS += -I$(TOP)/libletsencrypt
LDFLAGS += -L$(TOP)/libletsencrypt -lletsencrypt
endif

ifeq ($(RTCONFIG_OPENVPN),y)
CFLAGS += -DRTCONFIG_CROND
CFLAGS += -I$(TOP)/libvpn
LDFLAGS += -L$(TOP)/libvpn -lvpn
OBJS += ovpn.o
endif


OBJS := amas_wgn_test.o

OBJS += $(TOP)/rc/init.o $(TOP)/rc/interface.o $(TOP)/rc/lan.o $(TOP)/rc/wireless.o $(TOP)/rc/wan.o $(TOP)/rc/pppd.o $(TOP)/rc/auth.o
OBJS += $(TOP)/rc/firewall.o $(TOP)/rc/ppp.o $(TOP)/rc/services.o $(TOP)/rc/common.o
OBJS += $(TOP)/rc/watchdog.o  $(if $(RTCONFIG_MULTIWAN_CFG), $(TOP)/rc/qos.o) $(TOP)/rc/udhcpc.o $(TOP)/rc/ate.o
OBJS += $(TOP)/rc/format.o

ifeq ($(RTCONFIG_AMAS_WGN), y)
OBJS += $(TOP)/rc/amas_wgn.o
endif

ifeq ($(CONFIG_BCMWL5),y)
OBJS += $(TOP)/rc/init-broadcom.o $(TOP)/rc/wps-broadcom.o $(TOP)/rc/broadcom.o $(TOP)/rc/ate-broadcom.o
ifeq ($(RTCONFIG_AMAS),y)
OBJS += $(TOP)/rc/obd-broadcom.o
ifeq ($(RTCONFIG_BHCOST_OPT),y)
OBJS += $(TOP)/rc/amas-ssd-broadcom.o
endif
endif
ifeq ($(BUILD_NAME),$(filter $(BUILD_NAME),RT-AC68U RT-AC3200 4G-AC68U GT-AC2900))
OBJS += $(TOP)/rc/cfe.o
endif
ifeq ($(RTCONFIG_ADTBW),y)
OBJS += $(TOP)/rc/adtbw-broadcom.o
endif
ifeq ($(RTCONFIG_DSL),y)
OBJS += $(TOP)/rc/init-broadcom-dsl.o
endif
else ifeq ($(RTCONFIG_REALTEK),y)
OBJS += $(TOP)/rc/init-realtek.o $(TOP)/rc/wps-realtek.o $(TOP)/rc/realtek.o $(TOP)/rc/rtk_wifi_drvmib.o $(TOP)/rc/ate-realtek.o
ifeq ($(RTCONFIG_AMAS),y)
OBJS += $(TOP)/rc/obd-realtek.o
ifeq ($(RTCONFIG_BHCOST_OPT),y)
OBJS += $(TOP)/rc/amas-ssd-realtek.o
endif
endif
else ifeq ($(RTCONFIG_RALINK),y)
OBJS += $(TOP)/rc/init-ralink.o $(TOP)/rc/wps-ralink.o $(TOP)/rc/ralink.o $(TOP)/rc/ate-ralink.o
ifeq ($(RTCONFIG_AMAS),y)
OBJS += $(TOP)/rc/obd-ralink.o
ifeq ($(RTCONFIG_BHCOST_OPT),y)
OBJS += $(TOP)/rc/amas-ssd-ralink.o
endif
endif
ifeq ($(RTCONFIG_CONCURRENTREPEATER),y)
OBJS += $(TOP)/rc/air_monitor.o
endif
ifeq ($(RTCONFIG_DSL),y)
OBJS += $(TOP)/rc/init-ralink-dsl.o
endif
else ifeq ($(RTCONFIG_QCA),y)
OBJS += $(TOP)/rc/init-qca.o $(TOP)/rc/wps-qca.o $(TOP)/rc/qca.o $(TOP)/rc/ate-qca.o $(TOP)/rc/ctl.obj
ifeq ($(RTCONFIG_AMAS),y)
OBJS += $(TOP)/rc/obd-qca.o
ifeq ($(RTCONFIG_BHCOST_OPT),y)
OBJS += $(TOP)/rc/amas-ssd-qca.o
endif
endif
ifeq ($(RTCONFIG_WIFI_SON),y)
OBJS += $(TOP)/rc/repacd.o
endif
else ifeq ($(RTCONFIG_ALPINE),y)
OBJS += $(TOP)/rc/init-alpine.o $(TOP)/rc/wps-alpine.o $(TOP)/rc/alpine.o $(TOP)/rc/ate-alpine.o
ifeq ($(RTCONFIG_AMAS),y)
OBJS += $(TOP)/rc/obd-alpine.o
ifeq ($(RTCONFIG_BHCOST_OPT),y)
OBJS += $(TOP)/rc/amas-ssd-alpine.o
endif
endif
else ifeq ($(RTCONFIG_LANTIQ),y)
OBJS += $(TOP)/rc/init-lantiq.o $(TOP)/rc/lantiq.o $(TOP)/rc/ate-lantiq.o $(TOP)/rc/lantiq-wave.o
OBJS += $(if $(wildcard sysdeps/wps-lantiq.c),wps-lantiq.o,prebuild/wps-lantiq.o)
ifeq ($(RTCONFIG_AMAS),y)
OBJS += $(if $(wildcard sysdeps/lantiq/obd-lantiq.c),obd-lantiq.o,prebuild/obd-lantiq.o)
endif
endif

ifeq ($(RTCONFIG_TCODE),y)
OBJS += $(TOP)/rc/tcode_rc.o
ifeq ($(CONFIG_BCMWL5),y)
OBJS += $(TOP)/rc/tcode_brcm.o
endif
endif

ifeq ($(RTCONFIG_USB),y)
OBJS += $(TOP)/rc/usb.o
OBJS += $(TOP)/rc/usb_devices.o
endif

ifeq ($(RTCONFIG_JFFS2),y)
OBJS += $(TOP)/rc/jffs2.o
endif

ifeq ($(RTCONFIG_BRCM_NAND_JFFS2),y)
OBJS += $(TOP)/rc/jffs2.o
endif

ifeq ($(RTCONFIG_TIMEMACHINE),y)
OBJS += $(TOP)/rc/timemachine.o
endif

ifeq ($(RTCONFIG_HTTPS),y)
ifeq ($(RTCONFIG_FRS_FEEDBACK),y)
OBJS += $(TOP)/rc/dsl_fb.o
endif
endif

ifeq ($(RTCONFIG_AMAS),y)
OBJS += $(TOP)/rc/amas_lib.o
OBJS += $(TOP)/rc/linklist.o
endif

ifeq ($(RTCONFIG_NOTIFICATION_CENTER),y)
OBJS += $(TOP)/rc/nt_mail.o
endif

ifeq ($(RTCONFIG_BWDPI),y)
OBJS += $(TOP)/rc/bwdpi.o
OBJS += $(TOP)/rc/bwdpi_check.o
OBJS += $(TOP)/rc/bwdpi_wred_alive.o
OBJS += $(TOP)/rc/bwdpi_db_10.o
OBJS += $(TOP)/rc/hour_monitor.o
endif


ifeq ($(RTCONFIG_PARENTALCTRL),y)
OBJS += $(TOP)/rc/pc.o
OBJS += $(TOP)/rc/pc_block.o
OBJS += $(TOP)/rc/pc_tmp.o
endif

ifneq ($(RTCONFIG_RALINK)$(RTCONFIG_QCA)$(RTCONFIG_REALTEK),y)
OBJS += $(TOP)/rc/erp_monitor.o
endif

ifeq ($(RTCONFIG_TUNNEL),y)
OBJS += $(TOP)/rc/natnl_api.o
endif

ifeq ($(RTCONFIG_SSH),y)
OBJS += $(TOP)/rc/ssh.o
endif

ifeq ($(RTCONFIG_IPSEC),y)
OBJS += $(TOP)/rc/rc_ipsec.o
endif
OBJS += $(TOP)/rc/vpn.o

ifeq ($(RTCONFIG_WTFAST),y)
OBJS += $(TOP)/rc/wtfast.o
endif

OBJS += $(TOP)/rc/private.o

ifeq ($(RTCONFIG_VPNC),y)
ifeq ($(RTCONFIG_VPN_FUSION),y)
OBJS += $(TOP)/rc/vpnc_fusion.o
else
OBJS += $(TOP)/rc/vpnc.o
endif
endif

ifeq ($(RTAC1200G),y)
OBJS += $(TOP)/rc/mtd_sflash.o
else
OBJS += $(TOP)/rc/mtd.o
endif

ifeq ($(RTCONFIG_HTTPS),y)
CFLAGS += -I$(TOP)/curl-7.21.7/include
LDFLAGS += -L$(TOP)/curl-7.21.7/lib/.libs -lcurl
ifeq ($(or $(RTCONFIG_PUSH_EMAIL),$(RTCONFIG_NOTIFICATION_CENTER)),y)
OBJS += $(TOP)/rc/pwdec.o
CFLAGS += -I$(TOP)/json-c
LDFLAGS += -L$(TOP)/json-c/.libs -ljson-c
endif
endif

ifeq ($(RTCONFIG_USB),y)
CFLAGS += -I$(TOP)/libdisk
CFLAGS += -DU2EC
LDFLAGS += -L$(TOP)/libdisk -ldisk
ifeq ($(BCM_MMC),y)
CFLAGS += -DBCM_MMC
endif
ifeq ($(RTCONFIG_USB_SMS_MODEM),y)
CFLAGS += -I$(TOP)/smspdu
LDFLAGS += -L$(TOP)/smspdu -lsmspdu
endif
ifeq ($(RTCONFIG_USB_MODEM),y)
CFLAGS += -DSET_USB_MODEM_MTU_ETH
CFLAGS += -DSET_USB_MODEM_MTU_PPP
ifeq ($(RTCONFIG_USB_SMS_MODEM),y)
CFLAGS += -I$(TOP)/smspdu
LDFLAGS += -L$(TOP)/smspdu -lsmspdu
endif
endif
endif

LDFLAGS += -L$(TOP)/libvpn -lvpn
LDFLAGS += -L$(TOP)/openssl -lcrypto -lssl
LDFLAGS += -L$(TOP)/cfg_mnt -lcfgmnt
LDFLAGS += -lm -lpthread
LDFLAGS += -L$(TOP)/sqlite/.libs -lsqlite3
LDFLAGS += -L$(TOP)/libasuslog -lasuslog

ifeq ($(RTCONFIG_CONNDIAG),y)
ifeq ($(RTCONFIG_BCMBSD),y)
CFLAGS += -I$(TOP)/bsd
endif
ifeq ($(RTCONFIG_LANTIQ),y)
CFLAGS += -fPIC
endif
ifeq ($(RTCONFIG_HND_ROUTER_AX),y)
CFLAGS += $(ACSDV2_DIR)
endif
CFLAGS += -I$(TOP)/acsd$(BCMEX)$(EX7)
LDFLAGS += -L$(TOP)/sqlite/.libs -lsqlite3
LDFLAGS += -L$(TOP)/rc -lconn_diag

OBJS += $(TOP)/rc/conn_diag.o
ifeq ($(CONFIG_BCMWL5),y)
OBJS += $(TOP)/rc/conn_diag-broadcom.o
else ifeq ($(RTCONFIG_LANTIQ),y)
OBJS += $(TOP)/rc/conn_diag-lantiq.o
else ifeq ($(RTCONFIG_QCA),y)
OBJS += $(TOP)/rc/conn_diag-qca.o
endif
DIAG_OBJS += $(TOP)/rc/conn_diag-sql.o
endif

##ifeq ($(HND_ROUTER),y)
##CFLAGS += -I$(HND_SRC)/userspace/public/include -I$(HND_SRC)/userspace/public/include/linux -I$(HND_SRC)/userspace/private/include
##LDFLAGS += -L$(TOP)/wlcsm -lwlcsm -L$(TOP)/bcm_flashutil -lbcm_flashutil -L$(TOP)/bcm_util -lbcm_crc -L$(TOP)/bcm_boardctl -lbcm_boardctl
##endif

ifeq ($(RTCONFIG_UNIT_TEST),y)
CFLAGS += -I$(TOP)/shared
CFLAGS += -I$(TOP)/rc
endif

all: amas_wgn_test

amas_wgn_test: $(OBJS)
	@echo " [amas_wgn_test] CC $@"
	@$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

	$(SIZECHECK)
	$(CPTMP)

clean:
	@echo clean ...
	@rm -rf *.o
	@rm -rf amas_wgn_test
