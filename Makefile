CFLAGS = -Wall -Wpedantic \
-I/usr/local/opt/libusb/include/libusb-1.0 \
-L/usr/local/opt/libusb/lib/ \
-lusb-1.0

ifdef DEBUG
	CFLAGS += -DDEBUG -Og
else
	CFLAGS += -O3
endif

FNAMES = keyboard fmt
HEADERS = $(addsuffix .h, $(addprefix src/, $(FNAMES)))
SOURCES = $(addsuffix .c, $(addprefix src/, $(FNAMES)))

PNAMES = packets_cli keyboard fmt
PHEADERS = $(addsuffix .h, $(addprefix src/, $(HFNAMES)))
PSOURCES = $(addsuffix .c, $(addprefix src/, $(PNAMES)))

all: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) src/main.c -o volcano
	$(CC) $(CFLAGS) $(SOURCES) src/daemon.c -o volcanod

packets: $(PHEADERS) $(PSOURCES)
	$(CC) $(CFLAGS) $(PSOURCES) -o packets

info: src/info.c
	$(CC) $(CFLAGS) src/info.c -o info

test: test/hotplug.c
	$(CC) $(CFLAGS) test/hotplug.c -o hotplug
