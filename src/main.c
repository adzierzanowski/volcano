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
  uint8_t brightness = 0;
  bool brightness_passed = false;
  uint8_t direction = 0;
  bool direction_passed = false;
  uint8_t speed = 0;
  bool speed_passed = false;
  uint8_t reprate = 0;
  bool reprate_passed = false;


  while ((opt = getopt(argc, argv, "b:B:cC:D:g:hH:k:m:Mr:R:S:")) != -1) {
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
    if (strcmp(mode, "list") == 0) {
      kbd_print_modes();

    } else {
      kbd_set_mode(kbdh, kbd_get_mode(mode));
    }

  } else if (rainbow_passed) {
    kbd_set_rainbow(kbdh, rainbow);
  } else if (remap) {

    struct kbd_keymap_t kmap = {
      .a = KBD_KMAP_A,
      .a_m = KBD_KMAP_MODE_NORMAL,
      .b = KBD_KMAP_B,
      .b_m = KBD_KMAP_MODE_NORMAL,
      .c = KBD_KMAP_C,
      .c_m = KBD_KMAP_MODE_NORMAL,
      .d = KBD_KMAP_D,
      .d_m = KBD_KMAP_MODE_NORMAL,
      .e = KBD_KMAP_E,
      .e_m = KBD_KMAP_MODE_NORMAL,
      .f = KBD_KMAP_F,
      .f_m = KBD_KMAP_MODE_NORMAL,
      .g = KBD_KMAP_G,
      .g_m = KBD_KMAP_MODE_NORMAL,
      .h = KBD_KMAP_H,
      .h_m = KBD_KMAP_MODE_NORMAL,
      .i = KBD_KMAP_I,
      .i_m = KBD_KMAP_MODE_NORMAL,
      .j = KBD_KMAP_J,
      .j_m = KBD_KMAP_MODE_NORMAL,
      .k = KBD_KMAP_K,
      .k_m = KBD_KMAP_MODE_NORMAL,
      .l = KBD_KMAP_L,
      .l_m = KBD_KMAP_MODE_NORMAL,
      .m = KBD_KMAP_M,
      .m_m = KBD_KMAP_MODE_NORMAL,
      .n = KBD_KMAP_N,
      .n_m = KBD_KMAP_MODE_NORMAL,
      .o = KBD_KMAP_O,
      .o_m = KBD_KMAP_MODE_NORMAL,
      .p = KBD_KMAP_P,
      .p_m = KBD_KMAP_MODE_NORMAL,
      .q = KBD_KMAP_Q,
      .q_m = KBD_KMAP_MODE_NORMAL,
      .r = KBD_KMAP_R,
      .r_m = KBD_KMAP_MODE_NORMAL,
      .s = KBD_KMAP_S,
      .s_m = KBD_KMAP_MODE_NORMAL,
      .t = KBD_KMAP_T,
      .t_m = KBD_KMAP_MODE_NORMAL,
      .u = KBD_KMAP_U,
      .u_m = KBD_KMAP_MODE_NORMAL,
      .v = KBD_KMAP_V,
      .v_m = KBD_KMAP_MODE_NORMAL,
      .w = KBD_KMAP_W,
      .w_m = KBD_KMAP_MODE_NORMAL,
      .x = KBD_KMAP_X,
      .x_m = KBD_KMAP_MODE_NORMAL,
      .y = KBD_KMAP_Y,
      .y_m = KBD_KMAP_MODE_NORMAL,
      .z = KBD_KMAP_Z,
      .z_m = KBD_KMAP_MODE_NORMAL,
      .k0 = KBD_KMAP_0,
      .k0_m = KBD_KMAP_MODE_NORMAL,
      .k1 = KBD_KMAP_1,
      .k1_m = KBD_KMAP_MODE_NORMAL,
      .k2 = KBD_KMAP_2,
      .k2_m = KBD_KMAP_MODE_NORMAL,
      .k3 = KBD_KMAP_3,
      .k3_m = KBD_KMAP_MODE_NORMAL,
      .k4 = KBD_KMAP_4,
      .k4_m = KBD_KMAP_MODE_NORMAL,
      .k5 = KBD_KMAP_5,
      .k5_m = KBD_KMAP_MODE_NORMAL,
      .k6 = KBD_KMAP_6,
      .k6_m = KBD_KMAP_MODE_NORMAL,
      .k7 = KBD_KMAP_7,
      .k7_m = KBD_KMAP_MODE_NORMAL,
      .k8 = KBD_KMAP_8,
      .k8_m = KBD_KMAP_MODE_NORMAL,
      .k9 = KBD_KMAP_9,
      .k9_m = KBD_KMAP_MODE_NORMAL,
      .f1 = KBD_KMAP_F1,
      .f1_m = KBD_KMAP_MODE_NORMAL,
      .f2 = KBD_KMAP_F2,
      .f2_m = KBD_KMAP_MODE_NORMAL,
      .f3 = KBD_KMAP_F3,
      .f3_m = KBD_KMAP_MODE_NORMAL,
      .f4 = KBD_KMAP_F4,
      .f4_m = KBD_KMAP_MODE_NORMAL,
      .f5 = KBD_KMAP_F5,
      .f5_m = KBD_KMAP_MODE_NORMAL,
      .f6 = KBD_KMAP_F6,
      .f6_m = KBD_KMAP_MODE_NORMAL,
      .f7 = KBD_KMAP_F7,
      .f7_m = KBD_KMAP_MODE_NORMAL,
      .f8 = KBD_KMAP_F8,
      .f8_m = KBD_KMAP_MODE_NORMAL,
      .f9 = KBD_KMAP_F9,
      .f9_m = KBD_KMAP_MODE_NORMAL,
      .f10 = KBD_KMAP_F10,
      .f10_m = KBD_KMAP_MODE_NORMAL,
      .f11 = KBD_KMAP_F11,
      .f11_m = KBD_KMAP_MODE_NORMAL,
      .f12 = KBD_KMAP_F12,
      .f12_m = KBD_KMAP_MODE_NORMAL,
      .enter = KBD_KMAP_ENTER,
      .enter_m = KBD_KMAP_MODE_NORMAL,
      .esc = KBD_KMAP_ESC,
      .esc_m = KBD_KMAP_MODE_NORMAL,
      .backspace = KBD_KMAP_BACKSPACE,
      .backspace_m = KBD_KMAP_MODE_NORMAL,
      .tab = KBD_KMAP_TAB,
      .tab_m = KBD_KMAP_MODE_NORMAL,
      .space = KBD_KMAP_SPACE,
      .space_m = KBD_KMAP_MODE_NORMAL,

      .minus = KBD_KMAP_MINUS,
      .minus_m = KBD_KMAP_MODE_NORMAL,
      .plus= KBD_KMAP_PLUS,
      .plus_m = KBD_KMAP_MODE_NORMAL,
      .lbracket = KBD_KMAP_LBRACKET,
      .lbracket_m = KBD_KMAP_MODE_NORMAL,
      .rbracket = KBD_KMAP_RBRACKET,
      .rbracket_m = KBD_KMAP_MODE_NORMAL,
      .backslash = KBD_KMAP_BACKSLASH,
      .backslash_m = KBD_KMAP_MODE_NORMAL,
      .colon = KBD_KMAP_COLON,
      .colon_m = KBD_KMAP_MODE_NORMAL,
      .ap = KBD_KMAP_APOSTROPHE,
      .ap_m = KBD_KMAP_MODE_NORMAL,
      .tilde = KBD_KMAP_TILDE,
      .tilde_m = KBD_KMAP_MODE_NORMAL,
      .comma = KBD_KMAP_COMMA,
      .comma_m = KBD_KMAP_MODE_NORMAL,
      .dot = KBD_KMAP_DOT,
      .dot_m = KBD_KMAP_MODE_NORMAL,
      .slash = KBD_KMAP_SLASH,
      .slash_m = KBD_KMAP_MODE_NORMAL,
      .caps = KBD_KMAP_CAPS,
      .caps_m = KBD_KMAP_MODE_NORMAL,

      .ps = KBD_KMAP_PS,
      .ps_m = KBD_KMAP_MODE_NORMAL,
      .sl = KBD_KMAP_SL,
      .sl_m = KBD_KMAP_MODE_NORMAL,
      .pb = KBD_KMAP_PB,
      .pb_m = KBD_KMAP_MODE_NORMAL,
      .ins = KBD_KMAP_INS,
      .ins_m = KBD_KMAP_MODE_NORMAL,
      .hm = KBD_KMAP_HM,
      .hm_m = KBD_KMAP_MODE_NORMAL,
      .pu = KBD_KMAP_PU,
      .pu_m = KBD_KMAP_MODE_NORMAL,
      .del = KBD_KMAP_DEL,
      .del_m = KBD_KMAP_MODE_NORMAL,
      .end = KBD_KMAP_END,
      .end_m = KBD_KMAP_MODE_NORMAL,
      .pd = KBD_KMAP_PD,
      .pd_m = KBD_KMAP_MODE_NORMAL,
      .right = KBD_KMAP_RIGHT,
      .right_m = KBD_KMAP_MODE_NORMAL,
      .left = KBD_KMAP_LEFT,
      .left_m = KBD_KMAP_MODE_NORMAL,
      .down = KBD_KMAP_DOWN,
      .down_m = KBD_KMAP_MODE_NORMAL,
      .up = KBD_KMAP_UP,
      .up_m = KBD_KMAP_MODE_NORMAL,

      .lshift = KBD_KMAP_LSHIFT,
      .lshift_m = KBD_KMAP_MODE_MODAL,
      .rshift = KBD_KMAP_RSHIFT,
      .rshift_m = KBD_KMAP_MODE_MODAL,
      .lctrl = KBD_KMAP_LCTRL,
      .lctrl_m = KBD_KMAP_MODE_MODAL,
      .rctrl = KBD_KMAP_RCTRL,
      .rctrl_m = KBD_KMAP_MODE_MODAL,
      .lalt = KBD_KMAP_LALT,
      .lalt_m = KBD_KMAP_MODE_MODAL,
      .ralt = KBD_KMAP_META,
      .ralt_m = KBD_KMAP_MODE_MODAL,
      .meta = KBD_KMAP_META,
      .meta_m = KBD_KMAP_MODE_MODAL,
      .menu = KBD_KMAP_RALT,
      .menu_m = KBD_KMAP_MODE_MODAL,
    };
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
