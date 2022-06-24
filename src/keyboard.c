#include "keyboard.h"


const struct vlc_kbd_key_t vlc_kcolor_keys[111] = {
  { .name = "ESC", .val = 0x140000 },
  { .name = "F1", .val = 0x170300 },
  { .name = "F2", .val = 0x1a0600 },
  { .name = "F3", .val = 0x1d0900 },
  { .name = "F4", .val = 0x200c00 },
  { .name = "F5", .val = 0x230f00 },
  { .name = "F6", .val = 0x261200 },
  { .name = "F7", .val = 0x291500 },
  { .name = "F8", .val = 0x2c1800 },
  { .name = "F9", .val = 0x2f1b00 },
  { .name = "F10", .val = 0x301e00 },
  { .name = "F11", .val = 0x332100  },
  { .name = "F12", .val = 0x362400 },
  { .name = "PS", .val = 0x3e2a00 },
  { .name = "SL", .val = 0x412d00 },
  { .name = "PB", .val = 0x443000 },
  { .name = "PAUSE", .val = 0x443000 },
  { .name = "BREAK", .val = 0x443000 },

  { .name = "TILDE", .val = 0x533f00 },
  { .name = "BACKTICK", .val = 0x533f00 },
  { .name = "1", .val = 0x564200 },
  { .name = "2", .val = 0x594500 },
  { .name = "3", .val = 0x5c4800 },
  { .name = "4", .val = 0x5f4b00 },
  { .name = "5", .val = 0x624e00 },
  { .name = "6", .val = 0x655100 },
  { .name = "7", .val = 0x685400 },
  { .name = "8", .val = 0x6b5700 },
  { .name = "9", .val = 0x6e5a00 },
  { .name = "0", .val = 0x715d00 },
  { .name = "MINUS", .val = 0x736000 },
  { .name = "UNDERSCORE", .val = 0x736000 },
  { .name = "PLUS", .val = 0x776300 },
  { .name = "EQ", .val = 0x776300 },
  { .name = "BACKSPACE", .val = 0x7a6600 },
  { .name = "INS", .val = 0x7d6900 },
  { .name = "INSERT", .val = 0x7d6900 },
  { .name = "HM", .val = 0x806c00 },
  { .name = "HOME", .val = 0x806c00 },
  { .name = "PU", .val = 0x836f00 },
  { .name = "PGUP", .val = 0x836f00 },

  { .name = "TAB", .val = 0x927e00 },
  { .name = "Q", .val = 0x958100 },
  { .name = "W", .val = 0x988400 },
  { .name = "E", .val = 0x9b8700 },
  { .name = "R", .val = 0x9e8a00 },
  { .name = "T", .val = 0xa18d00 },
  { .name = "Y", .val = 0xa49000 },
  { .name = "U", .val = 0xa79300 },
  { .name = "I", .val = 0xaa9600 },
  { .name = "O", .val = 0xad9900 },
  { .name = "P", .val = 0xb09c00 },
  { .name = "LBRACKET", .val = 0xb39f00 },
  { .name = "LBRACE", .val = 0xb39f00 },
  { .name = "RBRACKET", .val = 0xb4a200 },
  { .name = "RBRACE", .val = 0xb4a200 },
  { .name = "BACKSLASH", .val = 0xb9a500 },
  { .name = "PIPE", .val = 0xb9a500 },
  { .name = "DEL", .val = 0xbba800  },
  { .name = "DELETE", .val = 0xbba800  },
  { .name = "END", .val = 0xbdab00 },
  { .name = "PD", .val = 0xc0ae00 },
  { .name = "PGDN", .val = 0xc0ae00 },

  { .name = "CAPS", .val = 0xd1bd00 },
  { .name = "A", .val = 0xd4c000 },
  { .name = "S", .val = 0xd7c300 },
  { .name = "D", .val = 0xdac600 },
  { .name = "F", .val = 0xddc900 },
  { .name = "G", .val = 0xe0cc00 },
  { .name = "H", .val = 0xe3cf00 },
  { .name = "J", .val = 0xe6d200 },
  { .name = "K", .val = 0xe9d500 },
  { .name = "L", .val = 0xecd800 },
  { .name = "SEMICOLON", .val = 0xefdb00 },
  { .name = "COLON", .val = 0xefdb00 },
  { .name = "AP", .val = 0xf0de00 },
  { .name = "APOSTROPHE", .val = 0xf0de00 },
  { .name = "QUOTE", .val = 0xf0de00 },
  { .name = "ENTER", .val = 0xf6e400 },
  { .name = "RETURN", .val = 0xf6e400 },

  { .name = "LSHIFT", .val = 0x0efc00 },
  { .name = "Z", .val = 0x170201 },
  { .name = "X", .val = 0x1a0501 },
  { .name = "C", .val = 0x1d0801 },
  { .name = "V", .val = 0x200b01 },
  { .name = "N", .val = 0x261101 },
  { .name = "B", .val = 0x230e01 },
  { .name = "M", .val = 0x291401 },
  { .name = "COMMA", .val = 0x2a1701 },
  { .name = "LT", .val = 0x2a1701 },
  { .name = "DOT", .val = 0x2d1a01 },
  { .name = "GT", .val = 0x2d1a01 },
  { .name = "QMARK", .val = 0x301d01 },
  { .name = "SLASH", .val = 0x301d01 },
  { .name = "RSHIFT", .val = 0x362301 },
  { .name = "UP", .val = 0x3c2901 },

  { .name = "LCTRL", .val = 0x4e3b01 },
  { .name = "META", .val = 0x513e01 },
  { .name = "WIN", .val = 0x513e01 },
  { .name = "CMD", .val = 0x513e01 },
  { .name = "LALT", .val = 0x544101 },
  { .name = "LOPTION", .val = 0x544101 },
  { .name = "SPACE", .val = 0x574401 },
  { .name = "RALT", .val = 0x5a4701 },
  { .name = "ROPTION", .val = 0x5a4701 },
  { .name = "FN", .val = 0x5d4a01 },
  { .name = "MENU", .val = 0x604d01 },
  { .name = "RCTRL", .val = 0x665301 },
  { .name = "LEFT", .val = 0x786501 },
  { .name = "DOWN", .val = 0x7b6801 },
  { .name = "RIGHT", .val = 0x7e6b01 },
};

