CFLAGS = -Wall \
-Wpedantic \
-I/usr/local/opt/libusb/include/libusb-1.0 \
-L/usr/local/opt/libusb/lib/ \
-lusb-1.0 \
-DDEBUG

all: src/main2.c src/keyboard.c src/fmt.c
	$(CC) $(CFLAGS) src/main2.c src/keyboard.c src/fmt.c -o volcano

info: info.c
	$(CC) $(CFLAGS) info.c -o info
