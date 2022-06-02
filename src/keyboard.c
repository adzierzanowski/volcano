#include "keyboard.h"

bool kbd_open(libusb_context **ctx, libusb_device_handle **kbdh) {
  *ctx = NULL;
  *kbdh = NULL;

  int status = 0;
  libusb_init(ctx);
  if (status != 0) {
    print_libusb_err(status);
    return false;
  }

  *kbdh = libusb_open_device_with_vid_pid(*ctx, KBD_VID, KBD_PID);
  if (*kbdh == NULL) {
    printerr("Error: Keyboard not found\n");
    return false;
  }

  status = libusb_detach_kernel_driver(*kbdh, CTL_INTERFACE);
  if (status != 0) {
    print_libusb_err(status);
    libusb_close(*kbdh);
    return false;
  }

  status = libusb_claim_interface(*kbdh, CTL_INTERFACE);
  if (status != 0) {
    print_libusb_err(status);
    libusb_attach_kernel_driver(*kbdh, CTL_INTERFACE);
    return false;
  }

  //status = libusb_set_configuration(kbdh, 1);

  return true;
}

bool kbd_close(libusb_context *ctx, libusb_device_handle *kbdh) {
  libusb_release_interface(kbdh, CTL_INTERFACE);
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_ERROR);
  libusb_attach_kernel_driver(kbdh, CTL_INTERFACE);
  libusb_close(kbdh);
  libusb_exit(ctx);
  return true;
}

bool kbd_send(libusb_device_handle *kbdh, uint8_t msg[64]) {
  int transferred = libusb_control_transfer(
    kbdh,
    CTL_REQUEST_TYPE,
    CTL_REQUEST,
    CTL_REQUEST_VALUE,
    CTL_INTERFACE,
    msg,
    64,
    1000);

  if (transferred != 64) {
    print_libusb_err(transferred);
    return false;
  }

  return true;
}

bool kbd_recv(
  libusb_device_handle *kbdh, size_t sz, uint8_t *buf, unsigned int timeout) {
  memset(buf, 0, sz);

  // TODO:
  int transferred = 0;
  int status = libusb_bulk_transfer(
    kbdh, CTL_ENDPOINT, (unsigned char *) buf, sz, &transferred, timeout);

  if (status != 0) {
    print_libusb_err(status);
    return false;
  }

  return true;
}

bool kbd_validate_response(uint8_t *sent, uint8_t *recv, size_t sz) {
  for (int i = 0; i < sz; i++) {
    if (i == 7) {
      if (recv[i] != 1) {
        printerr("Response invalid (NACK)");
        return false;
      }
    } else {
      if ((uint8_t) sent[i] != (uint8_t) recv[i]) {
        printerr("received data does not match\n");
        printerr("i=%d sent=%d received=%d\n", i, sent[i], recv[i]);
        return false;
      }
    }
  }
  return true;
}

bool kbd_send_and_recv(libusb_device_handle *kbdh, uint8_t msg[64]) {
  if (!kbd_send(kbdh, msg)) {
    return false;
  }

  uint8_t recv[64] = {0};

  if (!kbd_recv(kbdh, 64, (uint8_t *) recv, 1000)) {
    return false;
  }

  if (!kbd_validate_response(msg, recv, 64)) {
    return false;
  }

  return true;
}

bool kbd_va_send_and_recv(libusb_device_handle *kbdh, size_t len, ...) {
  uint8_t msg[64] = {0};

  va_list args;
  va_start(args, len);
  for (int i = 0; i < len; i++) {
    msg[i] = va_arg(args, int);
  }
  va_end(args);

  bool status = kbd_send_and_recv(kbdh, msg);

  return status;
}

void kbd_set_key_color(libusb_device_handle *kbdh, uint32_t key, uint32_t color) {
  uint8_t d1 = (key >> 16) & 0xff;
  uint8_t d5 = (key >> 8) & 0xff;
  uint8_t d6 = key & 0xff;
  uint8_t r = (color >> 16) & 0xff;
  uint8_t g = (color >> 8) & 0xff;
  uint8_t b = color & 0xff;

  kbd_va_send_and_recv(
    kbdh, 11, 0x04, d1, 0x00, 0x11, 0x03, d5, d6, 0x00, r, b, g );
}

