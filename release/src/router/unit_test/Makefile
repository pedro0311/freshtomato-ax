export HND_ROUTER := $(shell pwd | sed 's/.*hnd.*/y/g' | sed 's/.*src.*//g')
ifeq ($(HND_ROUTER_AX),)
export HND_ROUTER_AX := $(shell pwd | sed 's/.*axhnd.*/y/g' | sed 's/.*src.*//g')
endif

ifeq ($(HND_ROUTER_AX_675X),)
export HND_ROUTER_AX_675X := $(shell pwd | sed 's/.*axhnd.675x.*/y/g' | sed 's/.*src.*//g')
endif

ifeq ($(HND_ROUTER_AX_675X),y)
ifeq ($(CUR_CHIP_PROFILE),)
export CHIP_CONFIG_DIR := $(shell pwd | sed 's/\(.*src-rt-.*hnd.*\).*/\1/')
include $(CHIP_CONFIG_DIR)/../chip_profile.tmp
endif
endif

ifeq ($(HND_ROUTER),y)
ifeq ($(HND_ROUTER_AX_675X),y)
export BCM_CHIP := $(CUR_CHIP_PROFILE)
else
export BCM_CHIP := 4908
endif
export HND_SRC := $(shell pwd | sed 's/\(.*src-rt-.*hnd.*\).*/\1/' | sed 's|\/router-sysdep|\/|g' | sed 's|\/router/unit_test|\/|g')
export SRCBASE := $(shell pwd | sed 's/\(.*src-rt-.*hnd.*\).*/\1/' | sed 's|\/router-sysdep|\/bcmdrivers\/broadcom\/net\/wl\/bcm9$(BCM_CHIP)\/main\/src\/|g' | sed 's|\/router/unit_test|\/bcmdrivers\/broadcom\/net\/wl\/bcm9$(BCM_CHIP)\/main\/src\/|g')
PREBUILT_BCMBIN=$(shell if [ ! -f "$(HND_SRC)/upb.mak" -o "$(PRBM)" = "1" ]; then echo 1; else echo 0; fi)
export PREBUILT_BCMBIN
export PRBM_DIR=prebuilt
include $(SRCBASE)/.config
ifeq ($(BCM_FSBUILD_DIR),)
include $(HND_SRC)/make.common
endif
include $(HND_SRC)/targets/$(PROFILE)/$(PROFILE).$(BUILD_NAME)
BUILD_DIR:=$(HND_SRC)
export ALLSRCBASE= $(BUILD_DIR)/bcmdrivers/broadcom/net/wl/bcm9$(BRCM_CHIP)
export WLSRCBASE= $(ALLSRCBASE)/main/src
export WLCFGDIR = $(ALLSRCBASE)/main/src/wl/config
export WLMAIN = main/src
ifeq ($(HND_ROUTER_AX),y)
include $(SRCBASE)/makefiles/WLAN_Common.mk
WLAN_ComponentsInUse := avs awd bcmcrypto bcmdrv bcmwifi clm clm-api ppr math
export WLAN_ComponentSrcDirsA
export WLAN_ComponentIncPathA
export WLAN_StdSrcDirsA
export WLAN_StdIncPathA
export BCM_EXTFDIMAGE_PATH=/etc/wlan/dhd
endif
else
export HND_ROUTER := n
endif

include ../common.mak
ifneq ($(HND_ROUTER),y)
include $(SRCBASE)/.config
endif

ifeq ($(RTCONFIG_RALINK),y)
else ifeq ($(or $(RTCONFIG_QCA),$(RTCONFIG_REALTEK)),y)
export CFLAGS += $(EXTRACFLAGS)
else
# Broadcom
export CFLAGS += -DBCMWPA2
ifeq ($(RTCONFIG_BCMWL6),y)
export CFLAGS += -DBCMQOS
export CFLAGS += -DEXT_ACS
export CFLAGS += -DD11AC_IOTYPES
export CFLAGS += -DPHYMON
export CFLAGS += -DPROXYARP
export CFLAGS += -DTRAFFIC_MGMT
export CFLAGS += -DTRAFFIC_MGMT_RSSI_POLICY
ifeq ($(RTCONFIG_BCMARM),y)
export CONFIG_MFP=y
export CFLAGS += -DMFP
export CFLAGS += -D__CONFIG_MFP__
ifeq ($(RTCONFIG_BCM_MFG),y)
export BCM_MFG=y
export CFLAGS += -DBCM_MFG
endif
endif
endif
ifeq ($(HND_ROUTER),n)
export CFLAGS += $(EXTRACFLAGS)
else
export CFLAGS += -DHND_ROUTER -DBCA_HNDROUTER -DMCPD_PROXY
export EXTRA_NV_LDFLAGS := -L$(TOP)/wlcsm -lwlcsm
export EXTRA_LDFLAGS := -lgcc_s
ifeq ($(TOPBUILD),y)
export CFLAGS += $(HND_CMN_CFLAGS)
endif
endif
endif

