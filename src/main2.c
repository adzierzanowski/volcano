#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "keyboard.h"

#define VERSION "0.0.1"

void usage() {
  puts(
    "volcano [OPTIONS...]\n"
    "\n"
    "Modecom Volcano Lanparty RGB userland driver\n"
    "\n"
    "OPTIONS:\n"
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
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  bool set_color = false;
  bool rainbow = false;
  bool rainbow_passed = false;

  while ((opt = getopt(argc, argv, "cC:hk:r:R:g:b:")) != -1) {
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
    const struct key_t *key = kbd_get_key(keyname);

    if (key) {
      kbd_send_start(kbdh);
      kbd_set_key_color(kbdh, key->val, color);
      kbd_send_end(kbdh);
    } else {
      printerr("Key %s not found\n", keyname);
    }
  } else if (rainbow_passed) {
    kbd_send_start(kbdh);
    kbd_set_rainbow(kbdh, rainbow);
    kbd_send_end(kbdh);
  }

  kbd_close(ctx, kbdh);
  return 0;
}