bool kbd_send_start(libusb_device_handle *kbdh) {
  return kbd_va_send_and_recv(kbdh, 4, 0x04, 0x01, 0x00, 0x01);
}

bool kbd_send_end(libusb_device_handle *kbdh) {
  return kbd_va_send_and_recv(kbdh, 4, 0x04, 0x02, 0x00, 0x02);
}

const struct kbd_key_t keys[88] = {
  { .name = "PLUS", .val = KEY_PLUS },
  { .name = "DEL", .val = KEY_DEL },
  { .name = "A", .val = KEY_A },
  { .name = "E", .val = KEY_E },
  { .name = "INS", .val = KEY_INS },
  { .name = "SL", .val = KEY_SL },
  { .name = "F11", .val = KEY_F11 },
  { .name = "F8", .val = KEY_F8 },
  { .name = "8", .val = KEY_8 },
  { .name = "G", .val = KEY_G },
  { .name = "END", .val = KEY_END },
  { .name = "PD", .val = KEY_PD },
  { .name = "RBRACKET", .val = KEY_RBRACKET },
  { .name = "Y", .val = KEY_Y },
  { .name = "COLON", .val = KEY_COLON },
  { .name = "PB", .val = KEY_PB },
  { .name = "F9", .val = KEY_F9 },
  { .name = "CAPS", .val = KEY_CAPS },
  { .name = "F10", .val = KEY_F10 },
  { .name = "F", .val = KEY_F },
  { .name = "9", .val = KEY_9 },
  { .name = "LBRACKET", .val = KEY_LBRACKET },
  { .name = "D", .val = KEY_D },
  { .name = "F12", .val = KEY_F12 },
  { .name = "TAB", .val = KEY_TAB },
  { .name = "F7", .val = KEY_F7 },
  { .name = "APOSTROPHE", .val = KEY_APOSTROPHE },
  { .name = "S", .val = KEY_S },
  { .name = "ENTER", .val = KEY_ENTER },
  { .name = "H", .val = KEY_H },
  { .name = "7", .val = KEY_7 },
  { .name = "MINUS", .val = KEY_MINUS },
  { .name = "5", .val = KEY_5 },
  { .name = "Q", .val = KEY_Q },
  { .name = "J", .val = KEY_J },
  { .name = "PU", .val = KEY_PU },
  { .name = "F5", .val = KEY_F5 },
  { .name = "F1", .val = KEY_F1 },
  { .name = "MINUS", .val = KEY_MINUS },
  { .name = "1", .val = KEY_1 },
  { .name = "U", .val = KEY_U },
  { .name = "L", .val = KEY_L },
  { .name = "W", .val = KEY_W },
  { .name = "3", .val = KEY_3 },
  { .name = "ESC", .val = KEY_ESC },
  { .name = "F3", .val = KEY_F3 },
  { .name = "BACKSLASH", .val = KEY_BACKSLASH },
  { .name = "PS", .val = KEY_PS },
  { .name = "K", .val = KEY_K },
  { .name = "P", .val = KEY_P },
  { .name = "4", .val = KEY_4 },
  { .name = "F4", .val = KEY_F4 },
  { .name = "F6", .val = KEY_F6 },
  { .name = "HM", .val = KEY_HM },
  { .name = "6", .val = KEY_6 },
  { .name = "I", .val = KEY_I },
  { .name = "R", .val = KEY_R },
  { .name = "2", .val = KEY_2 },
  { .name = "F2", .val = KEY_F2 },
  { .name = "T", .val = KEY_T },
  { .name = "O", .val = KEY_O },
  { .name = "LSHIFT", .val = KEY_LSHIFT },
  { .name = "0", .val = KEY_0 },
  { .name = "TILDE", .val = KEY_TILDE },
  { .name = "BACKSPACE", .val = KEY_BACKSPACE },
  { .name = "X", .val = KEY_X },
  { .name = "UP", .val = KEY_UP },
  { .name = "C", .val = KEY_C },
  { .name = "Z", .val = KEY_Z },
  { .name = "MENU", .val = KEY_MENU },
  { .name = "LEFT", .val = KEY_LEFT },
  { .name = "B", .val = KEY_B },
  { .name = "DOWN", .val = KEY_DOWN },
  { .name = "GT", .val = KEY_GT },
  { .name = "RIGHT", .val = KEY_RIGHT },
  { .name = "LCTRL", .val = KEY_LCTRL },
  { .name = "RALT", .val = KEY_RALT },
  { .name = "FN", .val = KEY_FN },
  { .name = "LT", .val = KEY_LT },
  { .name = "N", .val = KEY_N },
  { .name = "LALT", .val = KEY_LALT },
  { .name = "RCTRL", .val = KEY_RCTRL },
  { .name = "WIN", .val = KEY_WIN },
  { .name = "QMARK", .val = KEY_QMARK },
  { .name = "SPACE", .val = KEY_SPACE },
  { .name = "V", .val = KEY_V },
  { .name = "M", .val = KEY_M },
  { .name = "RSHIFT", .val = KEY_RSHIFT }
};