# New Broadcom/Ralink/MTK/QCA/RTK platform.
KPATH_2636_OR_3X = $(if $(or $(findstring linux-2.6.36,$(LINUXDIR)),$(findstring linux-3.x,$(LINUXDIR)),$(findstring linux-3.3.x,$(LINUXDIR)),$(findstring linux-3.3.8,$(LINUXDIR)),$(findstring linux-3.4.x,$(LINUXDIR)),$(findstring linux-4.1,$(LINUXDIR)),$(findstring linux-3.10,$(LINUXDIR)),$(findstring linux-3.14.x,$(LINUXDIR)),$(findstring linux-4.4.x,$(LINUXDIR))),y)

# New Ralink/MTK/QCA platform.
KPATH_2636X_OR_3X = $(if $(or $(findstring linux-2.6.36.x,$(LINUXDIR)),$(findstring linux-3.x,$(LINUXDIR)),$(findstring linux-3.3.x,$(LINUXDIR)),$(findstring linux-3.3.8,$(LINUXDIR)),$(findstring linux-3.4.x,$(LINUXDIR)),$(findstring linux-3.14.x,$(LINUXDIR)),$(findstring linux-4.4.x,$(LINUXDIR))),y)

# New Ralink/MTK/QCA/RTK platform.
KPATH_3X = $(if $(or $(findstring linux-3.x,$(LINUXDIR)),$(findstring linux-3.3.x,$(LINUXDIR)),$(findstring linux-3.3.8,$(LINUXDIR)),$(findstring linux-3.4.x,$(LINUXDIR)),$(findstring linux-3.10,$(LINUXDIR)),$(findstring linux-3.14.x,$(LINUXDIR)),$(findstring linux-4.4.x,$(LINUXDIR))),y)

KPATH_37_OR_ABOVE = $(if $(or $(findstring linux-3.10,$(LINUXDIR)),$(findstring linux-3.14,$(LINUXDIR)),$(findstring linux-4.4.x,$(LINUXDIR))),y)
KPATH_314_OR_ABOVE = $(if $(or $(findstring linux-3.14,$(LINUXDIR)),$(findstring linux-4.4.x,$(LINUXDIR))),y)

export KPATH_37_OR_ABOVE KPATH_314_OR_ABOVE

INCLUDE_LINUX_VERSION_H = $(if $(or $(RTCONFIG_LANTIQ),$(KPATH_314_OR_ABOVE)),include/generated/uapi/linux/version.h,include/linux/version.h)

ifneq ($(BASE_MODEL),)
ifneq ($(findstring 4G-,$(BASE_MODEL)),)
MODEL = RT$(subst -,,$(BASE_MODEL))
else ifneq ($(findstring DSL,$(BASE_MODEL)),)
MODEL = $(subst -,_,$(BASE_MODEL))
else
MODEL = $(subst -,,$(subst +,P,$(BASE_MODEL)))
endif
else ifneq ($(BUILD_NAME),)
ifneq ($(findstring 4G-,$(BUILD_NAME)),)
MODEL = RT$(subst -,,$(BUILD_NAME))
else ifneq ($(findstring DSL,$(BUILD_NAME)),)
MODEL = $(subst -,_,$(BUILD_NAME))
else
MODEL = $(subst -,,$(subst +,P,$(BUILD_NAME)))
endif
endif
ifneq ($(MODEL),)
export CFLAGS += -D$(MODEL)
export $(MODEL)=y
endif

export OLD_SRC=$(SRCBASE)/../src
ifeq ($(RTCONFIG_REALTEK),y)
# do nothing #
else ifeq ($(RTCONFIG_BCMARM),y)
#export CFLAGS += -I$(SRCBASE)/common/include

