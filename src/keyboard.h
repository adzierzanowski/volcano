#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <libusb.h>

#include "fmt.h"

// USB VID and PID values of the Volcano keyboard controller
#define VLC_KBD_VID 0x0c45
#define VLC_KBD_PID 0x0820

// USB control and bulk transfer constants used for communication via libusb
#define VLC_CTL_REQUEST_TYPE 0x21
#define VLC_CTL_REQUEST 0x09
#define VLC_CTL_REQUEST_VALUE 0x0204
#define VLC_CTL_INTERFACE 0x01
#define VLC_CTL_ENDPOINT 0x82


// Structure containing a key name and its code used for color mapping
struct vlc_kbd_key_t {
  const char *name;
  uint32_t val;
};


// An array of the key codes used for setting a single key color
const struct vlc_kbd_key_t vlc_kcolor_keys[111];
const size_t vlc_keys_sz;

// Keymapping struct
// Mapping the keys is performed all at once, thus such a big struct
struct vlc_kbd_keymap_t {
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

  uint8_t a_mk, b_mk, c_mk, d_mk, e_mk, f_mk, g_mk, h_mk, i_mk, j_mk, k_mk, l_mk, m_mk, n_mk;
  uint8_t o_mk, p_mk, q_mk, r_mk, s_mk, t_mk, u_mk, v_mk, w_mk, x_mk, y_mk, z_mk;
  uint8_t k1_mk, k2_mk, k3_mk, k4_mk, k5_mk, k6_mk, k7_mk, k8_mk, k9_mk, k0_mk;
  uint8_t enter_mk, esc_mk, backspace_mk, tab_mk, space_mk, minus_mk, plus_mk;
  uint8_t lbracket_mk, rbracket_mk, backslash_mk, colon_mk, ap_mk, tilde_mk;
  uint8_t comma_mk, dot_mk, slash_mk, caps_mk;
  uint8_t f1_mk, f2_mk, f3_mk, f4_mk, f5_mk, f6_mk, f7_mk, f8_mk, f9_mk, f10_mk, f11_mk;
  uint8_t f12_mk, ps_mk, sl_mk, pb_mk, ins_mk, hm_mk, pu_mk, del_mk, end_mk, pd_mk;
  uint8_t right_mk, left_mk, down_mk, up_mk, lshift_mk, rshift_mk, lalt_mk, ralt_mk;
  uint8_t lctrl_mk, rctrl_mk, meta_mk, menu_mk;
};

// USB report rate
enum vlc_kbd_rate_t {
  VLC_KBD_RATE_125HZ = 0x00,
  VLC_KBD_RATE_250HZ = 0x01,
  VLC_KBD_RATE_500HZ = 0x02,
  VLC_KBD_RATE_1000HZ = 0x03,
};

// Keymap codes (seems similar to USB keycodes)
enum vlc_kbd_keycode_t {
  VLC_KBD_KMAP_MODE_MODAL = 0x01,
  VLC_KBD_KMAP_MODE_NORMAL = 0x02,

  VLC_KBD_KMAP_LSHIFT = 0x02,
  VLC_KBD_KMAP_RSHIFT = 0x20,
  VLC_KBD_KMAP_LALT = 0x04,
  VLC_KBD_KMAP_RALT = 0x40,
  VLC_KBD_KMAP_LCTRL = 0x01,
  VLC_KBD_KMAP_RCTRL = 0x10,
  VLC_KBD_KMAP_META = 0x08,
  VLC_KBD_KMAP_MENU = 0x65,