const size_t keys_sz = sizeof keys / sizeof keys[0];

const struct kbd_key_t *kbd_get_key(const char *name) {
  for (int i = 0; i < keys_sz; i++) {
    if (strcmp(name, keys[i].name) == 0) {
      return &keys[i];
    }
  }
  return NULL;
}

void kbd_set_color(libusb_device_handle *kbdh, uint8_t r, uint8_t g, uint8_t b) {
  kbd_va_send_and_recv(
    kbdh,
    11,
    0x04, 0x0d, 0x01, 0x06, 0x03, 0x05, 0x00, 0x00, r, b, g);
}

void kbd_set_rainbow(libusb_device_handle *kbdh, bool rainbow) {
  if (rainbow) {
    kbd_va_send_and_recv(kbdh, 8, 0x04, 0x36, 0x00, 0x06, 0x01, 0x2e, 0x00, 0x01);
  } else {
    kbd_va_send_and_recv(kbdh, 8, 0x04, 0x35, 0x00, 0x06, 0x01, 0x2e);
  }
}

enum kbd_mode_t kbd_get_mode(const char *modestr) {
  if (strcmp(modestr, "norm") == 0) {
    return KBD_MODE_NORMAL;
  } else if (strcmp(modestr, "custom") == 0) {
    return KBD_MODE_CUSTOM;
  } else if (strcmp(modestr, "stream") == 0) {
    return KBD_MODE_STREAM;
  } else if (strcmp(modestr, "clouds") == 0) {
    return KBD_MODE_CLOUDS;
  } else if (strcmp(modestr, "swirl") == 0) {
    return KBD_MODE_SWIRL;
  } else if (strcmp(modestr, "rgb-breath") == 0) {
    return KBD_MODE_RAINBOW_BREATHING;
  } else if (strcmp(modestr, "breath") == 0) {
    return KBD_MODE_BREATHING;
  } else if (strcmp(modestr, "hotmap") == 0) {
    return KBD_MODE_HOTMAP;
  } else if (strcmp(modestr, "ripple") == 0) {
    return KBD_MODE_RIPPLE;
  } else if (strcmp(modestr, "ripple-lines") == 0) {
    return KBD_MODE_RIPPLE_LINES;
  } else if (strcmp(modestr, "snow") == 0) {
    return KBD_MODE_SNOW;
  } else if (strcmp(modestr, "rgb-dots") == 0) {
    return KBD_MODE_RAINBOW_DOTS;
  } else if (strcmp(modestr, "rgb-lines") == 0) {
    return KBD_MODE_RAINBOW_LINES;
  } else if (strcmp(modestr, "triangular") == 0) {
    return KBD_MODE_TRAINGULAR_WAVES;
  } else if (strcmp(modestr, "drain") == 0) {
    return KBD_MODE_DRAIN;
  } else if (strcmp(modestr, "matrix") == 0) {
    return KBD_MODE_MATRIX;
  } else if (strcmp(modestr, "scanline") == 0) {
    return KBD_MODE_SCANLINE;
  } else if (strcmp(modestr, "gradient") == 0) {
    return KBD_MODE_GRADIENT;
  } else if (strcmp(modestr, "rgb-circle") == 0) {
    return KBD_MODE_RAINBOW_CIRCLES;
  }

  return KBD_MODE_NORMAL;
}

