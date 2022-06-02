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
#define KEY_MINUS 0x048200
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
#define KEY_DEL 0x04ca00
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

struct kbd_key_t {
  const char *name;
  uint32_t val;
};

struct kbd_keymap_t {
  uint8_t a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w;
  uint8_t x, y, z, k1, k2, k3, k4, k5, k6, k7, k8, k9, k0;
  uint8_t enter, esc, backspace, tab, space, minus, plus, lbracket, rbracket;
  uint8_t backslash, colon, ap, tilde, comma, dot, slash, caps;
  uint8_t f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;
  uint8_t ps, sl, pb, ins, hm, pu, del, end, pd, right, left, down, up;
  uint8_t lshift, rshift, lalt, ralt, lctrl, rctrl, meta, menu;

  uint8_t a_m, b_m, c_m, d_m, e_m, f_m, g_m, h_m, i_m, j_m, k_m, l_m, m_m, n_m;
  uint8_t o_m, p_m, q_m, r_m, s_m, t_m, u_m, v_m, w_m, x_m, y_m, z_m;
  uint8_t k1_m, k2_m, k3_m, k4_m, k5_m, k6_m, k7_m, k8_m, k9_m, k0_m;
  uint8_t enter_m, esc_m, backspace_m, tab_m, space_m, minus_m, plus_m;
  uint8_t lbracket_m, rbracket_m, backslash_m, colon_m, ap_m, tilde_m;
  uint8_t comma_m, dot_m, slash_m, caps_m;
  uint8_t f1_m, f2_m, f3_m, f4_m, f5_m, f6_m, f7_m, f8_m, f9_m, f10_m, f11_m;
  uint8_t f12_m, ps_m, sl_m, pb_m, ins_m, hm_m, pu_m, del_m, end_m, pd_m;
  uint8_t right_m, left_m, down_m, up_m, lshift_m, rshift_m, lalt_m, ralt_m;
  uint8_t lctrl_m, rctrl_m, meta_m, menu_m;
};

enum kbd_rate_t {
  KBD_RATE_125HZ = 0x00,
  KBD_RATE_250HZ = 0x01,
  KBD_RATE_500HZ = 0x02,
  KBD_RATE_1000HZ = 0x03,
};

enum kbd_keymap_key_t {
  KBD_KMAP_MODE_MODAL = 0x01,
  KBD_KMAP_MODE_NORMAL = 0x02,

  KBD_KMAP_LSHIFT = 0x02,
  KBD_KMAP_RSHIFT = 0x20,
  KBD_KMAP_LALT = 0x04,
  KBD_KMAP_RALT = 0x40,
  KBD_KMAP_LCTRL = 0x01,
  KBD_KMAP_RCTRL = 0x10,
  KBD_KMAP_META = 0x08,
  KBD_KMAP_MENU = 0x65,

  KBD_KMAP_A = 0x04,
  KBD_KMAP_B = 0x05,
  KBD_KMAP_C = 0x06,
  KBD_KMAP_D = 0x07,
  KBD_KMAP_E = 0x08,
  KBD_KMAP_F = 0x09,
  KBD_KMAP_G = 0x0a,
  KBD_KMAP_H = 0x0b,
  KBD_KMAP_I = 0x0c,
  KBD_KMAP_J = 0x0d,
  KBD_KMAP_K = 0x0e,
  KBD_KMAP_L = 0x0f,
  KBD_KMAP_M = 0x10,
  KBD_KMAP_N = 0x11,
  KBD_KMAP_O = 0x12,
  KBD_KMAP_P = 0x13,
  KBD_KMAP_Q = 0x14,
  KBD_KMAP_R = 0x15,
  KBD_KMAP_S = 0x16,
  KBD_KMAP_T = 0x17,
  KBD_KMAP_U = 0x18,
  KBD_KMAP_V = 0x19,
  KBD_KMAP_W = 0x1a,
  KBD_KMAP_X = 0x1b,
  KBD_KMAP_Y = 0x1c,
  KBD_KMAP_Z = 0x1d,
  KBD_KMAP_1 = 0x1e,
  KBD_KMAP_2 = 0x1f,
  KBD_KMAP_3 = 0x20,
  KBD_KMAP_4 = 0x21,
  KBD_KMAP_5 = 0x22,
  KBD_KMAP_6 = 0x23,
  KBD_KMAP_7 = 0x24,
  KBD_KMAP_8 = 0x25,
  KBD_KMAP_9 = 0x26,
  KBD_KMAP_0 = 0x27,
  KBD_KMAP_ENTER = 0x28,
  KBD_KMAP_ESC = 0x29,
  KBD_KMAP_BACKSPACE = 0x2a,
  KBD_KMAP_TAB = 0x2b,
  KBD_KMAP_SPACE = 0x2c,
  KBD_KMAP_MINUS = 0x2d,
  KBD_KMAP_PLUS = 0x2e,
  KBD_KMAP_LBRACKET = 0x2f,
  KBD_KMAP_RBRACKET = 0x30,
  KBD_KMAP_BACKSLASH = 0x31,
  KBD_KMAP_COLON = 0x33,
  KBD_KMAP_APOSTROPHE = 0x34,
  KBD_KMAP_TILDE = 0x35,
  KBD_KMAP_COMMA = 0x36,
  KBD_KMAP_DOT = 0x37,
  KBD_KMAP_SLASH = 0x38,
  KBD_KMAP_CAPS = 0x39,
  KBD_KMAP_F1 = 0x3a,
  KBD_KMAP_F2 = 0x3b,
  KBD_KMAP_F3 = 0x3c,
  KBD_KMAP_F4 = 0x3d,
  KBD_KMAP_F5 = 0x3e,
  KBD_KMAP_F6 = 0x3f,
  KBD_KMAP_F7 = 0x40,
  KBD_KMAP_F8 = 0x41,
  KBD_KMAP_F9 = 0x42,
  KBD_KMAP_F10 = 0x43,
  KBD_KMAP_F11 = 0x44,
  KBD_KMAP_F12 = 0x45,
  KBD_KMAP_PS = 0x46,
  KBD_KMAP_SL = 0x47,
  KBD_KMAP_PB = 0x48,
  KBD_KMAP_INS = 0x49,
  KBD_KMAP_HM = 0x4a,
  KBD_KMAP_PU = 0x4b,
  KBD_KMAP_DEL = 0x4c,
  KBD_KMAP_END = 0x4d,
  KBD_KMAP_PD = 0x4e,
  KBD_KMAP_RIGHT= 0x4f,
  KBD_KMAP_LEFT = 0x50,
  KBD_KMAP_DOWN = 0x51,
  KBD_KMAP_UP = 0x52,
};