  VLC_KBD_KMAP_A = 0x04,
  VLC_KBD_KMAP_B = 0x05,
  VLC_KBD_KMAP_C = 0x06,
  VLC_KBD_KMAP_D = 0x07,
  VLC_KBD_KMAP_E = 0x08,
  VLC_KBD_KMAP_F = 0x09,
  VLC_KBD_KMAP_G = 0x0a,
  VLC_KBD_KMAP_H = 0x0b,
  VLC_KBD_KMAP_I = 0x0c,
  VLC_KBD_KMAP_J = 0x0d,
  VLC_KBD_KMAP_K = 0x0e,
  VLC_KBD_KMAP_L = 0x0f,
  VLC_KBD_KMAP_M = 0x10,
  VLC_KBD_KMAP_N = 0x11,
  VLC_KBD_KMAP_O = 0x12,
  VLC_KBD_KMAP_P = 0x13,
  VLC_KBD_KMAP_Q = 0x14,
  VLC_KBD_KMAP_R = 0x15,
  VLC_KBD_KMAP_S = 0x16,
  VLC_KBD_KMAP_T = 0x17,
  VLC_KBD_KMAP_U = 0x18,
  VLC_KBD_KMAP_V = 0x19,
  VLC_KBD_KMAP_W = 0x1a,
  VLC_KBD_KMAP_X = 0x1b,
  VLC_KBD_KMAP_Y = 0x1c,
  VLC_KBD_KMAP_Z = 0x1d,
  VLC_KBD_KMAP_1 = 0x1e,
  VLC_KBD_KMAP_2 = 0x1f,
  VLC_KBD_KMAP_3 = 0x20,
  VLC_KBD_KMAP_4 = 0x21,
  VLC_KBD_KMAP_5 = 0x22,
  VLC_KBD_KMAP_6 = 0x23,
  VLC_KBD_KMAP_7 = 0x24,
  VLC_KBD_KMAP_8 = 0x25,
  VLC_KBD_KMAP_9 = 0x26,
  VLC_KBD_KMAP_0 = 0x27,
  VLC_KBD_KMAP_ENTER = 0x28,
  VLC_KBD_KMAP_ESC = 0x29,
  VLC_KBD_KMAP_BACKSPACE = 0x2a,
  VLC_KBD_KMAP_TAB = 0x2b,
  VLC_KBD_KMAP_SPACE = 0x2c,
  VLC_KBD_KMAP_MINUS = 0x2d,
  VLC_KBD_KMAP_PLUS = 0x2e,
  VLC_KBD_KMAP_LBRACKET = 0x2f,
  VLC_KBD_KMAP_RBRACKET = 0x30,
  VLC_KBD_KMAP_BACKSLASH = 0x31,
  VLC_KBD_KMAP_COLON = 0x33,
  VLC_KBD_KMAP_APOSTROPHE = 0x34,
  VLC_KBD_KMAP_TILDE = 0x35,
  VLC_KBD_KMAP_COMMA = 0x36,
  VLC_KBD_KMAP_DOT = 0x37,
  VLC_KBD_KMAP_SLASH = 0x38,
  VLC_KBD_KMAP_CAPS = 0x39,
  VLC_KBD_KMAP_F1 = 0x3a,
  VLC_KBD_KMAP_F2 = 0x3b,
  VLC_KBD_KMAP_F3 = 0x3c,
  VLC_KBD_KMAP_F4 = 0x3d,
  VLC_KBD_KMAP_F5 = 0x3e,
  VLC_KBD_KMAP_F6 = 0x3f,
  VLC_KBD_KMAP_F7 = 0x40,
  VLC_KBD_KMAP_F8 = 0x41,
  VLC_KBD_KMAP_F9 = 0x42,
  VLC_KBD_KMAP_F10 = 0x43,
  VLC_KBD_KMAP_F11 = 0x44,
  VLC_KBD_KMAP_F12 = 0x45,
  VLC_KBD_KMAP_PS = 0x46,
  VLC_KBD_KMAP_SL = 0x47,
  VLC_KBD_KMAP_PB = 0x48,
  VLC_KBD_KMAP_INS = 0x49,
  VLC_KBD_KMAP_HM = 0x4a,
  VLC_KBD_KMAP_PU = 0x4b,
  VLC_KBD_KMAP_DEL = 0x4c,
  VLC_KBD_KMAP_END = 0x4d,
  VLC_KBD_KMAP_PD = 0x4e,
  VLC_KBD_KMAP_RIGHT= 0x4f,
  VLC_KBD_KMAP_LEFT = 0x50,
  VLC_KBD_KMAP_DOWN = 0x51,
  VLC_KBD_KMAP_UP = 0x52,
};


