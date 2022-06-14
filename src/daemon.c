#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>

#include "fmt.h"
#include "keyboard.h"

bool kbd_hotplugged = false;

libusb_context *ctx = NULL;
libusb_device_handle *kbdh = NULL;

struct sockaddr_un addr;
struct timeval timeout = { .tv_usec = 100000 };

struct config_t {
  char *kmap_file;
  char *socket_file;
  char *init_mode;
  uint32_t init_color;
} config;


void init_config(const char *rcfname) {
  dlog(INFO, "Initializing config\n");
  dlog(DEBUG, "Config file path: %s\n", rcfname);

  FILE *f = fopen(rcfname, "r");
  if (!f) {
    dlog(ERROR, "Config file not found (%s).\n", rcfname);
    exit(1);
    return;
  }

  char buf[10240] = {0};
  fread(buf, sizeof (char), 10240, f);

  char *tok = strtok(buf, "=");

  while (tok != NULL) {
    char *key = tok;
    tok = strtok(NULL, "\n");

    if (strcmp(key, "SOCKET_FILE") == 0) {
      config.socket_file = calloc(strlen(tok) + 1, sizeof (char));
      sprintf(config.socket_file, "%s", tok);

    } else if (strcmp(key, "KMAP_FILE") == 0) {
      config.kmap_file = calloc(strlen(tok) + 1, sizeof (char));
      sprintf(config.kmap_file, "%s", tok);

    } else if (strcmp(key, "INIT_MODE") == 0) {
      config.init_mode = calloc(strlen(tok) + 1, sizeof (char));
      sprintf(config.init_mode, "%s", tok);

    } else if (strcmp(key, "INIT_COLOR") == 0) {
      config.init_color = strtol(tok, NULL, 16);

    } else if (strcmp(key, "LOGLEVEL") == 0) {
      set_loglevel(atoi(tok));

    }

    tok = strtok(NULL, "=");
  }

  fclose(f);

  dlog(DEBUG, "socket file: %s\n", config.socket_file);
  dlog(DEBUG, "kmap file:   %s\n", config.kmap_file);
  dlog(DEBUG, "init mode:   %s\n", config.init_mode);
  dlog(DEBUG, "init color:  #%06x\n", config.init_color);
}

void remap(const char *fname) {
  dlog(INFO, "Remapping from file %s\n", fname);
  struct kbd_keymap_t kmap;
  kbd_read_keymap_file(fname, &kmap);
  kbd_remap(kbdh, &kmap);
  dlog(DEBUG, "Remapping complete\n");
}

int hotplug_handler(
    struct libusb_context *ctx,
    struct libusb_device *dev,
    libusb_hotplug_event event,
    void *user_data) {
  if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
    kbdh = libusb_open_device_with_vid_pid(ctx, KBD_VID, KBD_PID);
    dlog(INFO, "Keyboard hotplugged\n");
    kbd_hotplugged = true;

  } else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
    libusb_close(kbdh);
    kbdh = NULL;
    dlog(INFO, "Keyboard unplugged\n");
  }

  return 0;
}

int create_socket() {
  struct stat statbuf;
  int res = stat(config.socket_file, &statbuf);
  if (res == 0) {
    remove(config.socket_file);
  }

  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, config.socket_file);

  int s = socket(AF_UNIX, SOCK_STREAM, 0);

  fcntl(s, F_SETFL, O_NONBLOCK);

  if (s == -1) {
    dlog(ERROR, "Could not create socket.\n");
    exit(1);
  }

  if (bind(s, (struct sockaddr *) &addr, strlen(addr.sun_path) + sizeof (addr.sun_family) + 1) == -1) {
    dlog(ERROR, "Failed to bind to the socket.\n");
    exit(1);
  }

  chmod(config.socket_file, S_IRWXG | S_IRWXO | S_IRWXU);
  chown(config.socket_file, 501, 20);

  if (listen(s, 5) == -1) {
    dlog(ERROR, "Failed to listen.\n");
    exit(1);
  }

  return s;
}

void claim() {
  libusb_detach_kernel_driver(kbdh, CTL_INTERFACE);
  libusb_claim_interface(kbdh, CTL_INTERFACE);
}

