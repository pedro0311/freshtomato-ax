ifdef BCM_KF # defined(CONFIG_BCM_KF_ARM_BCM963XX)
# SHARED := ../../../../../../../src/shared

ifdef CONFIG_BCM96878  
obj-y += bcm6878.o
else
ifeq ($(strip $(CONFIG_BCM96846)),)
AFLAGS_plat-head.o        := -DTEXT_OFFSET=$(TEXT_OFFSET)

obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o
obj-$(CONFIG_PLAT_CA9_MPCORE) += ca9mp_core.o
obj-$(CONFIG_PLAT_CA9_MPCORE_TIMER) += ca9mp_timer.o
obj-$(CONFIG_PLAT_B15_CORE) += b15_core.o
obj-$(CONFIG_CACHE_L310) += cache-l310.o
obj-$(CONFIG_PLAT_BCM63138) += bcm63138.o
obj-$(CONFIG_PLAT_BCM63148) += bcm63148.o
obj-$(CONFIG_PLAT_BCM63XX_EXT_TIMER) += bcm63xx_timer.o
obj-$(CONFIG_PLAT_BCM63XX_ACP) += bcm63xx_acp.o

else
obj-y += bcm6846.o
obj-$(CONFIG_HOTPLUG_CPU) += hotplug.o
endif
endif

EXTRA_CFLAGS    += -I$(INC_BRCMBOARDPARMS_PATH)/$(BRCM_BOARD) -I$(SRCBASE)/include -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD) -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)/pmc -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)

ifeq ($(strip $(CONFIG_BCM96878)),)
obj-$(CONFIG_SMP) += platsmp.o plat-ca9mp-headsmp.o
endif

endif # BCM_KF # defined(CONFIG_BCM_KF_ARM_BCM963XX)
