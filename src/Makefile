CFLAGS = -Wall -Wpedantic -I/usr/local/opt/libusb/include/libusb-1.0 -L/usr/local/opt/libusb/lib/ -lusb-1.0

all:
	$(CC) $(CFLAGS) main.c -o main

info: info.c
	$(CC) $(CFLAGS) info.c -o info
