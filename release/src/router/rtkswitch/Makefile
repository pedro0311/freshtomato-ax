
include ../common.mak
include $(SRCBASE)/.config
include ../.config

#CFLAGS += $(if $(RTCONFIG_DEBUG),-g,-Os) -Wall $(EXTRACFLAGS) -fPIC
CFLAGS = -I. -I$(TOP)/shared -I$(SRCBASE)/include

CFLAGS += -DHND_ROUTER -DLINUX26 -DCONFIG_BCMWL5
CFLAGS += -I$(HND_SRC)/bcmdrivers/opensource/include/bcm963xx
CFLAGS += -I$(HND_SRC)/shared/opensource/include/bcm963xx
CFLAGS += -I$(HND_SRC)/userspace/private/include
CFLAGS += -I$(SRCBASE)/include -I$(SRCBASE)/../components/wlioctl/include -I$(SRCBASE)/../components/proto/include
CFLAGS += -I$(SRCBASE)/shared/bcmwifi/include -I $(SRCBASE)/../components/math/include/
CFLAGS += -D__EXPORTED_HEADERS__

CFLAGS += -I$(SRCBASE)/shared/bcmwifi/include
CFLAGS += -I$(SRCBASE)/common/include
CFLAGS += -I$(SRCBASE)/shared


OBJS = rtkswitch.o

EXEC = rtkswitch

ifeq ($(HND_ROUTER),y)
CFLAGS += -Werror=return-type -Werror=uninitialized -Wno-date-time 
endif
CFLAGS  += -I. -I$(TOP)/shared -I$(TOP)/kernel_header/include -I$(SRCBASE)/include -I$(TOP)/json-c
CFLAGS  += $(if $(RTCONFIG_DEBUG),,-s) -O2 -DNO_PARALLEL
CFLAGS  += -I$(TOP)/libasc
CFLAGS  += -I$(TOP)/curl/include


ifeq ($(RTCONFIG_BCMARM),y)
CFLAGS += -I$(SRCBASE)/shared/bcmwifi/include -DTYPEDEF_FLOAT_T
CFLAGS += -I$(SRCBASE)/common/include
CFLAGS += -I$(LINUXDIR)/drivers/char/rtk
#LDFLAGS += -lgcc_s
endif

#LDFLAGS += -L$(TOP)/curl/lib/.libs -lcurl -L$(TOP)/
#LDFLAGS += -L$(TOP)/libasc -lasc


ifeq ($(HND_ROUTER_AX), y)
CFLAGS += -I$(SRCBASE)/../components/proto/include -I$(SRCBASE)/../components/wlioctl/include -I$(SRCBASE)/wl -I$(SRCBASE)/wl/sys
endif

#LDFLAGS += -lm -L$(TOP)/shared -lshared -L$(TOP_PLATFORM)/nvram${BCMEX} -lnvram -L$(TOP)/json-c/.libs -ljson-c
#LDFLAGS += -L$(TOP)/shared -lshared -L$(TOP_PLATFORM)/nvram${BCMEX}${EX7} -lnvram 
#LDFLAGS += -L$(TOP)/openssl -lcrypto -lssl


include $(SRCBASE)/.config

%.o: %.c
	$(CC) $(CFLAGS) -c $<

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^  $(CFLAGS)

vpath %.c

install:
	install -D $(EXEC) $(INSTALLDIR)/usr/sbin/$(EXEC)
	#$(if $(RTCONFIG_DEBUG),,$(STRIP) $(INSTALLDIR)/usr/sbin/$(EXEC))
	#install -d $(INSTALLDIR)/usr/rtkswitch

clean:
	rm -rf *.o $(EXEC) *~