ifeq ($(HND_ROUTER),y)
export LINUX_VERSION=4_1_0
export LINUX_KERNEL=4.1.27
export KBUILD_VERBOSE := 1
export BUILD_MFG := 0
SUBMAKE_SETTINGS = SRCBASE=$(SRCBASE) BASEDIR=$(BASEDIR)
else
LINUX_VERSION=2_6_36
LINUX_KERNEL=2.6.36
endif
export PLATFORM_ARCH LIBDIR USRLIBDIR LINUX_VERSION

ifeq ($(RTCONFIG_BCM7),y)
export BCMSRC=src-rt-7.x.main/src
export OLD_SRC=$(SRCBASE)/../../src
export BUILD_MFG := 0
export EX7 := _7
else ifeq ($(HND_ROUTER_AX),y)
export BCMSRC=src-rt-5.02axhnd
export OLD_SRC=$(HND_SRC)/../src
export BUILD_MFG := 0
export BCMEX :=
export EX7 :=
export DHD_MONITOR := debug_monitor
export LIBNL_PLT := arm
export LIBNL_BUILD := i386-pc-linux-gnu
else ifeq ($(HND_ROUTER),y)
export BCMSRC=src-rt-5.02hnd
export OLD_SRC=$(HND_SRC)/../src
export BUILD_MFG := 0
export EX7 := _94908hnd
export DHD_MONITOR := dhd_monitor
else ifeq ($(RTCONFIG_BCM_7114),y)
export BCMSRC=src-rt-7.14.114.x/src
export OLD_SRC=$(SRCBASE)/../../src
export BUILD_MFG := 0
export EX7 := _7114
export DHD_MONITOR := dhd_monitor
else ifeq ($(RTCONFIG_BCM9),y)
export BCMSRC=src-rt-9.x/src
export OLD_SRC=$(SRCBASE)/../../src
export BUILD_MFG := 0
export EX7 := _9
else
WLAN_ComponentsInUse := bcmwifi clm ppr olpc
export BCMSRC=src-rt-6.x.4708
endif
ifeq ($(HND_ROUTER),y)
export EXTRA_NV_LDFLAGS := -L$(TOP_PLATFORM)/wlcsm -lwlcsm
export EXTRALDFLAGS = -lgcc_s
export LD_LIBRARY_PATH := $(TOOLCHAIN)/lib
export BASEDIR := $(SRCBASE)/../
else
include $(SRCBASE)/makefiles/WLAN_Common.mk
export BASEDIR := $(WLAN_TreeBaseA)
export LINUXDIR := $(SRCBASE)/linux/linux-2.6.36

export EXTRALDFLAGS = -lgcc_s
export EXTRALDFLAGS2 = -L$(TOP)/nvram$(BCMEX)$(EX7) -lnvram -L$(TOP)/shared -lshared


export LD_LIBRARY_PATH := $(SRCBASE)/toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3/lib
endif
ifeq (2_6_36,$(LINUX_VERSION))
export BUILD_MFG := 0
endif
SUBMAKE_SETTINGS = SRCBASE=$(SRCBASE) BASEDIR=$(BASEDIR)
SUBMAKE_SETTINGS += ARCH=$(ARCH)
export CFLAGS += -O2
export OPTCFLAGS = -O2
WLCFGDIR=$(SRCBASE)/wl/config

CRAMFSDIR := cramfs

export MKSYM := $(shell which $(TOP)/misc/mksym.pl)
endif

ifeq ($(RTCONFIG_REALTEK),y)
# do nothing #
else ifeq ($(RTCONFIG_BCMWL6),y)
ifneq ($(RTCONFIG_BCMARM),y)
LINUX_VERSION=2_6
LINUX_KERNEL=2.6.22.19
endif
ifeq ($(or $(RTCONFIG_BCMWL6),$(RTAC53U)),y)
WLAN_ComponentsInUse := bcmwifi clm ppr
ifeq ($(or $(RTCONFIG_BCMWL6A),$(RTAC53U)),y)
WLAN_ComponentsInUse += olpc
endif
-include ../../$(SRCBASE)/makefiles/WLAN_Common.mk
endif
endif

