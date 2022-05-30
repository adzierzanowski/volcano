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
  printf("send and receive");
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
