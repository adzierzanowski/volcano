CFLAGS = -Wall -Wpedantic \
-I/usr/local/opt/libusb/include/libusb-1.0 \
-L/usr/local/opt/libusb/lib/ \
-lusb-1.0

ifdef DEBUG
	CFLAGS += -DDEBUG -Og
else
	CFLAGS += -O3
endif

FNAMES = main keyboard fmt
HFNAMES = keyboard fmt
HEADERS = $(addsuffix .h, $(addprefix src/, $(HFNAMES)))
SOURCES = $(addsuffix .c, $(addprefix src/, $(FNAMES)))

PNAMES = packets_cli keyboard fmt
PHEADERS = $(addsuffix .h, $(addprefix src/, $(HFNAMES)))
PSOURCES = $(addsuffix .c, $(addprefix src/, $(PNAMES)))

all: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o volcano

packets: $(PHEADERS) $(PSOURCES)
	$(CC) $(CFLAGS) $(PSOURCES) -o packets

info: src/info.c
	$(CC) $(CFLAGS) src/info.c -o info
