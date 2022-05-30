#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_VID 0x0c45
#define KBD_PID 0x0820
#define CTL_REQUEST_TYPE 0x21
#define CTL_REQUEST 0x09
#define CTL_REQUEST_VALUE 0x0204
#define CTL_INTERFACE 0x01
#define CTL_ENDPOINT 0x82

#include <stdbool.h>
#include <string.h>

#include <libusb.h>

#include "fmt.h"

bool kbd_open(libusb_context **ctx, libusb_device_handle **kbdh);
bool kbd_close(libusb_context *ctx, libusb_device_handle *kbdh);
bool kbd_send(libusb_device_handle *kbdh, uint8_t msg[64]);
bool kbd_recv(
  libusb_device_handle *kbdh, size_t sz, uint8_t *buf, unsigned int timeout);
bool kbd_send_and_recv(libusb_device_handle *kbdh, uint8_t msg[64]);
bool kbd_va_send_and_recv(libusb_device_handle *kbdh, size_t len, ...);

#endif
