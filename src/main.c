#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "keyboard.h"


int main(int argc, char *argv[]) {
  size_t bufsz = 64 * (argc - 1);
  uint8_t *buf = calloc(bufsz, sizeof (uint8_t));

  if (argc < 2) {
    printerr("Need at least one filename\n");
    free(buf);
    exit(1);
  }

  for (int i = 1; i < argc; i++) {
    FILE *f = fopen(argv[i], "rb");
    if (f == NULL) {
      printerr("File not found: %s\n", argv[i]);
      free(buf);
      exit(1);
    }
    fread(buf+64*(i-1), 1, 64, f);
    fclose(f);
  }

  bool status;
  libusb_context *ctx = NULL;
  libusb_device_handle *kbdh = NULL;

  if (!kbd_open(&ctx, &kbdh)) {
    exit(1);
  }

#ifdef DEBUG
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
#endif

  for (int i = 1; i < argc; i++) {
    uint8_t msg[64] = {0};
    memcpy(msg, buf + (i-1) * 64, 64);
    status = kbd_send_and_recv(kbdh, msg);
    if (!status) {
      break;
    }
    char buf[64];
    kbd_recv(kbdh, 64, (uint8_t *) buf, 50);
  }

  kbd_close(ctx, kbdh);
  return 0;
}