ifeq ($(HND_ROUTER_AX),y)
ifeq ($(RTCONFIG_BCM_MEVENTD),y)
export CFLAGS += -DBCM_MEVENTD
endif
ACSDV2_DIR := $(BASEDIR)/components/apps/acsdv2
ifeq ($(RTCONFIG_BCM_CEVENTD),y)
CEVENT_APP_DIR := $(BASEDIR)/components/apps/cevent_app
export CFLAGS += -D__CONFIG_BCM_CEVENT__ -DBCM_CEVENT -DBCM_CEVENTD
export CONFIG_BCM_CEVENT=y
endif
ifeq ($(RTCONFIG_BRCM_HOSTAPD),y)
export CFLAGS += -DCONFIG_HOSTAPD
export CFLAGS += -I$(SRCBASE)/include -I$(SRCBASE)/../components/bcmcrypto/include
export CFLAGS += -I$(HND_SRC)/bcmdrivers/opensource/include/bcm963xx -I$(HND_SRC)/shared/opensource/include/bcm963xx
export IEEE1905SOURCE_DIR := $(BASEDIR)/components/apps/ieee1905
export WPSSOURCE_DIR := $(BASEDIR)/components/apps/wps
export LIBNL_DIR := $(BASEDIR)/components/opensource/router_tools/libnl
export PKG_CONFIG_PATH := $(LIBNL_DIR)/install/lib/pkgconfig:$(PKG_CONFIG_PATH)
HOSTAPD_DIR := $(BASEDIR)/components/opensource/router_tools/hostapd
export WPA_SUPPLICANT_DIR := $(BASEDIR)/components/opensource/router_tools/wpa_supplicant
IW_DIR := $(BASEDIR)/components/opensource/router_tools/iw
endif
endif


ifeq ($(RTCONFIG_REALTEK),y)
KDIR=$(TOP)/kernel_header
KERNEL_HEADER_DIR=$(TOP)/kernel_header/include
else ifeq ($(RTCONFIG_LANTIQ),y)
KDIR=$(TOP)/kernel_header
KERNEL_HEADER_DIR=$(TOP)/kernel_header/include
else ifneq ($(RTCONFIG_BCMARM),y)
KDIR=$(TOP)/kernel_header
KERNEL_HEADER_DIR=$(TOP)/kernel_header/include
else
ifeq ($(HND_ROUTER),y)
KDIR=$(TOOLCHAIN)/arm-buildroot-linux-gnueabi/sysroot/usr
KERNEL_HEADER_DIR=$(KDIR)/include
TOOL_SYS_INCLUDE=$(TOOLCHAIN)/arm-buildroot-linux-gnueabi/sysroot/usr/include
ifeq ($(RTCONFIG_VISUALIZATION),y)
export TOOLCHAIN_LIB_DIR=$(KDIR)/lib
export SRCBASE_ROUTER=$(SRCBASE)/router
endif
export CROSS_COMPILE_64 := /opt/toolchains/crosstools-aarch64-gcc-5.3-linux-4.1-glibc-2.22-binutils-2.25/usr/bin/aarch64-buildroot-linux-gnu-
export STRIP_64 := $(CROSS_COMPILE_64)strip
export TOOLCHAIN_64 := $(shell cd $(dir $(shell which $(CROSS_COMPILE_64)gcc))/.. && pwd -P)
export TOOL_SYS_INCLUDE_64=$(TOOLCHAIN_64)/aarch64-buildroot-linux-gnu/sysroot/usr/include
export PATH:=$(PATH):/opt/toolchains/crosstools-aarch64-gcc-5.3-linux-4.1-glibc-2.22-binutils-2.25/usr/bin
else
KDIR=$(LINUXDIR)
KERNEL_HEADER_DIR=$(LINUXDIR)/include
endif
endif

ifeq ($(HND_ROUTER),y)
export LINUX_INC_DIR := $(TOOLCHAIN)/arm-buildroot-linux-gnueabi/sysroot/usr
else
export LINUX_INC_DIR := $(LINUXDIR)
endif

ifeq ($(HND_ROUTER),y)
ifeq ($(RTCONFIG_VISUALIZATION),y)
VISSOURCE_BASE_DIR := $(BASEDIR)/components/apps/visualization
VISDCOLL_DIR := $(VISSOURCE_BASE_DIR)/datacollector
VISDCON_DIR := $(VISSOURCE_BASE_DIR)/dataconcentrator
export SQLITE3_DIR := $(BASEDIR)/components/opensource/sqlite3
endif
export WBDSOURCE_DIR := $(BASEDIR)/components/apps/wbd
export EX_ROM := rom
else ifeq ($(RTCONFIG_BCM_7114),y)
export WBDSOURCE_DIR := $(BASEDIR)/components/apps/wbd
endif