enum kbd_mode_t {                    // Original name             Options
  KBD_MODE_NORMAL = 0x0d,            // Normally on               C/R
  KBD_MODE_CUSTOM = 0x1b,            // Custom settings           n/a

  KBD_MODE_STREAM = 0x08,            // Go with the stream        S, C/R, Direction
  KBD_MODE_CLOUDS = 0x09,            // Clouds fly                S, C/R, Direction
  KBD_MODE_SWIRL = 0x0a,             // Winding paths             S. C/R, Direction
  KBD_MODE_RAINBOW_BREATHING = 0x0b, // The trial of light        S
  KBD_MODE_BREATHING = 0x0c,         // Breathing                 S, C/R
  KBD_MODE_HOTMAP = 0x0e,            // Pass without trace        S, C/R
  KBD_MODE_RIPPLE = 0x0f,            // Ripple graff              S, C/R
  KBD_MODE_RIPPLE_LINES = 0x10,      // Fast run without trace    S, C/R
  KBD_MODE_SNOW = 0x11,              // Snow winter jasmine       S
  KBD_MODE_RAINBOW_DOTS = 0x12,      // Flowers blooming          S
  KBD_MODE_RAINBOW_LINES = 0x13,     // Swift action              S, Direction
  KBD_MODE_TRAINGULAR_WAVES = 0x14,  // Hurricane                 S, C/R
  KBD_MODE_DRAIN = 0x15,             // Accumulate                S, C/R
  KBD_MODE_MATRIX = 0x16,            // Digital times             S. C/R
  KBD_MODE_SCANLINE = 0x17,          // Both ways                 C/R
  KBD_MODE_GRADIENT = 0x18,          // Surmount                  Predefined gradient
  KBD_MODE_RAINBOW_CIRCLES = 0x19,   // Fast and the Furious      Inward/Outward
};

enum kbd_dir_t {
  DIR1 = 0x00,
  DIR2 = 0x01,
};

const struct kbd_key_t keys[88];
const size_t keys_sz;

bool kbd_open(libusb_context **ctx, libusb_device_handle **kbdh);
bool kbd_close(libusb_context *ctx, libusb_device_handle *kbdh);
bool kbd_send(libusb_device_handle *kbdh, uint8_t msg[64]);
bool kbd_recv(
  libusb_device_handle *kbdh, size_t sz, uint8_t *buf, unsigned int timeout);
bool kbd_send_and_recv(libusb_device_handle *kbdh, uint8_t msg[64]);
bool kbd_va_send_and_recv(libusb_device_handle *kbdh, size_t len, ...);

bool kbd_send_start(libusb_device_handle *kbdh);
bool kbd_send_end(libusb_device_handle *kbdh);
void kbd_set_key_color(libusb_device_handle *kbdh, uint32_t key, uint32_t color);
void kbd_set_color(libusb_device_handle *kbdh, uint8_t r, uint8_t g, uint8_t b);
void kbd_set_brightness(libusb_device_handle *kbdh, uint8_t level);
void kbd_set_speed(libusb_device_handle *kbdh, uint8_t level);
void kbd_set_report_rate(libusb_device_handle *kbdh, enum kbd_rate_t rate);
void kbd_set_direction(libusb_device_handle *kbdh, enum kbd_dir_t dir);
void kbd_set_rainbow(libusb_device_handle *kbdh, bool rainbow);
void kbd_set_mode(libusb_device_handle *kbdh, enum kbd_mode_t mode);
void kbd_print_modes(void);
void kbd_read_keymap_file(const char *fname, struct kbd_keymap_t *kmap);
void kbd_remap(libusb_device_handle *kbdh, struct kbd_keymap_t *kmap);
enum kbd_mode_t kbd_get_mode(const char *modestr);
const struct kbd_key_t *kbd_get_key(const char *name);


#endif
