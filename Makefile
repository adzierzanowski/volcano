UNAME = $(shell uname)
CFLAGS = -Wall -Wpedantic -lusb-1.0
ifeq ($(UNAME), Darwin)
CFLAGS += -Wno-unused-command-line-argument \
-I/usr/local/opt/libusb/include/libusb-1.0 \
-L/usr/local/opt/libusb/lib/
else
CFLAGS += -I/usr/include/libusb-1.0 -Wno-unused-result -Wno-format-overflow
endif

ifdef DEBUG
	CFLAGS += -DDEBUG -Og
else
	CFLAGS += -O3
endif

BUILD = build
BIN = bin
SRC = src

.PHONY: all
all: volcanod volcano volcanosrv volcanoctl

volcanod: $(addprefix $(BUILD)/, util.o config.o fmt.o daemon.o keyboard.o)
	@-mkdir -p $(BIN)
	$(CC) $^ $(CFLAGS) -o $(BIN)/$@

volcanosrv: $(addprefix $(BUILD)/, util.o config.o fmt.o srv.o keyboard.o)
	@-mkdir -p $(BIN)
	$(CC) $^ $(CFLAGS) -o $(BIN)/$@

volcano: $(addprefix $(BUILD)/, fmt.o main.o keyboard.o)
	@-mkdir -p $(BIN)
	$(CC) $^ $(CFLAGS) -o $(BIN)/$@

volcanoctl:
	@-mkdir -p $(BIN)
	cp tools/volcanoctl $(BIN)/volcanoctl

test: $(addprefix $(BUILD)/, fmt.o keyboard.o)
	$(CC) $^ test/main.c $(CFLAGS) -o testbin

$(BUILD):
	@-mkdir -p $@

$(BIN):
	@-mkdir -p $@

clean:
	-rm -rf $(BUILD)
	-rm -rf $(BIN)

$(BUILD)/%.o: $(SRC)/%.c $(BUILD)
	$(CC) -c $(CFLAGS) $< -o $@


$(SRC)/%.c: $(SRC)/%.h
