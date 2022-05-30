CFLAGS = -Wall \
-Wpedantic \
-I/usr/local/opt/libusb/include/libusb-1.0 \
-L/usr/local/opt/libusb/lib/ \
-lusb-1.0 \
-DDEBUG

all: src/main.c src/keyboard.c src/fmt.c
	$(CC) $(CFLAGS) src/main.c src/keyboard.c src/fmt.c -o main

info: info.c
	$(CC) $(CFLAGS) info.c -o info