void release() {
  libusb_attach_kernel_driver(kbdh, CTL_INTERFACE);
  libusb_release_interface(kbdh, CTL_INTERFACE);
}

void parse_command(char *buf) {
  dlog(DEBUG, "Parsing command\n");
  const char *delim = " \n";
  char *tok = strtok(buf, delim);

  if (!tok) {
    dlog(WARNING, "Empty command.\n");
    return;
  }

  if (strcmp(tok, "kmap") == 0) {
    tok = strtok(NULL, delim);

    const char *kmap_file = config.kmap_file;

    if (tok != NULL) {
      kmap_file = tok;
    }

    struct kbd_keymap_t kmap;
    dlog(INFO, "Mapping keys to %s.\n", kmap_file);
    kbd_read_keymap_file(kmap_file, &kmap);
    claim();
    kbd_remap(kbdh, &kmap);
    release();
    return;

  } else if (strcmp(tok, "mode") == 0) {
    tok = strtok(NULL, delim);

    if (tok == NULL) {
      dlog(WARNING, "Mode command with no argument.\n");
      return;
    }

    enum kbd_mode_t mode = kbd_get_mode(tok);

    if (mode == KBD_MODE_UNKNOWN) {
      dlog(WARNING, "Unknown mode: %s\n", tok);
      return;
    }

    dlog(INFO, "Setting mode to %s.\n", tok);
    claim();
    kbd_set_mode(kbdh, mode);
    release();
    return;

  } else if (strcmp(tok, "color") == 0) {
    int r = 0;
    int g = 0;
    int b = 0;

    tok = strtok(NULL, delim);
    if (tok != NULL) {
      r = atoi(tok);

      tok = strtok(NULL, delim);
      if (tok != NULL) {
        g = atoi(tok);

        tok = strtok(NULL, delim);
        if (tok != NULL) {
          b = atoi(tok);
        }
      }
    }

    dlog(INFO, "Setting color to RGB(%d, %d, %d) HEX(#%02x%02x%02x).\n", r, g, b, r, g, b);
    claim();
    kbd_set_color(kbdh, r, g, b);
    release();
    return;

  } else if (strcmp(tok, "kcolor") == 0) {
    char keycode[128] = {0};
    int r = 0;
    int g = 0;
    int b = 0;

    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(WARNING, "Key color command with no KEY argument.\n");
      return;
    }

    sprintf(keycode, "%s", tok);

    tok = strtok(NULL, delim);
    if (tok) {
      r = atoi(tok);
      tok = strtok(NULL, delim);
      if (tok) {
        g = atoi(tok);
        tok = strtok(NULL, delim);
        if (tok) {
          b = atoi(tok);
        }
      }
    }

    const struct kbd_key_t *key = kbd_get_key(keycode);
    if (key == NULL) {
      dlog(WARNING, "Unknown key: %s\n", keycode);
      return;
    }

    dlog(INFO, "Setting key %s to RGB(%d, %d, %d) HEX(#%02x%02x%02x).\n");
    claim();
    kbd_set_mode(kbdh, KBD_MODE_CUSTOM);
    kbd_set_key_color(kbdh, key->val, (r << 16) | (g << 8) | b);
    release();
    return;
  } else if (strcmp(tok, "brightness") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(WARNING, "Brightness command without LEVEL argument.\n");
      return;
    }

    int level = atoi(tok);
    if (level < 0 || level > 4) {
      dlog(WARNING, "Brightness level not in range (0, 4).\n");
      return;
    }

    dlog(INFO, "Setting brightness level to %d\n", level);
    claim();
    kbd_set_brightness(kbdh, level);
    release();
    return;

  } else if (strcmp(tok, "speed") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(WARNING, "Speed command without LEVEL argument.\n");
      return;
    }

    int level = atoi(tok);
    if (level < 0 || level > 4) {
      dlog(WARNING, "Speed level not in range (0, 4).\n");
      return;
    }

    dlog(INFO, "Setting speed level to %d.\n", level);
    claim();
    kbd_set_speed(kbdh, level);
    release();
    return;

  } else if (strcmp(tok, "rate") == 0) {
    tok = strtok(NULL, delim);

    if (tok == NULL) {
      dlog(WARNING, "Report rate command without an argument.\n");
      return;
    }

    enum kbd_rate_t rate;
    if (strcmp(tok, "125") == 0) {
      rate = KBD_RATE_125HZ;
    } else if (strcmp(tok, "250") == 0) {
      rate = KBD_RATE_250HZ;
    } else if (strcmp(tok, "500") == 0) {
      rate = KBD_RATE_500HZ;
    } else if (strcmp(tok, "1000") == 0) {
      rate = KBD_RATE_1000HZ;
    } else {
      dlog(WARNING, "Report rate not available: %s.\n", tok);
      return;
    }

    dlog(INFO, "Setting report rate to %s Hz.\n", tok);
    claim();
    kbd_set_report_rate(kbdh, rate);
    release();
    return;

  } else if (strcmp(tok, "dir") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(WARNING, "Direction command without an argument.\n");
      return;
    }

    enum kbd_dir_t dir = atoi(tok);
    if (dir < 0 || dir > 1) {
      dlog(WARNING, "Direction not in range (0, 1).\n");
      return;
    }

    dlog(INFO, "Setting direction to %d.\n", dir);
    claim();
    kbd_set_direction(kbdh, dir);
    release();
    return;

  } else if (strcmp(tok, "rainbow") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(WARNING, "Rainbow command without an argument.\n");
      return;
    }

    bool rainbow;
    if (strcmp(tok, "true") == 0) {
      rainbow = true;
    } else if (strcmp(tok, "false") == 0) {
      rainbow = false;
    } else {
      dlog(WARNING, "Invalid argument for rainbow command.\n");
      return;
    }

    dlog(INFO, "Setting rainbow to %s.\n", tok);
    claim();
    kbd_set_rainbow(kbdh, rainbow);
    release();
    return;
  }

  dlog(WARNING, "Unknown command: %s\n", buf);
}