const size_t vlc_keys_sz = sizeof vlc_kcolor_keys / sizeof vlc_kcolor_keys[0];

bool vlc_kbd_open(libusb_context **ctx, libusb_device_handle **kbdh) {
  *ctx = NULL;
  *kbdh = NULL;

  int status = 0;
  libusb_init(ctx);
  if (status != 0) {
    vlc_print_libusb_err(status);
    return false;
  }

  *kbdh = libusb_open_device_with_vid_pid(*ctx, VLC_KBD_VID, VLC_KBD_PID);
  if (*kbdh == NULL) {
    vlc_log(VLC_LOG_ERROR, "Keyboard not found.\n");
    return false;
  }

  status = libusb_detach_kernel_driver(*kbdh, VLC_CTL_INTERFACE);
  if (status != 0) {
    vlc_print_libusb_err(status);
    libusb_close(*kbdh);
    return false;
  }

  status = libusb_claim_interface(*kbdh, VLC_CTL_INTERFACE);
  if (status != 0) {
    vlc_print_libusb_err(status);
    libusb_attach_kernel_driver(*kbdh, VLC_CTL_INTERFACE);
    return false;
  }

  //status = libusb_set_configuration(kbdh, 1);

  return true;
}

bool vlc_kbd_close(libusb_context *ctx, libusb_device_handle *kbdh) {
  libusb_release_interface(kbdh, VLC_CTL_INTERFACE);
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_ERROR);
  libusb_attach_kernel_driver(kbdh, VLC_CTL_INTERFACE);
  libusb_close(kbdh);
  libusb_exit(ctx);
  return true;
}

bool vlc_kbd_send(libusb_device_handle *kbdh, uint8_t msg[64]) {
  int transferred = libusb_control_transfer(
    kbdh,
    VLC_CTL_REQUEST_TYPE,
    VLC_CTL_REQUEST,
    VLC_CTL_REQUEST_VALUE,
    VLC_CTL_INTERFACE,
    msg,
    64,
    1000);

  if (transferred != 64) {
    vlc_print_libusb_err(transferred);
    return false;
  }

  return true;
}

