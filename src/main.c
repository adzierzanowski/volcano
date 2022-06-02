#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "keyboard.h"

#define VERSION "0.0.3"

void usage() {
  puts(
    "volcano [OPTIONS...]\n"
    "\n"
    "Modecom Volcano Lanparty RGB userland driver\n"
    "\n"
    "OPTIONS:\n"
    "    -m         select mode ('list' to list all modes)\n"
    "    -c         set color for entire keyboard\n"
    "    -C         pass a command\n"
    "    -h         show this help message and exit\n"
    "    -k         select a key for the color change\n"
    "    -r         red value\n"
    "    -R (0|1)   rainbow\n"
    "    -g         green value\n"
    "    -b         blue value\n"
    "\n"
    "EXAMPLES:\n"
    "    volcano -k BACKSPACE -r 255 -g 0 -b 0      # set backspace to red\n"
    "    volcano -c -g 255              # set the entire keyboard to green\n"
    "    volcano -C clear                      # clear the entire keyboard\n"
    "    volcano -C clear-m1    # clear the entire keyboard in custom mode\n"
    "\n"
    "CODE REPOSITORY:"
  );

  printf("    volcano v. %s\n", VERSION);

  puts(
    "    Copyright (C) 2022 A. Dzierzanowski\n"
    "    This software is licensed under MIT license.\n"
    "    https://github.com/adzierzanowski/volcano\n"
  );
}


