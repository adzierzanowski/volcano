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

const struct key_t keys[88] = {
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

const struct key_t *kbd_get_key(const char *name) {
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