bool vlc_kbd_recv(
  libusb_device_handle *kbdh, size_t sz, uint8_t *buf, unsigned int timeout) {
  memset(buf, 0, sz);

  // TODO:
  int transferred = 0;
  int status = libusb_bulk_transfer(
    kbdh, VLC_CTL_ENDPOINT, (unsigned char *) buf, sz, &transferred, timeout);

  if (status != 0) {
    vlc_print_libusb_err(status);
    return false;
  }

  return true;
}

bool kbd_validate_response(uint8_t *sent, uint8_t *recv, size_t sz) {
  for (int i = 0; i < sz; i++) {
    if (i == 7) {
      if (recv[i] != 1) {
        vlc_log(VLC_LOG_ERROR, "Response invalid (NACK)");
        return false;
      }
    } else {
      if ((uint8_t) sent[i] != (uint8_t) recv[i]) {
        vlc_log(VLC_LOG_ERROR, "Received data does not match.\n");
        vlc_log(VLC_LOG_ERROR, "  i=%d sent=%d received=%d\n", i, sent[i], recv[i]);
        return false;
      }
    }
  }
  return true;
}

bool vlc_kbd_send_and_recv(libusb_device_handle *kbdh, uint8_t msg[64]) {
  if (!vlc_kbd_send(kbdh, msg)) {
    return false;
  }

  uint8_t recv[64] = {0};

  if (!vlc_kbd_recv(kbdh, 64, (uint8_t *) recv, 1000)) {
    return false;
  }

  if (!kbd_validate_response(msg, recv, 64)) {
    return false;
  }

  return true;
}

bool vlc_kbd_va_send_and_recv(libusb_device_handle *kbdh, size_t len, ...) {
  uint8_t msg[64] = {0};

  va_list args;
  va_start(args, len);
  for (int i = 0; i < len; i++) {
    msg[i] = va_arg(args, int);
  }
  va_end(args);

  bool status = vlc_kbd_send_and_recv(kbdh, msg);

  return status;
}