int main(int argc, char *argv[]) {
  char opt;
  const char *keyname = NULL;
  const char *command = NULL;
  const char *mode = NULL;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  bool set_color = false;
  bool rainbow = false;
  bool rainbow_passed = false;
  bool remap = false;

  while ((opt = getopt(argc, argv, "b:cC:g:hk:m:Mr:R:")) != -1) {
    switch (opt) {
      case 'c':
        set_color = true;
        break;

      case 'C':
        command = optarg;
        break;

      case 'h':
        usage();
        return 0;

      case 'k':
        keyname = optarg;
        break;

      case 'm':
        mode = optarg;
        break;

      case 'M':
        remap = true;
        break;

      case 'r':
        r = atoi(optarg);
        break;

      case 'R':
        rainbow = (bool) atoi(optarg);
        rainbow_passed = true;
        break;

      case 'g':
        g = atoi(optarg);
        break;

      case 'b':
        b = atoi(optarg);
        break;

      default:
        break;
    }
  }

  //bool status;
  libusb_context *ctx = NULL;
  libusb_device_handle *kbdh = NULL;

  if (!kbd_open(&ctx, &kbdh)) {
    exit(1);
  }

#ifdef DEBUG
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
#endif


  const uint32_t color = (r << 16) | (g << 8) | b;

  if (command) {
    if (strcmp(command, "clear-m1") == 0) {
      kbd_send_start(kbdh);
      for (int i = 0; i < keys_sz; i++) {
        kbd_set_key_color(kbdh, keys[i].val, 0);
      }
      kbd_send_end(kbdh);

    } else if (strcmp(command, "clear") == 0) {
      kbd_send_start(kbdh);
      kbd_set_color(kbdh, 0, 0, 0);
      kbd_send_end(kbdh);
    } else {
      printerr("Command not recognized\n");
    }

  } else if (set_color) {
    kbd_send_start(kbdh);
    kbd_set_color(kbdh, r, g, b);
    kbd_send_end(kbdh);

  } else if (keyname) {
    const struct kbd_key_t *key = kbd_get_key(keyname);

    if (key) {
      kbd_send_start(kbdh);
      kbd_set_key_color(kbdh, key->val, color);
      kbd_send_end(kbdh);
    } else {
      printerr("Key %s not found\n", keyname);
    }
  } else if (mode) {
    if (strcmp(mode, "list") == 0) {
      kbd_print_modes();

    } else {
      kbd_send_start(kbdh);
      kbd_set_mode(kbdh, kbd_get_mode(mode));
      kbd_send_end(kbdh);
    }

  } else if (rainbow_passed) {
    kbd_send_start(kbdh);
    kbd_set_rainbow(kbdh, rainbow);
    kbd_send_end(kbdh);
  } else if (remap) {

    struct kbd_keymap_t kmap = {
      .a = KBD_KMAP_A,
      .b = KBD_KMAP_B,
      .c = KBD_KMAP_C,
      .d = KBD_KMAP_D,
      .e = KBD_KMAP_E,
      .f = KBD_KMAP_F,
      .g = KBD_KMAP_G,
      .h = KBD_KMAP_H,
      .i = KBD_KMAP_I,
      .j = KBD_KMAP_J,
      .k = KBD_KMAP_K,
      .l = KBD_KMAP_L,
      .m = KBD_KMAP_M,
      .n = KBD_KMAP_N,
      .o = KBD_KMAP_O,
      .p = KBD_KMAP_P,
      .q = KBD_KMAP_Q,
      .r = KBD_KMAP_R,
      .s = KBD_KMAP_S,
      .t = KBD_KMAP_T,
      .u = KBD_KMAP_U,
      .v = KBD_KMAP_V,
      .w = KBD_KMAP_W,
      .x = KBD_KMAP_X,
      .y = KBD_KMAP_Y,
      .z = KBD_KMAP_Z,
      .k0 = KBD_KMAP_0,
      .k1 = KBD_KMAP_1,
      .k2 = KBD_KMAP_2,
      .k3 = KBD_KMAP_3,
      .k4 = KBD_KMAP_4,
      .k5 = KBD_KMAP_5,
      .k6 = KBD_KMAP_6,
      .k7 = KBD_KMAP_7,
      .k8 = KBD_KMAP_8,
      .k9 = KBD_KMAP_9,
      .f1 = KBD_KMAP_F1,
      .f2 = KBD_KMAP_F2,
      .f3 = KBD_KMAP_F3,
      .f4 = KBD_KMAP_F4,
      .f5 = KBD_KMAP_F5,
      .f6 = KBD_KMAP_F6,
      .f7 = KBD_KMAP_F7,
      .f8 = KBD_KMAP_F8,
      .f9 = KBD_KMAP_F9,
      .f10 = KBD_KMAP_F10,
      .f11 = KBD_KMAP_F11,
      .f12 = KBD_KMAP_F12,
      .enter = KBD_KMAP_ENTER,
      .esc = KBD_KMAP_ESC,
      .backspace = KBD_KMAP_BACKSPACE,
      .tab = KBD_KMAP_TAB,
      .space = KBD_KMAP_SPACE,

      .minus = KBD_KMAP_MINUS,
      .plus= KBD_KMAP_PLUS,
      .lbracket = KBD_KMAP_LBRACKET,
      .rbracket = KBD_KMAP_RBRACKET,
      .backslash = KBD_KMAP_BACKSLASH,
      .colon = KBD_KMAP_COLON,
      .ap = KBD_KMAP_APOSTROPHE,
      .tilde = KBD_KMAP_TILDE,
      .comma = KBD_KMAP_COMMA,
      .dot = KBD_KMAP_DOT,
      .slash = KBD_KMAP_SLASH,
      .caps = KBD_KMAP_CAPS,

      .ps = KBD_KMAP_PS,
      .sl = KBD_KMAP_SL,
      .pb = KBD_KMAP_PB,
      .ins = KBD_KMAP_INS,
      .hm = KBD_KMAP_HM,
      .pu = KBD_KMAP_PU,
      .del = KBD_KMAP_DEL,
      .end = KBD_KMAP_END,
      .pd = KBD_KMAP_PD,
      .right = KBD_KMAP_RIGHT,
      .left = KBD_KMAP_LEFT,
      .down = KBD_KMAP_DOWN,
      .up = KBD_KMAP_UP,

      .lshift = KBD_KMAP_LSHIFT,
      .rshift = KBD_KMAP_RSHIFT,
      .lctrl = KBD_KMAP_LCTRL,
      .rctrl = KBD_KMAP_RCTRL,
      .lalt = KBD_KMAP_LALT,
      .ralt = KBD_KMAP_RALT,
      .meta = KBD_KMAP_META,
      .menu = KBD_KMAP_MENU

    };
    kbd_send_start(kbdh);
    kbd_remap(kbdh, &kmap);
    kbd_send_end(kbdh);
  }

  kbd_close(ctx, kbdh);
  return 0;
}