// Color mode codes
enum vlc_kbd_mode_t {                    // Original name             Options
  VLC_KBD_MODE_NORMAL = 0x0d,            // Normally on               C/R
  VLC_KBD_MODE_CUSTOM = 0x1b,            // Custom settings           n/a

  VLC_KBD_MODE_STREAM = 0x08,            // Go with the stream        S, C/R, Direction
  VLC_KBD_MODE_CLOUDS = 0x09,            // Clouds fly                S, C/R, Direction
  VLC_KBD_MODE_SWIRL = 0x0a,             // Winding paths             S. C/R, Direction
  VLC_KBD_MODE_RAINBOW_BREATHING = 0x0b, // The trial of light        S
  VLC_KBD_MODE_BREATHING = 0x0c,         // Breathing                 S, C/R
  VLC_KBD_MODE_HOTMAP = 0x0e,            // Pass without trace        S, C/R
  VLC_KBD_MODE_RIPPLE = 0x0f,            // Ripple graff              S, C/R
  VLC_KBD_MODE_RIPPLE_LINES = 0x10,      // Fast run without trace    S, C/R
  VLC_KBD_MODE_SNOW = 0x11,              // Snow winter jasmine       S
  VLC_KBD_MODE_RAINBOW_DOTS = 0x12,      // Flowers blooming          S
  VLC_KBD_MODE_RAINBOW_LINES = 0x13,     // Swift action              S, Direction
  VLC_KBD_MODE_TRAINGULAR_WAVES = 0x14,  // Hurricane                 S, C/R
  VLC_KBD_MODE_DRAIN = 0x15,             // Accumulate                S, C/R
  VLC_KBD_MODE_MATRIX = 0x16,            // Digital times             S. C/R
  VLC_KBD_MODE_SCANLINE = 0x17,          // Both ways                 C/R
  VLC_KBD_MODE_GRADIENT = 0x18,          // Surmount                  Predefined gradient
  VLC_KBD_MODE_RAINBOW_CIRCLES = 0x19,   // Fast and the Furious      Inward/Outward

  VLC_KBD_MODE_UNKNOWN = 0xff
};

// Animation direction
enum vlc_kbd_dir_t {
  VLC_DIR1 = 0x00,
  VLC_DIR2 = 0x01,
};

enum vlc_kbd_gradient_t {
  VLC_GRADIENT_RED = 0x00,
  VLC_GRADIENT_ORANGE = 0x01,
  VLC_GRADIENT_GREEN = 0x02,
  VLC_GRADIENT_BLUE = 0x03,
};


// Functions that return `bool` return `true` if successful, `false` otherwise
// This is different from libusb functions that reside underneath as they
// return `0` if successful and error code if not

// Initializes libusb context and opens the keyboard device
// Results are assigned to `ctx` and `kbdh`
bool vlc_kbd_open(libusb_context **ctx, libusb_device_handle **kbdh);

// Closes the keyboard device handle and deinitializes libusb
bool vlc_kbd_close(libusb_context *ctx, libusb_device_handle *kbdh);


// Sends a 64-byte data frame via USB control transfer to the keyboard
bool vlc_kbd_send(libusb_device_handle *kbdh, uint8_t msg[64]);

// Receives a response from the keyboard via USB bulk transfer
// Stores the `sz` bytes of the response in `buf`
bool vlc_kbd_recv(
  libusb_device_handle *kbdh, size_t sz, uint8_t *buf, unsigned int timeout);

// Sends a 64-byte data fram to the keyboard then awaits a response
// and validates it
bool vlc_kbd_send_and_recv(libusb_device_handle *kbdh, uint8_t msg[64]);

// Variadic version of the `vlc_kbd_send_and_recv`
// The message is composed by taking `len` arguments and filling the rest
// of the 64 bytes with zeros
bool vlc_kbd_va_send_and_recv(libusb_device_handle *kbdh, size_t len, ...);


// Sends the initial data frame which is common for every command (each command
// consists of the start frame, set of messages and the end frame)
bool vlc_kbd_send_start(libusb_device_handle *kbdh);