bool vlc_kbd_set_key_color(libusb_device_handle *kbdh, uint32_t key, uint32_t color) {
  uint8_t d1 = (key >> 16) & 0xff;
  uint8_t d5 = (key >> 8) & 0xff;
  uint8_t d6 = key & 0xff;
  uint8_t r = (color >> 16) & 0xff;
  uint8_t g = (color >> 8) & 0xff;
  uint8_t b = color & 0xff;

  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_set_mode(kbdh, VLC_KBD_MODE_CUSTOM)) return false;
  if (!vlc_kbd_va_send_and_recv(
    kbdh, 11, 0x04, d1, 0x00, 0x11, 0x03, d5, d6, 0x00, r, b, g)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

bool vlc_kbd_send_start(libusb_device_handle *kbdh) {
  return vlc_kbd_va_send_and_recv(kbdh, 4, 0x04, 0x01, 0x00, 0x01);
}

bool vlc_kbd_send_end(libusb_device_handle *kbdh) {
  return vlc_kbd_va_send_and_recv(kbdh, 4, 0x04, 0x02, 0x00, 0x02);
}


const struct vlc_kbd_key_t *vlc_kbd_get_key(const char *name) {
  for (int i = 0; i < vlc_keys_sz; i++) {
    if (strcmp(name, vlc_kcolor_keys[i].name) == 0) {
      return &vlc_kcolor_keys[i];
    }
  }
  return NULL;
}

bool vlc_kbd_set_color(libusb_device_handle *kbdh, uint8_t r, uint8_t g, uint8_t b) {
  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_va_send_and_recv(
    kbdh,
    11,
    0x04, 0x0d, 0x01, 0x06, 0x03, 0x05, 0x00, 0x00, r, b, g)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

bool vlc_kbd_set_rainbow(libusb_device_handle *kbdh, bool rainbow) {
  if (!vlc_kbd_send_start(kbdh)) return false;
  if (rainbow) {
    if (!vlc_kbd_va_send_and_recv(
      kbdh, 9, 0x04, 0x0c, 0x00, 0x06, 0x01, 0x04, 0x00, 0x00, 0x01)) return false;
  } else {
    if (!vlc_kbd_va_send_and_recv(kbdh, 6, 0x04, 0x0b, 0x00, 0x06, 0x01, 0x04)) return false;
  }
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

enum vlc_kbd_mode_t vlc_kbd_get_mode(const char *modestr) {
  if (strcmp(modestr, "norm") == 0) {
    return VLC_KBD_MODE_NORMAL;
  } else if (strcmp(modestr, "custom") == 0) {
    return VLC_KBD_MODE_CUSTOM;
  } else if (strcmp(modestr, "stream") == 0) {
    return VLC_KBD_MODE_STREAM;
  } else if (strcmp(modestr, "clouds") == 0) {
    return VLC_KBD_MODE_CLOUDS;
  } else if (strcmp(modestr, "swirl") == 0) {
    return VLC_KBD_MODE_SWIRL;
  } else if (strcmp(modestr, "rgb-breath") == 0) {
    return VLC_KBD_MODE_RAINBOW_BREATHING;
  } else if (strcmp(modestr, "breath") == 0) {
    return VLC_KBD_MODE_BREATHING;
  } else if (strcmp(modestr, "hotmap") == 0) {
    return VLC_KBD_MODE_HOTMAP;
  } else if (strcmp(modestr, "ripple") == 0) {
    return VLC_KBD_MODE_RIPPLE;
  } else if (strcmp(modestr, "ripple-lines") == 0) {
    return VLC_KBD_MODE_RIPPLE_LINES;
  } else if (strcmp(modestr, "snow") == 0) {
    return VLC_KBD_MODE_SNOW;
  } else if (strcmp(modestr, "rgb-dots") == 0) {
    return VLC_KBD_MODE_RAINBOW_DOTS;
  } else if (strcmp(modestr, "rgb-lines") == 0) {
    return VLC_KBD_MODE_RAINBOW_LINES;
  } else if (strcmp(modestr, "triangular") == 0) {
    return VLC_KBD_MODE_TRAINGULAR_WAVES;
  } else if (strcmp(modestr, "drain") == 0) {
    return VLC_KBD_MODE_DRAIN;
  } else if (strcmp(modestr, "matrix") == 0) {
    return VLC_KBD_MODE_MATRIX;
  } else if (strcmp(modestr, "scanline") == 0) {
    return VLC_KBD_MODE_SCANLINE;
  } else if (strcmp(modestr, "gradient") == 0) {
    return VLC_KBD_MODE_GRADIENT;
  } else if (strcmp(modestr, "rgb-circle") == 0) {
    return VLC_KBD_MODE_RAINBOW_CIRCLES;
  }

  return VLC_KBD_MODE_UNKNOWN;
}

void vlc_kbd_print_modes(void) {
  puts("    NAME            Opts   ORIGINAL NAME");
  puts("    norm            -CR-    Normally on");
  puts("    custom                  Custom settings");
  puts("    stream          SCRD    Go with the stream");
  puts("    clouds          SCRD    Clouds fly");
  puts("    swirl           SCRD    Winding paths");
  puts("    rgb-breath      S---    The trial of light");
  puts("    breath          SCR-    Breathing");
  puts("    hotmap          SCR-    Pass without trace");
  puts("    ripple          SCR-    Ripple graff");
  puts("    ripple-lines    SCR-    Fast run without trace");
  puts("    snow            S---    Snow winter jasmine");
  puts("    rgb-dots        S---    Flowers blooming");
  puts("    rgb-lines       S--D    Swift action");
  puts("    triangular      SCR-    Hurricane");
  puts("    drain           SCR-    Accumulate");
  puts("    matrix          SCR-    Digital times");
  puts("    scanline        -CR-    Both ways");
  puts("    gradient        ----    Surmount");
  puts("    rgb-circle      ---D    Fast and the Furious");
}

bool vlc_kbd_set_mode(libusb_device_handle *kbdh, enum vlc_kbd_mode_t mode) {
  return vlc_kbd_va_send_and_recv(
    kbdh, 9, 0x04, mode, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, mode - 0x07);
}

bool vlc_kbd_remap(libusb_device_handle *kbdh, struct vlc_kbd_keymap_t *k) {
  uint8_t line1[64] = {
    0x04,        0xd4,        0x02,       0x08,
    0x38,        0x00,        0x00,       0x00,
    0x02,        k->esc_m,    k->esc,     0x02,
    k->tilde_m,  k->tilde,    0x02,       k->tab_m,
    k->tab,      0x02,        k->caps_m,  k->caps,
    0x02,        k->lshift_m, k->lshift,  0x02,
    k->lctrl_m,  k->lctrl,    0x02,       k->f1_m,
    k->f1,       0x02,        k->k1_m,    k->k1,
    0x02,        k->q_m,      k->q,       0x02,
    k->a_m,      k->a,        0x02,       0x02,
    0x64,        0x02,        k->meta_m,  k->meta,
    0x02,        k->f2_m,     k->f2,      0x02,
    k->k2_m,     k->k2,       0x02,       k->w_m,
    k->w,        0x02,        k->s_m,     k->s,
    0x02,        k->z_m,      k->z,       0x02,
    k->lalt_m,   k->lalt,     0x02,       0x02,
  };

  uint8_t line2[64] =  {
    0x04,      0x13,    0x03,        0x08,
    0x38,      0x38,    0x00,        0x00,
    k->f3,     0x02,    k->k3_m,     k->k3,
    0x02,      k->e_m,  k->e,        0x02,
    k->d_m,    k->d,    0x02,        k->x_m,
    k->x,      0x02,    k->space_m,  k->space,
    0x02,      k->f4_m, k->f4,       0x02,
    k->k4_m,   k->k4,   0x02,        k->r_m,
    k->r,      0x02,    k->f_m,      k->f,
    0x02,      k->c_m,  k->c,        0x02,
    k->ralt_m, k->ralt, 0x02,        k->f5_m,
    k->f5,     0x02,     k->k5_m,    k->k5,
    0x02,      k->t_m,   k->t,       0x02,
    k->g_m,    k->g,     0x02,       k->v_m,
    k->v,      0x05,     0x02,       0x02,
    0x02,      k->f7_m,  k->f7,      0x02,
  };

  uint8_t line3[64] = {
    0x04,       0xce,     0x03,    0x08,
    0x38,       0x70,     0x00,    0x00,
    k->k6_m,    k->k6,    0x02,    k->y_m,
    k->y,       0x02,     k->h_m,  k->h,
    0x02,       k->b_m,   k->b,    0x02,
    k->menu_m,  k->menu,  0x02,    k->f7_m,
    k->f7,      0x02,     k->k7_m, k->k7,
    0x02,       k->u_m,   k->u,    0x02,
    k->j_m,     k->j,     0x02,    k->n_m,
    k->n,       0x02,     0x01,    0x80,
    0x02,       k->f8_m,  k->f8,   0x02,
    k->k8_m,    k->k8,    0x02,    k->i_m,
    k->i,       0x02,     k->k_m,  k->k,
    0x02,       k->m_m,   k->m,    0x02,
    k->rctrl_m, k->rctrl, 0x02,    k->f9_m,
    k->f9,      0x02,     k->k9_m, k->k9
  };

  uint8_t line4[64] = {
    0x04,      0xeb,          0x03,        0x08,
    0x38,      0xa8,          0x00,        0x00,
    0x02,      k->o_m,        k->o,        0x02,
    k->l_m,    k->l,          0x02,        k->comma_m,
    k->comma,  0x02,          0x00,        0x00,
    0x02,      k->f10_m,      k->f10,      0x02,
    k->k0_m,   k->k0,         0x02,        k->p_m,
    k->p,      0x02,          k->colon_m,  k->colon,
    0x02,      k->dot_m,      k->dot,      0x02,
    0x00,      0x00,          0x02,        k->f11_m,
    k->f11,    0x02,          k->minus_m,  k->minus,
    0x02,      k->lbracket_m, k->lbracket, 0x02,
    k->ap_m,   k->ap,         0x02,        k->slash_m,
    k->slash,  0x02,          0x00,        0x00,
    0x02,      k->f12_m,      k->f12,      0x02,
    k->plus_m, k->plus,       0x02,        0x02
  };

  uint8_t line5[64] = {
    0x04,        0xc0,           0x04,           0x08,
    0x38,        0xe0,           0x00,           0x00,
    k->rbracket, 0x02,           0x00,           0x00,
    0x02,        0x02,           0x87,           0x02,
    0x00,        0x00,           0x02,           0x00,
    0x00,        0x02,           k->backspace_m, k->backspace,
    0x02,        k->backslash_m, k->backslash,   0x02,
    k->enter_m,  k->enter,       0x02,           k->rshift_m,
    k->rshift,   0x02,           0x00,           0x00,
    0x02,        k->ps_m,        k->ps,          0x02,
    k->ins_m,    k->ins,         0x02,           k->del_m,
    k->del,      0x02,           0x00,           0x00,
    0x02,        0x00,           0x00,           0x02,
    k->left_m,   k->left,        0x02,           k->sl_m,
    k->sl,       0x02,           k->hm_m,        k->hm,
    0x02,        k->end_m,       k->end,         0x02
  };

  uint8_t line6[64] = {
    0x04,       0xe2,     0x04,      0x08,
    0x38,       0x18,     0x01,      0x00,
    0x00,       0x00,     0x02,      k->up_m,
    k->up,      0x02,     k->down_m, k->down,
    0x02,       k->pb_m,  k->pb,     0x02,
    k->pu_m,    k->pu,    0x02,      k->pd_m,
    k->pd,      0x02,     0x00,      0x00,
    0x02,       0x00,     0x00,      0x02,
    k->right_m, k->right, 0x02,      0x00,
    0x00,       0x02,     0x02,      0x53,
    0x02,       0x02,     0x5f,      0x02,
    0x02,       0x5c,     0x02,      0x02,
    0x59,       0x02,     0x00,      0x00,
    0x02,       0x00,     0x00,      0x02,
    0x02,       0x54,     0x02,      0x02,
    0x60,       0x02,     0x02,      0x5d
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

  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_va_send_and_recv(
    kbdh, 15,
    0x04, 0x2a, 0x01, 0x0a, 0x10, 0x00, 0x00, 0x00,
    0xaa, 0x55, 0x10, 0x00, 0x00, 0x00, 0x01)) return false;
  if (!vlc_kbd_send_and_recv(kbdh, line1)) return false;
  if (!vlc_kbd_send_and_recv(kbdh, line2)) return false;
  if (!vlc_kbd_send_and_recv(kbdh, line3)) return false;
  if (!vlc_kbd_send_and_recv(kbdh, line4)) return false;
  if (!vlc_kbd_send_and_recv(kbdh, line5)) return false;
  if (!vlc_kbd_send_and_recv(kbdh, line6)) return false;
  if (!vlc_kbd_send_and_recv(kbdh, line7)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

bool vlc_kbd_read_keymap_file(const char *fname, struct vlc_kbd_keymap_t *kmap) {
  FILE *f = fopen(fname, "rb");
  if (f == NULL) return false;
  fread(kmap, sizeof (struct vlc_kbd_keymap_t), 1, f);
  fclose(f);
  return true;
}

bool vlc_kbd_set_brightness(libusb_device_handle *kbdh, uint8_t level) {
  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_va_send_and_recv(
    kbdh, 9, 0x04, 0x08 + level, 0x00, 0x06, 0x01, 0x01, 0x00, 0x00, level)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

bool vlc_kbd_set_speed(libusb_device_handle *kbdh, uint8_t level) {
  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_va_send_and_recv(
    kbdh, 9, 0x04, 0x0d - level, 0x00, 0x06, 0x01, 0x02, 0x00, 0x00, 0x04 - level)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

bool vlc_kbd_set_report_rate(libusb_device_handle *kbdh, enum vlc_kbd_rate_t rate) {
  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_va_send_and_recv(
    kbdh, 9, 0x04, 0x16 + rate, 0x00, 0x06, 0x01, 0x0f, 0x00, 0x00, rate)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

bool vlc_kbd_set_direction(libusb_device_handle *kbdh, enum vlc_kbd_dir_t dir) {
  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_va_send_and_recv(kbdh, 0, 0x04, 0x0a+dir, 0x00, 0x06, 0x01, 0x03, 0x00, 0x00, dir)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}
