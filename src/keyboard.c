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
    0x04,         0xd4,        0x02,       0x08,
    0x38,         0x00,        0x00,       0x00,
    k->esc_mk,    k->esc_m,    k->esc,     k->tilde_mk,
    k->tilde_m,   k->tilde,    k->tab_mk,  k->tab_m,
    k->tab,       k->caps_mk,  k->caps_m,  k->caps,
    k->lshift_mk, k->lshift_m, k->lshift,  k->lctrl_mk,
    k->lctrl_m,   k->lctrl,    k->f1_mk,   k->f1_m,
    k->f1,        k->k1_mk,    k->k1_m,    k->k1,
    k->q_mk,      k->q_m,      k->q,       k->a_mk,
    k->a_m,       k->a,        0x02,       0x02,
    0x64,         k->meta_mk,  k->meta_m,  k->meta,
    k->f2_mk,     k->f2_m,     k->f2,      k->k2_mk,
    k->k2_m,      k->k2,       k->w_mk,    k->w_m,
    k->w,         k->s_mk,     k->s_m,     k->s,
    k->z_mk,      k->z_m,      k->z,       k->lalt_mk,
    k->lalt_m,    k->lalt,     k->f3_mk,   k->f3_m,
  };

  uint8_t line2[64] =  {
    0x04,      0x13,        0x03,        0x08,
    0x38,      0x38,        0x00,        0x00,
    k->f3,     k->k3_mk,    k->k3_m,     k->k3,
    k->e_mk,   k->e_m,      k->e,        k->d_mk,
    k->d_m,    k->d,        k->x_mk,     k->x_m,
    k->x,      k->space_mk, k->space_m,  k->space,
    k->f4_mk,  k->f4_m,     k->f4,       k->k4_mk,
    k->k4_m,   k->k4,       k->r_mk,     k->r_m,
    k->r,      k->f_mk,     k->f_m,      k->f,
    k->c_mk,   k->c_m,      k->c,        k->ralt_mk,
    k->ralt_m, k->ralt,     k->f5_mk,    k->f5_m,
    k->f5,     k->k5_mk,    k->k5_m,     k->k5,
    k->t_mk,   k->t_m,      k->t,        k->g_mk,
    k->g_m,    k->g,        k->v_mk,     k->v_m,
    k->v,      0x05,        0x02,        0x02,
    k->f6_mk,  k->f6_m,     k->f6,       k->k6_mk,
  };

  uint8_t line3[64] = {
    0x04,       0xce,     0x03,     0x08,
    0x38,       0x70,     0x00,     0x00,
    k->k6_m,    k->k6,    k->y_mk,  k->y_m,
    k->y,       k->h_mk,  k->h_m,   k->h,
    k->b_mk,    k->b_m,   k->b,     k->menu_mk,
    k->menu_m,  k->menu,  k->f7_mk, k->f7_m,
    k->f7,      k->k7_mk, k->k7_m,  k->k7,
    k->u_mk,    k->u_m,   k->u,     k->j_mk,
    k->j_m,     k->j,     k->n_mk,  k->n_m,
    k->n,       0x02,     0x01,     0x80,
    k->f8_mk,   k->f8_m,  k->f8,    k->k8_mk,
    k->k8_m,    k->k8,    k->i_mk,  k->i_m,
    k->i,       k->k_mk,  k->k_m,   k->k,
    k->m_mk,    k->m_m,   k->m,     k->rctrl_mk,
    k->rctrl_m, k->rctrl, k->f9_mk, k->f9_m,
    k->f9,      k->k9_mk, k->k9_m,  k->k9
  };

  uint8_t line4[64] = {
    0x04,           0xeb,          0x03,           0x08,
    0x38,           0xa8,          0x00,           0x00,
    k->o_mk,        k->o_m,        k->o,           k->l_mk,
    k->l_m,         k->l,          k->comma_mk,    k->comma_m,
    k->comma,       0x02,          0x00,           0x00,
    k->f10_mk,      k->f10_m,      k->f10,         k->k0_mk,
    k->k0_m,        k->k0,         k->p_mk,        k->p_m,
    k->p,           k->colon_mk,   k->colon_m,     k->colon,
    k->dot_mk,      k->dot_m,      k->dot,         0x02,
    0x00,           0x00,          k->f11_mk,      k->f11_m,
    k->f11,         k->minus_mk,   k->minus_m,     k->minus,
    k->lbracket_mk, k->lbracket_m, k->lbracket,    k->ap_mk,
    k->ap_m,        k->ap,         k->slash_mk,    k->slash_m,
    k->slash,       0x02,          0x00,           0x00,
    k->f12_mk,      k->f12_m,      k->f12,         k->plus_mk,
    k->plus_m,      k->plus,       k->rbracket_mk, k->rbracket_m
  };

  uint8_t line5[64] = {
    0x04,            0xc0,            0x04,           0x08,
    0x38,            0xe0,            0x00,           0x00,
    k->rbracket,     0x02,            0x00,           0x00,
    0x02,            0x02,            0x87,           0x02,
    0x00,            0x00,            0x02,           0x00,
    0x00,            k->backspace_mk, k->backspace_m, k->backspace,
    k->backslash_mk, k->backslash_m,  k->backslash,   k->enter_mk,
    k->enter_m,      k->enter,        k->rshift_mk,   k->rshift_m,
    k->rshift,       0x02,            0x00,           0x00,
    k->ps_mk,        k->ps_m,         k->ps,          k->ins_mk,
    k->ins_m,        k->ins,          k->del_mk,      k->del_m,
    k->del,          0x02,            0x00,           0x00,
    0x02,            0x00,            0x00,           k->left_mk,
    k->left_m,       k->left,         k->sl_mk,       k->sl_m,
    k->sl,           k->hm_mk,        k->hm_m,        k->hm,
    k->end_mk,       k->end_m,        k->end,         0x02
  };

  uint8_t line6[64] = {
    0x04,       0xe2,       0x04,      0x08,
    0x38,       0x18,       0x01,      0x00,
    0x00,       0x00,       k->up_mk,  k->up_m,
    k->up,      k->down_mk, k->down_m, k->down,
    k->pb_mk,   k->pb_m,    k->pb,     k->pu_mk,
    k->pu_m,    k->pu,      k->pd_mk,  k->pd_m,
    k->pd,      0x02,       0x00,      0x00,
    0x02,       0x00,       0x00,      k->right_mk,
    k->right_m, k->right,   0x02,      0x00,
    0x00,       0x02,       0x02,      0x53,
    0x02,       0x02,       0x5f,      0x02,
    0x02,       0x5c,       0x02,      0x02,
    0x59,       0x02,       0x00,      0x00,
    0x02,       0x00,       0x00,      0x02,
    0x02,       0x54,       0x02,      0x02,
    0x60,       0x02,       0x02,      0x5d
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
  if (!vlc_kbd_va_send_and_recv(
    kbdh,
    9, 0x04, 0x0a+dir, 0x00, 0x06, 0x01, 0x03, 0x00, 0x00, dir)) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

bool vlc_kbd_set_gradient(libusb_device_handle *kbdh, enum vlc_kbd_gradient_t gradient) {
  if (!vlc_kbd_send_start(kbdh)) return false;
  if (!vlc_kbd_va_send_and_recv(
      kbdh, 9, 0x04, gradient+0x18, 0x00, 0x06, 0x01, 0x11, 0x00, 0x00, gradient
  )) return false;
  if (!vlc_kbd_send_end(kbdh)) return false;
  return true;
}

struct vlc_kbd_macro_t *vlc_kbd_macro_new(const char *name, uint8_t cycle_count) {
  struct vlc_kbd_macro_t *macro = malloc(sizeof (struct vlc_kbd_macro_t));
  macro->name_length = strlen(name);
  macro->name = calloc(macro->name_length + 1, sizeof (char));
  sprintf(macro->name, "%s", name);
  macro->entries_length = 0;
  macro->entries = NULL;
  macro->cycle_count = cycle_count;
  return macro;
}

void vlc_kbd_macro_free(struct vlc_kbd_macro_t *macro) {
  for (int i = 0; i < macro->entries_length; i++) {
    free(macro->entries[i]);
  }
  free(macro->entries);
  free(macro->name);
  free(macro);
}

void vlc_kbd_macro_add_entry(
    struct vlc_kbd_macro_t *macro,
    enum vlc_kbd_keycode_t kcode,
    bool is_modifier,
    uint16_t delay,
    bool down) {
  macro->entries_length++;
  macro->entries = realloc(macro->entries, sizeof (struct vlc_kbd_macro_entry_t *) * macro->entries_length);

  struct vlc_kbd_macro_entry_t *e = malloc(sizeof (struct vlc_kbd_macro_entry_t));
  e->keycode = kcode;
  e->modifier = is_modifier;
  e->delay = delay;
  e->down = down;

  macro->entries[macro->entries_length - 1] = e;
}

void vlc_kbd_macro_payload(
    uint8_t *dst, struct vlc_kbd_macro_t **mkrs, uint16_t mkrcnt) {
  // Magic numbers
  dst[0] = 0xaa;
  dst[1] = 0x55;

  // Copy the length of the entire payload at 0x04 offset
  uint16_t *ptr = (uint16_t *) &dst[4];
  ptr[0] = mkrcnt;

  // Keep the count of the current total length of the data
  uint16_t total = 0;

  // At offset 0x10 there's information about offsets of the macro definitions
  ptr = (uint16_t *) &dst[16];

  // For each macro, add its offset
  for (int i = 0; i < mkrcnt; i++) {
    struct vlc_kbd_macro_t *m = mkrs[i];
    // The whole data of the macro consists of its entries (4 bytes each),
    // its metadata (4 bytes: entries length, and name length)
    // and its name (2-byte chars)
    uint16_t mkrsz = m->entries_length * 4 + 4 + m->name_length * 2;
    // Add its offset info
    ptr[i] = 16 + 2 * mkrcnt + total;
    total += mkrsz;
  }

  // Now, having the total payload length, we can put it into 16-bit field
  // at offset 0x02
  total += 16 + 2 * mkrcnt;
  ptr = (uint16_t *) &dst[2];
  ptr[0] = total;

  ptr = (uint16_t *) &dst[16];
  for (int i = 0; i < mkrcnt; i++) {
    struct vlc_kbd_macro_t *mkr = mkrs[i];
    uint16_t moffset = ptr[i];
    uint16_t *mptr = (uint16_t *) &dst[moffset];

    mptr[0] = mkr->entries_length;
    mptr[1] = (mkr->name_length << 8) | 0x01;

    mptr = &mptr[2];

    for (int j = 0; j < mkr->entries_length; j++) {
      struct vlc_kbd_macro_entry_t *e = mkr->entries[j];
      uint16_t dnd = ((e->delay / 10) & 0x0fff) | (e->down ? 0xa000 : 0x2000);
      uint16_t modlen = (e->keycode << 8) | (e -> modifier ? 0x01 : 0x02);
      mptr[0] = dnd;
      mptr[1] = modlen;
      mptr = &mptr[2];
    }

    for (int j = 0; j < mkr->name_length; j++) {
      mptr[j] = mkr->name[j];
    }
  }

  // Fill the remaining bytes with the previous packet's payload chunk
  while (total % 56 != 0) {
    dst[total] = dst[total - 56];
    total++;
  }
}

uint8_t **vlc_kbd_macro_packets(size_t *pck_cnt, uint8_t *payload) {
  // Grab the length of the payload from itself
  uint16_t *pldsz = (uint16_t *) &payload[2];

  // Count how many packets are required
  double pck_cntf = *pldsz / 56.0;
  *pck_cnt = ceil(pck_cntf);

  uint8_t **packets = calloc(*pck_cnt, sizeof (uint8_t *));
  for (int i = 0; i < *pck_cnt; i++) {
    uint8_t *pck = calloc(64, sizeof (uint8_t));
    packets[i] = pck;

    pck[0] = 0x04;
    pck[1] = 0x00;
    pck[2] = 0x00;
    pck[3] = 0x0a;
    pck[4] = i == *pck_cnt - 1 ? 0x08 : 0x38;
    uint16_t *payload_offset = (uint16_t *) &pck[5];
    *payload_offset = 56 * i;

    for (int j = 0; j < 56; j++) {
      pck[j+8] = payload[*payload_offset + j];
    }
  }

  return packets;
}