// Sends the end data frame to the keyboard
bool vlc_kbd_send_end(libusb_device_handle *kbdh);

// Remaps the keys as defined in the `kmap` structure
bool vlc_kbd_remap(libusb_device_handle *kbdh, struct vlc_kbd_keymap_t *kmap);

// Sets the color of a single `key` (specified by code defined in `vlc_kcolor_keys`)
// The keyboard has to be set to the custom mode first
// `color` is the usual hexadecimal representation of an RGB value
bool vlc_kbd_set_key_color(libusb_device_handle *kbdh, uint32_t key, uint32_t color);

// Sets the color of the entire keyboard (works with most of the color modes)
bool vlc_kbd_set_color(libusb_device_handle *kbdh, uint8_t r, uint8_t g, uint8_t b);

// Sets the keyboard's brightness level (0-4)
bool vlc_kbd_set_brightness(libusb_device_handle *kbdh, uint8_t level);

// Sets the speed of the animation (0-4) for some of the color modes
bool vlc_kbd_set_speed(libusb_device_handle *kbdh, uint8_t level);

// Sets the USB report rate (125, 250, 500, or 1000 Hz - see `vlc_kbd_rate_t`)
bool vlc_kbd_set_report_rate(libusb_device_handle *kbdh, enum vlc_kbd_rate_t rate);

// Sets the direction of the animation for some of the color modes
// `dir` is a value in range 0-1
bool vlc_kbd_set_direction(libusb_device_handle *kbdh, enum vlc_kbd_dir_t dir);

// Sets the state of the rainbow mode for some of the color modes
bool vlc_kbd_set_rainbow(libusb_device_handle *kbdh, bool rainbow);

// Sets the color mode of the keyboard (see `vlc_kbd_mode_t` for available values)
bool vlc_kbd_set_mode(libusb_device_handle *kbdh, enum vlc_kbd_mode_t mode);

// Sets the gradient color in the gradient color mode
bool vlc_kbd_set_gradient(libusb_device_handle *kbdh, enum vlc_kbd_gradient_t gradient);


// Prints out all of the available color modes
void vlc_kbd_print_modes(void);

// Reads a binary keymap file and stores the result in the `kmap` struct
bool vlc_kbd_read_keymap_file(const char *fname, struct vlc_kbd_keymap_t *kmap);

// Returns a color mode code based on a string containing its name
enum vlc_kbd_mode_t vlc_kbd_get_mode(const char *modestr);

// Returns a key based on its name (some keys can have more than one name)
const struct vlc_kbd_key_t *vlc_kbd_get_key(const char *name);

// Struct representing a single entry in a macro definition
struct vlc_kbd_macro_entry_t {
  enum vlc_kbd_keycode_t keycode;
  uint16_t delay;
  bool down;
  bool modifier;
};

// Struct representing a macro definition
struct vlc_kbd_macro_t {
  size_t entries_length;
  size_t name_length;
  struct vlc_kbd_macro_entry_t **entries;
  char *name;
  uint8_t cycle_count;
};

// Creates a new macro definition
struct vlc_kbd_macro_t *vlc_kbd_macro_new(const char *name, uint8_t cycle_count);

// Frees the memory reserved for a `macro`
void vlc_kbd_macro_free(struct vlc_kbd_macro_t *macro);

// Adds an entry (i.e. definition of a key press) into the `macro` struct
void vlc_kbd_macro_add_entry(
  struct vlc_kbd_macro_t *macro,
  enum vlc_kbd_keycode_t kcode,
  bool is_modifier,
  uint16_t delay,
  bool down);

// Copies the macros defined in the `mkrs` struct into a `dst` buffer in a
// continuous payload form (which has to be later divided into separate packets
// before sending it to the keyboard)
void vlc_kbd_macro_payload(
  uint8_t *dst, struct vlc_kbd_macro_t **mkrs, uint16_t mkrcnt);

// Creates separate packets ready to be sent from the payload data
uint8_t **vlc_kbd_macro_packets(size_t *pck_cnt, uint8_t *payload);

#endif
