/* Daemon which listens on a UNIX socket for commands and sends them to the
   keyboard. */

#ifndef VLC_DAEMON_H
#define VLC_DAEMON_H

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>

#include <libusb.h>

#include "config.h"
#include "fmt.h"
#include "keyboard.h"
#include "util.h"
#include "version.h"


enum vlc_status_t {
  VLC_OK = 0,
  VLC_ERR_KH_NULL,
  VLC_ERR_CMD_NULL,
  VLC_ERR_CMD_UNKNOWN,
  VLC_ERR_CMD_ARGS_MISSING,
  VLC_ERR_CMD_ARGS_RANGE,
  VLC_ERR_MODE_UNKNOWN,
  VLC_ERR_KBD_CLAIM,
  VLC_ERR_KBD_RELEASE,
  VLC_ERR_KBD_KMAP,
  VLC_ERR_KBD_MODE,
  VLC_ERR_KBD_BRIGHTNESS,
  VLC_ERR_KBD_DIR,
  VLC_ERR_KBD_RAINBOW,
  VLC_ERR_KBD_RATE,
  VLC_ERR_KBD_COLOR,
  VLC_ERR_KBD_KCOLOR,
  VLC_ERR_KBD_SPEED,
  VLC_ERR_KMAP_READ,
  VLC_ERR_KEY_UNKNOWN
};


// Forks to a control panel server if needed
int main(int argc, const char *argv[]);

// Real main routine
int vlc_daemon_main(int argc, const char *argv[]);

// Detaches kernel driver and claim the certain USB interface
// Returns `true` on success, `false` otherwise
bool vlc_kbd_claim(void);

// Releases the control of the keyboard back to the system
// Returns `true` on success, `false` otherwise
bool vlc_kbd_release(void);


// Creates and returns a UNIX socket for listening for the commands
int vlc_daemon_create_socket(void);

// Handles the USB hotplug event
int vlc_hotplug_handler(
  struct libusb_context *ctx,
  struct libusb_device *dev,
  libusb_hotplug_event event,
  void *user_data);

// Parses the incoming command and acts accordingly
enum vlc_status_t vlc_daemon_parse_command(char *cmdbuf);

// Returns a short explanation of a status code
const char *vlc_status_str(enum vlc_status_t status);


#endif
