/* Daemon which listens on a UNIX socket for commands and sends them to the
   keyboard. */

#ifndef DAEMON_H
#define DAEMON_H

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/un.h>
#include <unistd.h>

#include <libusb.h>

#include "fmt.h"
#include "keyboard.h"


struct config_t {
  char *kmap_file;
  char *socket_file;
  char *init_mode;
  char *srv_data;
  char *srv_exe;
  int socket_uid;
  int socket_gid;
  uint32_t init_color;
  uint16_t srv_port;
  bool srv_enable;
} config;

enum status_t {
  OK = 0,
  ERR_KH_NULL,
  ERR_CMD_NULL,
  ERR_CMD_UNKNOWN,
  ERR_CMD_ARGS_MISSING,
  ERR_CMD_ARGS_RANGE,
  ERR_MODE_UNKNOWN,
  ERR_KBD_CLAIM,
  ERR_KBD_RELEASE,
  ERR_KBD_KMAP,
  ERR_KBD_MODE,
  ERR_KBD_BRIGHTNESS,
  ERR_KBD_DIR,
  ERR_KBD_RAINBOW,
  ERR_KBD_RATE,
  ERR_KBD_COLOR,
  ERR_KBD_KCOLOR,
  ERR_KBD_SPEED,
  ERR_KMAP_READ,
  ERR_KEY_UNKNOWN
};


// Forks to a control panel server if needed
int main(int argc, const char *argv[]);
// Real main routine
int daemon_main(int argc, const char *argv[]);

// Detaches kernel driver and claim the certain USB interface
// Returns `true` on success, `false` otherwise
bool kbd_claim(void);
// Releases the control of the keyboard back to the system
// Returns `true` on success, `false` otherwise
bool kbd_release(void);

// Creates and returns a UNIX socket for listening for the commands
int create_socket(void);
// Handles USB hotplug
int hotplug_handler(
  struct libusb_context *ctx,
  struct libusb_device *dev,
  libusb_hotplug_event event,
  void *user_data);
// Initializes the configuration based on a config file name
void init_config(const char *rcfname);
// Parses an incoming command and acts accordingly
// Returns status of the action (`OK` on success, other status otherwise)
enum status_t parse_command(char *cmdbuf);
// Returns a short explanation of a status code
const char *status_str(enum status_t status);

#endif
