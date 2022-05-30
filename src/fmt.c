#include "fmt.h"

#define RED "\x1b[38;5;1m"
#define END "\x1b[0m"


void printerr(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("%s", RED);
  vfprintf(stderr, fmt, args);
  printf("%s", END);
  va_end(args);
}

void print_libusb_err(int status) {
  printerr("Error: %s\n", libusb_error_name(status));
}

void print64(uint8_t *buf) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 16; j++) {
      printf("%02x ", buf[i*16+j]);
    }
    printf("\n");
  }
}
