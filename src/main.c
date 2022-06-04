#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "keyboard.h"

#define VERSION "0.1.1"

void usage() {
  puts(
    "volcano [OPTIONS...]\n"
    "\n"
    "Modecom Volcano Lanparty RGB userland driver\n"
    "\n"
    "OPTIONS:\n"
    "    -B (0-4)       set brightness (0-4)\n"
    "    -D (0-1)       set direction (0-1)\n"
    "    -S (0-4)       set speed\n"
    "    -R (0-1)       set rainbow\n"
    "    -H (0-3)       set report rate (0-3; 125 Hz - 1000 Hz)\n"
    "\n"
    "    -m MODE        select color mode ('list' to list all modes)\n"
    "    -M FILE        remap keyboard keys\n"
    "    -c             set color for entire keyboard\n"
    "    -C CMD         pass a command\n"
    "    -h             show this help message and exit\n"
    "    -k KEY         select a key for the color change\n"
    "    -r (0-255)     red value\n"
    "    -g (0-255)     green value\n"
    "    -b (0-255)     blue value\n"
    "\n"
    "EXAMPLES:\n"
    "    volcano -k BACKSPACE -r 255 -g 0 -b 0      # set backspace to red\n"
    "    volcano -c -g 255              # set the entire keyboard to green\n"
    "    volcano -C clear                      # clear the entire keyboard\n"
    "    volcano -C clear-m1    # clear the entire keyboard in custom mode\n"
    "    volcano -M kmap.dat      # load a keymap from the file keymap.dat\n"
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
  const char *remap_fname = NULL;
  uint8_t brightness = 0;
  bool brightness_passed = false;
  uint8_t direction = 0;
  bool direction_passed = false;
  uint8_t speed = 0;
  bool speed_passed = false;
  uint8_t reprate = 0;
  bool reprate_passed = false;


  while ((opt = getopt(argc, argv, "b:B:cC:D:g:hH:k:m:M:r:R:S:")) != -1) {
    switch (opt) {
      case 'B':
        brightness_passed = true;
        brightness = atoi(optarg);
        break;

      case 'D':
        direction_passed = true;
        direction = atoi(optarg);
        break;

      case 'S':
        speed_passed = true;
        speed = atoi(optarg);
        break;

      case 'H':
        reprate_passed = true;
        reprate = atoi(optarg);
        break;

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
        remap_fname = optarg;
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

  libusb_context *ctx = NULL;
  libusb_device_handle *kbdh = NULL;

  if (mode && strcmp(mode, "list") == 0) {
    kbd_print_modes();
    return 0;
  }

  if (!kbd_open(&ctx, &kbdh)) {
    return 1;
  }

#ifdef DEBUG
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
#endif


  const uint32_t color = (r << 16) | (g << 8) | b;

  if (command) {
    if (strcmp(command, "clear-m1") == 0) {
      for (int i = 0; i < keys_sz; i++) {
        kbd_set_key_color(kbdh, keys[i].val, 0);
      }
    } else if (strcmp(command, "clear") == 0) {
      kbd_set_color(kbdh, 0, 0, 0);
    } else {
      printerr("Command not recognized\n");
    }

  } else if (set_color) {
    kbd_set_color(kbdh, r, g, b);

  } else if (keyname) {
    const struct kbd_key_t *key = kbd_get_key(keyname);

    if (key) {
      kbd_set_key_color(kbdh, key->val, color);
    } else {
      printerr("Key %s not found\n", keyname);
    }
  } else if (mode) {
    kbd_set_mode(kbdh, kbd_get_mode(mode));

  } else if (rainbow_passed) {
    kbd_set_rainbow(kbdh, rainbow);

  } else if (remap) {
    struct kbd_keymap_t kmap;
    FILE *f = fopen(remap_fname, "rb");
    fread(&kmap, sizeof (struct kbd_keymap_t), 1, f);
    fclose(f);

    kbd_remap(kbdh, &kmap);

  } else if (brightness_passed) {
    kbd_set_brightness(kbdh, brightness);

  } else if (direction_passed) {
    kbd_set_direction(kbdh, direction);

  } else if (speed_passed) {
    kbd_set_speed(kbdh, speed);

  } else if (reprate_passed) {
    kbd_set_report_rate(kbdh, reprate);
  }

  kbd_close(ctx, kbdh);
  return 0;
}