ifeq ($(RTCONFIG_WLEXE),y)
ifeq ($(RTCONFIG_BCM9),y)
export SRCBASE_SYS := $(SRCBASE)/../sys/src
export CONFIG_WLEXE
export RWL ?= 0
endif
endif

ifeq ($(RTCONFIG_LBR_AGGR), y)
export CONFIG_LBR_AGGR
export CFLAGS += -D__CONFIG_LBR_AGGR__
endif

ifeq ($(RTCONFIG_GMAC3),y)
export CFLAGS += -D__CONFIG_GMAC3__
endif

ifeq ($(RTCONFIG_BRCM_USBAP),y)
export CFLAGS += -D__CONFIG_USBAP__
endif

ifeq ($(RTCONFIG_TOAD),y)
export CFLAGS += -D__CONFIG_TOAD__
endif

ifeq ($(RTCONFIG_BCM_APPEVENTD),y)
export CFLAGS += -DBCM_APPEVENTD
endif

ifeq ($(RTCONFIG_BCMBSD),y)
export CFLAGS += -DBCM_BSD
endif

ifeq ($(RTCONFIG_LANTIQ),y)
export CFLAGS += -DLANTIQ_BSD
endif

ifeq ($(RTCONFIG_QCA_LBD),y)
export CFLAGS += -DQCA_BSD
endif

ifeq ($(RTCONFIG_MTK_BSD),y)
export CFLAGS += -DMTK_BSD
endif

ifeq ($(RTCONFIG_BCMSSD),y)
export CFLAGS += -DBCM_SSD
endif

ifeq ($(RTCONFIG_WLCLMLOAD),y)
export CFLAGS += -DWLCLMLOAD
export WLCLMLOAD := 1
endif

# BCA_HNDROUTER mfg flag
ifneq ($(BUILD_HND_MFG),)
export BUILD_MFG := 1
export WLTEST := 1
endif

ifeq ($(RTCONFIG_FBT),y)
export CFLAGS  += -DWLHOSTFBT
endif

ifeq ($(RTCONFIG_BCMASPMD),y)
export CFLAGS += -DBCM_ASPMD
endif

ifeq ($(RTCONFIG_WBD),y)
export CFLAGS += -DBCM_WBD
ifeq ($(RTCONFIG_PLCWBD), y)
export PLC_WBD := 1
endif # PLC_WBD
endif

ifeq ($(RTCONFIG_WBD),y)
export CFLAGS += -DBCM_WBD
ifeq ($(RTCONFIG_PLCWBD), y)
export PLC_WBD := 1
endif # PLC_WBD
endif # WBD

ifeq ($(or $(RTCONFIG_BCMEVENTD),$(RTCONFIG_BCN_RPT)),y)
export CFLAGS += -DBCM_EVENTD
endif

ifeq ($(RTCONFIG_BCMDCS),y)
export CFLAGS += -DBCM_DCS
endif

ifeq ($(RTCONFIG_EMF),y)
export CFLAGS += -D__CONFIG_EMF__
export CONFIG_EMF_ENABLED := $(RTCONFIG_EMF)
endif

ifeq ($(RTCONFIG_HSPOT),y)
export CONFIG_HSPOT=y
#export CFLAGS += -D__CONFIG_HSPOT__
export CFLAGS += -DNAS_GTK_PER_STA
ifeq ($(or $(RTCONFIG_BCM7),$(RTCONFIG_BCM_7114)),y)
export CFLAGS += -DHSPOT_OSEN
endif
ifeq ($(or $(RTCONFIG_BCM7114),$(RTCONFIG_HND_ROUTER)),y)
export ICONPATH := /www/hspot
export CFLAGS += -DICONPATH=\"$(ICONPATH)\"
endif
endif

ifeq ($(RTCONFIG_VISUALIZATION),y)
export CFLAGS += -D__CONFIG_VISUALIZATION__ -DCONFIG_VISUALIZATION_ENABLED
endif

ifeq ($(RTCONFIG_WPS),y)
ifeq ($(CONFIG_BCMWL5),y)
endif
export CFLAGS += -D__CONFIG_WPS__
# WFA WPS 2.0 Testbed extra caps
#export CFLAGS += -DWFA_WPS_20_TESTBED
endif

ifeq ($(RTCONFIG_NFC),y)
# WPS_NFC
export CFLAGS += -D__CONFIG_NFC__
endif

ifeq ($(RTCONFIG_NORTON),y)
export CFLAGS += -D__CONFIG_NORTON__
endif