void kbd_print_modes(void) {
  puts("    NAME         -- ORIGINAL NAME");
  puts("    norm         -- Normally on");
  puts("    custom       -- Custom settings");
  puts("    stream       -- Go with the stream");
  puts("    clouds       -- Clouds fly");
  puts("    swirl        -- Winding paths");
  puts("    rgb-breath   -- The trial of light");
  puts("    breath       -- Breathing");
  puts("    hotmap       -- Pass without trace");
  puts("    ripple       -- Ripple graff");
  puts("    ripple-lines -- Fast run without trace");
  puts("    snow         -- Snow winter jasmine");
  puts("    rgb-dots     -- Flowers blooming");
  puts("    rgb-lines    -- Swift action");
  puts("    triangular   -- Hurricane");
  puts("    drain        -- Accumulate");
  puts("    matrix       -- Digital times");
  puts("    scanline     -- Both ways");
  puts("    gradient     -- Surmount");
  puts("    rgb-circle   -- Fast and the Furious");
}

void kbd_set_mode(libusb_device_handle *kbdh, enum kbd_mode_t mode) {
  kbd_va_send_and_recv(
    kbdh, 9, 0x04, mode, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, mode - 0x07);
}

void kbd_remap(libusb_device_handle *kbdh, struct kbd_keymap_t *k) {
  uint8_t line1[64] = {
    0x04,   0xd4,  0x02,   0x08,    0x38,  0x00,     0x00,  0x00,
    0x02,   0x02,  k->esc, 0x02,    0x02,  k->tilde, 0x02,  0x02,
    k->tab, 0x02,  0x02,   k->caps, 0x02,  k->lshift == 0x02 ? 0x01 : 0x02,     k->lshift,  0x02,
    k->lctrl == KBD_KMAP_LCTRL ? 0x01 : 0x02,   k->lctrl,  0x02,   0x02,    k->f1, 0x02,     0x02,  k->k1,
    0x02,   0x02,  k->q,   0x02,    0x02,  k->a,     0x02,  0x02,
    0x64,   0x02,  k->meta == KBD_KMAP_META ? 0x01 : 0x02,   k->meta,    0x02,  0x02,     k->f2, 0x02,
    0x02,   k->k2, 0x02,   0x02,    k->w,  0x02,     0x02,  k->s,
    0x02,   0x02,  k->z,   0x02,    k->lalt == KBD_KMAP_LALT ? 0x01 : 0x02,  k->lalt,     0x02,  0x02,
  };

  uint8_t line2[64] =  {
    0x04,  0x13, 0x03,  0x08,  0x38,  0x38,  0x00,  0x00,
    k->f3, 0x02, 0x02,  k->k3, 0x02,  0x02,  k->e,  0x02,
    0x02,  k->d, 0x02,  0x02,  k->x,  0x02,  0x02,  k->space,
    0x02,  0x02, k->f4, 0x02,  0x02,  k->k4, 0x02,  0x02,
    k->r,  0x02, 0x02,  k->f,  0x02,  0x02,  k->c,  0x02,
    k->ralt == KBD_KMAP_LALT ? 0x01 : 0x02,  k->ralt, 0x02,  0x02,  k->f5, 0x02,  0x02,  k->k5,
    0x02,  0x02, k->t,  0x02,  0x02,  k->g,  0x02,  0x02,
    k->v,  0x05, 0x02,  0x02,  0x02,  0x02,  k->f7, 0x02,
  };

  uint8_t line3[64] = {
    0x04,  0xce,  0x03,  0x08,  0x38,  0x70,  0x00, 0x00,
    0x02,  k->k6, 0x02,  0x02,  k->y,  0x02,  0x02, k->h,
    0x02,  0x02,  k->b,  0x02,  0x02,  k->menu,  0x02, 0x02,
    k->f7, 0x02,  0x02,  k->k7, 0x02,  0x02,  k->u, 0x02,
    0x02,  k->j,  0x02,  0x02,  k->n,  0x02,  0x01, 0x80,
    0x02,  0x02,  k->f8, 0x02,  0x02,  k->k8, 0x02, 0x02,
    k->i,  0x02,  0x02,  k->k,  0x02,  0x02,  k->m, 0x02,
    k->rctrl == KBD_KMAP_RCTRL ? 0x01 : 0x02,  k->rctrl,  0x02,  0x02,  k->f9, 0x02,  0x02, k->k9
  };

  uint8_t line4[64] = {
    0x04,     0xeb,   0x03,     0x08,     0x38,      0xa8,    0x00,          0x00,
    0x02,     0x02,   k->o,     0x02,     0x02,      k->l,    0x02,          0x02,
    k->comma, 0x02,   0x00,     0x00,     0x02,      0x02,    k->f10,        0x02,
    0x02,     k->k0,  0x02,     0x02,     k->p,      0x02,    0x02,          k->colon,
    0x02,     0x02,   k->dot,   0x02,     0x00,      0x00,    0x02,          0x02,
    k->f11,   0x02,   0x02,     k->minus, 0x02,      0x02,    k->lbracket,   0x02,
    0x02,     k->ap,  0x02,     0x02,     k->slash,  0x02,    0x00,          0x00,
    0x02,     0x02,   k->f12,   0x02,     0x02,      k->plus, 0x02,          0x02
  };

  uint8_t line5[64] = {
    0x04,        0xc0,   0x04,         0x08,  0x38,   0xe0,    0x00,    0x00,
    k->rbracket, 0x02,   0x00,         0x00,  0x02,   0x02,    0x87,    0x02,
    0x00,        0x00,   0x02,         0x00,  0x00,   0x02,    0x02,    k->backspace,
    0x02,        0x02,   k->backslash, 0x02,  0x02,   k->enter,    0x02,    k->rshift == KBD_KMAP_RSHIFT ? 0x01 : 0x02,
    k->rshift,        0x02,   0x00,         0x00,  0x02,   0x02,    k->ps,   0x02,
    0x02,        k->ins, 0x02,         0x02,  k->del, 0x02,    0x00,    0x00,
    0x02,        0x00,   0x00,         0x02,  0x02,   k->left, 0x02,    0x02,
    k->sl,       0x02,   0x02,         k->hm, 0x02,   0x02,    k->end,  0x02
  };

  uint8_t line6[64] = {
    0x04,  0xe2,     0x04,  0x08, 0x38,  0x18,  0x01, 0x00,
    0x00,  0x00,     0x02,  0x02, k->up, 0x02,  0x02, k->down,
    0x02,  0x02,     k->pb, 0x02, 0x02,  k->pu, 0x02, 0x02,
    k->pd, 0x02,     0x00,  0x00, 0x02,  0x00,  0x00, 0x02,
    0x02,  k->right, 0x02,  0x00, 0x00,  0x02,  0x02, 0x53,
    0x02,  0x02,     0x5f,  0x02, 0x02,  0x5c,  0x02, 0x02,
    0x59,  0x02,     0x00,  0x00, 0x02,  0x00,  0x00, 0x02,
    0x02,  0x54,     0x02,  0x02, 0x60,  0x02,  0x02, 0x5d
  };

  uint8_t line7[64] = {
    0x04, 0x65, 0x05, 0x08, 0x2a, 0x50, 0x01, 0x00,
    0x02, 0x02, 0x5a, 0x02, 0x02, 0x62, 0x02, 0x00,
    0x00, 0x02, 0x02, 0x55, 0x02, 0x02, 0x61, 0x02,
    0x02, 0x5e, 0x02, 0x02, 0x5b, 0x02, 0x02, 0x63,
    0x02, 0x00, 0x00, 0x02, 0x02, 0x56, 0x02, 0x02,
    0x57, 0x02, 0x00, 0x00, 0x02, 0x02, 0x58, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x02,
    0x02, 0x54, 0x02, 0x02, 0x60, 0x02, 0x02, 0x5d
  };

  kbd_va_send_and_recv(
    kbdh, 15,
    0x04, 0x2a, 0x01, 0x0a, 0x10, 0x00, 0x00, 0x00,
    0xaa, 0x55, 0x10, 0x00, 0x00, 0x00, 0x01);
  kbd_send_and_recv(kbdh, line1);
  kbd_send_and_recv(kbdh, line2);
  kbd_send_and_recv(kbdh, line3);
  kbd_send_and_recv(kbdh, line4);
  kbd_send_and_recv(kbdh, line5);
  kbd_send_and_recv(kbdh, line6);
  kbd_send_and_recv(kbdh, line7);
}