int main(int argc, char *argv[]) {
  dlog(INFO, "\nStarting volcanod\n");
  dlog(DEBUG, "argc=%d\n", argc);

  if (argc < 2) {
    dlog(ERROR, "Need a config file path as an argument.\n");
    exit(1);
  } else {
    init_config(argv[1]);
  }

  libusb_init(&ctx);
  libusb_hotplug_callback_handle cbhandle;

  dlog(DEBUG, "Attempting to register hotplug callback.\n");
  int status = libusb_hotplug_register_callback(
    ctx,
    LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
    0,
    KBD_VID,
    KBD_PID,
    LIBUSB_HOTPLUG_MATCH_ANY,
    hotplug_handler,
    NULL,
    &cbhandle);

  if (status != 0) {
    dlog(ERROR, "Failed to register hotplug callback.\n");
    exit(1);
  }

  if (!kbdh) {
    kbdh = libusb_open_device_with_vid_pid(ctx, KBD_VID, KBD_PID);
    dlog(INFO, "Initial keyboard connection: %p\n", kbdh);
    if (!kbdh) {
      dlog(ERROR, "Failed to connect to the keyboard.\n");
      exit(1);
    }
  }

  int s = create_socket();
  dlog(INFO, "Socket created: fd=%d\n", s);

  for (;;) {
    libusb_handle_events_timeout_completed(ctx, &timeout, NULL);
    socklen_t len;

    if (kbd_hotplugged && kbdh) {
      kbd_hotplugged = false;
      claim();
      dlog(INFO, "Hotplug: Remapping keys.\n");
      remap(config.kmap_file);
      dlog(INFO, "Hotplug: Setting initial mode to %s.\n", config.init_mode);
      enum kbd_mode_t mode = kbd_get_mode(config.init_mode);
      kbd_set_mode(kbdh, mode);
      dlog(INFO, "Hotplug: Setting initial color to #%06x.\n", config.init_color);
      kbd_set_color(kbdh, config.init_color >> 16, config.init_color >> 8, config.init_color);
      release();
    }

    int rsock = accept(s, (struct sockaddr *) &addr, &len);

    if (rsock != -1) {
      char buf[1024] = {0};
      ssize_t rlen = recv(rsock, buf, 1024, 0);
      dlog(DEBUG, "Received %zd bytes: %s\n", rlen, buf);

      if (!kbdh) {
        dlog(ERROR, "Keyboard handle is NULL.\n");
        continue;
      }

      parse_command(buf);
    }

    usleep(100000);
  }

  return 0;
}