ifeq ($(RTCONFIG_JFFS_NVRAM),y)
export CFLAGS += -DJFFS_NVRAM
endif

ifeq ($(RTCONFIG_DEBUG),y)
export CFLAGS += -g
endif

ifeq ($(strip $(BRCM_USER_SSP)),y)
export CFLAGS += $(SSP_MIN_COMPILER_OPTS)
export EXTRA_LDFLAGS += $(SSP_LIBS)
endif

ifneq (,$(filter y,$(RTCONFIG_SFP) $(RTCONFIG_4M_SFP)))
export MUVER := _1.4
endif

export NONOPT_CFLAGS := $(shell echo $(CFLAGS)| sed 's/-O2//g')
ifeq ($(HND_ROUTER),y)
export CFLAGS := $(shell echo $(CFLAGS)| sed 's/-fno-common//g')
endif

ifeq ($(RTCONFIG_NOTIFICATION_CENTER),y)
KERNEL_PTHREAD_ONE = $(if $(or $(findstring linux-4.1,$(LINUXDIR)),$(findstring linux-3.10,$(LINUXDIR)),$(RTCONFIG_QCA)),y)
export CONFIG_KERNEL_PTHREAD_ONE = $(KERNEL_PTHREAD_ONE)
endif

ifeq ($(RTCONFIG_QCA),y)
ifneq ($(PLATFORM_ROUTER),)
HOSTAPD_PKG:=$(PLATFORM_ROUTER)/qca-hostap
else ifeq ($(RTCONFIG_SOC_IPQ40XX),y)
HOSTAPD_PKG:=qca-hostap.ipq40xx
endif
else
HOSTAPD_PKG:=hostapd-2.6
endif

#
#
#
SEP=printf "\033[41;1m   $@   \033[0m\n"


TOPTARGETS := all clean
BIN_DIR := bin
SUFFIX_CLEAN := -clean
SUFFIX_INSTALL := -install
SUB_DIR := $(filter-out $(BIN_DIR)/., $(wildcard */.))
MODULES := $(patsubst %/.,%, $(SUB_DIR))
MODULES := $(foreach n, $(MODULES), $(shell find $(n)/ -iname "Makefile"))
MODULES := $(patsubst %/Makefile,%, $(MODULES))
MODULES_CLEAN := $(addsuffix $(SUFFIX_CLEAN), $(MODULES))
MODULES_INSTALL := $(addsuffix $(SUFFIX_INSTALL), $(MODULES))
BIN_FILES := $(foreach n, $(MODULES), $(shell find -executable -type f -path "./$(n)/*"))
INSTALL_FILES := $(shell find $(BIN_DIR)/ -executable -type f)

ifneq ($(filter $(MODULES), $(MAKECMDGOALS)),)

$(MODULES):
	$(MAKE) -C $@/.

else ifneq ($(filter $(MODULES_CLEAN), $(MAKECMDGOALS)),)
$(MODULES_CLEAN):
	rm -rf $(BIN_DIR)
	$(MAKE) -C $(patsubst %$(SUFFIX_CLEAN),%, $@)/. clean

else ifneq ($(filter $(MODULES_INSTALL), $(MAKECMDGOALS)),)
$(MODULES_INSTALL):
	mkdir -p $(BIN_DIR)
	cp -f script/tc-script $(BIN_DIR)/
	cp -f $(shell find -executable -type f -path "./$(patsubst %$(SUFFIX_INSTALL),%, $@)/*") $(BIN_DIR)/

else

$(TOPTARGETS): $(MODULES)
$(MODULES):
	$(MAKE) -C $@/. $(MAKECMDGOALS)

clean:
	rm -rf $(BIN_DIR)

install:
ifeq ($(strip $(BIN_FILES)),) 
else
	mkdir -p $(BIN_DIR)
	cp -f script/tc-script $(BIN_DIR)/
	cp -f $(BIN_FILES) $(BIN_DIR)/
endif

endif

target-install:
	for f in $(INSTALL_FILES); do \
		echo $$f "Installing..."; \
		install -D $$f $(INSTALLDIR)/sbin/unit_test/$$f; \
		$(STRIP) $(INSTALLDIR)/sbin/unit_test/$$f; \
		chmod 0500 $(INSTALLDIR)/sbin/unit_test/$$f; \
	done

.PHONY: $(TOPTARGETS) $(MODULES) $(MODULES_CLEAN) $(MODULES_INSTALL)
