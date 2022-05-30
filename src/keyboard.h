#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_VID 0x0c45
#define KBD_PID 0x0820
#define CTL_REQUEST_TYPE 0x21
#define CTL_REQUEST 0x09
#define CTL_REQUEST_VALUE 0x0204
#define CTL_INTERFACE 0x01
#define CTL_ENDPOINT 0x82

#define KEY_ESC 0x140000
#define KEY_F1 0x170300
#define KEY_F2 0x1a0600
#define KEY_F3 0x1d0900
#define KEY_F4 0x200c00
#define KEY_F5 0x230f00
#define KEY_F6 0x261200
#define KEY_F7 0x291500
#define KEY_F8 0x2c1800
#define KEY_F9 0x2f1b00
#define KEY_F10 0x321e00
#define KEY_F11 0x352100
#define KEY_F12 0x382400
#define KEY_PS 0x3e2a00
#define KEY_SL 0x412d00
#define KEY_PB 0x443000

#define KEY_TILDE 0x533f00
#define KEY_1 0x564200
#define KEY_2 0x594500
#define KEY_3 0x5c4800
#define KEY_4 0x5f4b00
#define KEY_5 0x624e00
#define KEY_6 0x655100
#define KEY_7 0x685400
#define KEY_8 0x6b5700
#define KEY_9 0x6e5a00
#define KEY_0 0x715d00
#define KEY_MINUS 0x020000
#define KEY_PLUS 0x776300
#define KEY_BACKSPACE 0x7a6600
#define KEY_INS 0x7d6900
#define KEY_HM 0x806c00
#define KEY_PU 0x836f00

#define KEY_TAB 0x927e00
#define KEY_Q 0x958100
#define KEY_W 0x988400
#define KEY_E 0x9b8700
#define KEY_R 0x9e8a00
#define KEY_T 0xa18d00
#define KEY_Y 0xa49000
#define KEY_U 0xa79300
#define KEY_I 0xaa9600
#define KEY_O 0xad9900
#define KEY_P 0xb09c00
#define KEY_LBRACKET 0xb39f00
#define KEY_RBRACKET 0xb6a200
#define KEY_BACKSLASH 0xb9a500
#define KEY_END 0xbfab00
#define KEY_PD 0xc2ae00

#define KEY_CAPS 0xd1bd00
#define KEY_A 0xd4c000
#define KEY_S 0xd7c300
#define KEY_D 0xdac600
#define KEY_F 0xddc900
#define KEY_G 0xe0cc00
#define KEY_H 0xe3cf00
#define KEY_J 0xe6d200
#define KEY_K 0xe9d500
#define KEY_L 0xecd800
#define KEY_COLON 0xefdb00
#define KEY_APOSTROPHE 0xf2de00
#define KEY_ENTER 0xf8e400

#define KEY_LSHIFT 0x10fc00
#define KEY_Z 0x170201
#define KEY_X 0x1a0501
#define KEY_C 0x1d0801
#define KEY_V 0x200b01
#define KEY_B 0x230e01
#define KEY_N 0x261101
#define KEY_M 0x291401
#define KEY_LT 0x2c1701
#define KEY_GT 0x2f1a01
#define KEY_QMARK 0x321d01
#define KEY_RSHIFT 0x382301
#define KEY_UP 0x3e2901

#define KEY_LCTRL 0x503b01
#define KEY_WIN 0x533e01
#define KEY_LALT 0x564101
#define KEY_SPACE 0x594401
#define KEY_RALT 0x5c4701
#define KEY_FN 0x5f4a01
#define KEY_MENU 0x624d01
#define KEY_RCTRL 0x685301
#define KEY_LEFT 0x7a6501
#define KEY_DOWN 0x7d6801
#define KEY_RIGHT 0x806b01

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
bool kbd_set_key_color(libusb_device_handle *kbdh, uint32_t key, uint32_t color);

#endif
