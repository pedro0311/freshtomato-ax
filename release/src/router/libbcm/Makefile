include $(TOP)/.config


CFLAGS	+= -I. -I$(SRCBASE)/include ${WLAN_StdIncPathA} -I$(SRCBASE)/common/include -I$(SRCBASE)/router/shared -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s
ifeq ($(CONFIG_RTR_OPTIMIZE_SIZE),y)
CFLAGS  += -Os
else
CFLAGS  += -O2
endif
LDFLAGS += -L.

OBJS := bcmgpio.o

ifeq ($(wildcard ./*.c),)
all:
	cp -f ./prebuilt/libbcm.so .
else
all: libbcm.so
endif

install: all
	install -d $(INSTALLDIR)/usr/lib
	install -m 755 libbcm.so $(INSTALLDIR)/usr/lib
	$(STRIP) $(INSTALLDIR)/usr/lib/libbcm.so

clean:
	rm -f *.o *.so

libbcm.so: $(OBJS)
	$(LD) -shared -o $@ $^
