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


  /*

  uint32_t keys[] = { KEY_ESC, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
    KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_PS, KEY_SL, KEY_PB,
    KEY_TILDE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
    KEY_0, KEY_MINUS, KEY_PLUS, KEY_BACKSPACE, KEY_INS, KEY_HM, KEY_PU, KEY_TAB,
    KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P,
    KEY_LBRACKET, KEY_RBRACKET, KEY_BACKSLASH, KEY_END, KEY_PD, KEY_CAPS,
    KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_COLON,
    KEY_APOSTROPHE, KEY_ENTER, KEY_LSHIFT, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B,
    KEY_N, KEY_M, KEY_LT, KEY_GT, KEY_QMARK, KEY_RSHIFT, KEY_UP, KEY_LCTRL,
    KEY_WIN, KEY_LALT, KEY_SPACE, KEY_RALT, KEY_FN, KEY_MENU, KEY_RCTRL,
    KEY_LEFT, KEY_DOWN, KEY_RIGHT
  };

  size_t keys_sz = sizeof keys / sizeof keys[0];

  for (int i = 0; i < keys_sz; i++) {
    kbd_set_key_color(kbdh, keys[i], (i *834) % 0x1000000);
  }
  */

  free(buf);
  kbd_close(ctx, kbdh);
  return 0;
}
