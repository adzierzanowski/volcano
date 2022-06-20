UNAME = $(shell uname)
CFLAGS = -Wall -Wpedantic -lusb-1.0
ifeq ($(UNAME), Darwin)
CFLAGS += -Wno-unused-command-line-argument \
-I/usr/local/opt/libusb/include/libusb-1.0 \
-L/usr/local/opt/libusb/lib/
else
CFLAGS += -I/usr/include/libusb-1.0 -Wno-unused-result
endif

ifdef DEBUG
	CFLAGS += -DDEBUG -Og
else
	CFLAGS += -O3
endif

BUILD = build
BIN = bin

.PHONY: all
all: volcanod volcano volcanosrv volcano-info

volcanod: $(addprefix $(BUILD)/, fmt.o daemon.o keyboard.o)
	@-mkdir -p $(BIN)
	$(CC) $^ $(CFLAGS) -o $(BIN)/$@

volcanosrv: $(addprefix $(BUILD)/, fmt.o srv.o keyboard.o)
	@-mkdir -p $(BIN)
	$(CC) $^ $(CFLAGS) -o $(BIN)/$@

volcano: $(addprefix $(BUILD)/, fmt.o main.o keyboard.o)
	@-mkdir -p $(BIN)
	$(CC) $^ $(CFLAGS) -o $(BIN)/$@

volcano-info: $(addprefix $(BUILD)/, info.o)
	@-mkdir -p $(BIN)
	$(CC) $^ $(CFLAGS) -o $(BIN)/$@

$(BUILD):
	@-mkdir -p $@

$(BIN):
	@-mkdir -p $@

clean:
	-rm -rf $(BUILD)
	-rm -rf $(BIN)

$(BUILD)/%.o: src/%.c $(BUILD)
	$(CC) -c $(CFLAGS) $< -o $@
