#include "daemon.h"


// A flag which is being set on hotplug and is used in the main routine
static bool kbd_hotplugged = false;

// Global libusb handles
// They're being set in the hotplug handler
// and used throughout the program
static libusb_context *ctx = NULL;
static libusb_device_handle *kbdh = NULL;

static struct sockaddr_un addr;

// Timeout for libusb polling
static struct timeval timeout = { .tv_usec = 100000 };

// Default keyboard remapper
static bool remap(const char *fname) {
  dlog(LOG_INFO, "Remapping from file %s\n", fname);
  struct kbd_keymap_t kmap;
  if (!kbd_read_keymap_file(fname, &kmap)) {
    dlog(LOG_ERROR, "Failed reading kmap file.\n");
    return false;
  }
  kbd_remap(kbdh, &kmap);
  dlog(LOG_DEBUG, "Remapping complete\n");
  return true;
}

// Sends initial configuration commands to the keyboard when hotplugged
static void on_hotplug() {
  if (kbdh) {
    kbd_claim();
    dlog(LOG_INFO, "Hotplug: Remapping keys.\n");
    remap(config.kmap_file);
    dlog(LOG_INFO, "Hotplug: Setting initial mode to %s.\n", config.init_mode);
    enum kbd_mode_t mode = kbd_get_mode(config.init_mode);
    kbd_set_mode(kbdh, mode);
    kbd_release();
    kbd_claim();
    dlog(LOG_INFO,
      "Hotplug: Setting initial color to #%06x.\n", config.init_color);
    kbd_set_color(
      kbdh, config.init_color >> 16, config.init_color >> 8, config.init_color);
    kbd_release();
  }
}


int main(int argc, const char *argv[]) {
  config.srv_port = 65226;

  dlog(LOG_INFO, "\n");
  dlog(LOG_INFO, "Starting volcanod\n");
  for (int i = 0; i < argc; i++) {
    dlog(LOG_DEBUG, "argv[%d]=%s\n", i, argv[i]);
  }

  if (argc < 2) {
    dlog(LOG_ERROR, "Need a config file path as an argument.\n");
    exit(1);
  } else {
    init_config(argv[1]);
  }

  if (config.srv_enable) {
    pid_t pid = fork();

    if (pid == 0) {
      dlog(LOG_INFO, "Creating a child server.\n");

      char srv_port[6] = {0};
      sprintf(srv_port, "%hu", config.srv_port);
      char srv_data[SMALLBUFSZ] = {0};
      sprintf(srv_data, "%s", config.srv_data);
      char srv_socket[SMALLBUFSZ] = {0};
      sprintf(srv_socket, "%s", config.socket_file);

      int err = execl(config.srv_exe, "volcanosrv", srv_port, srv_data, srv_socket, (char *)0);
      if (err) {
        dlog(LOG_ERROR, "execl failed: %s\n", strerror(errno));
        exit(1);
      }

    } else if (pid < 0) {
      dlog(LOG_ERROR, "Could not create a child process.\n");
      exit(1);

    } else {
      return daemon_main(argc, argv);
    }
  }

  return daemon_main(argc, argv);
}

int daemon_main(int argc, const char *argv[]) {
  libusb_init(&ctx);
  libusb_hotplug_callback_handle cbhandle;

  dlog(LOG_DEBUG, "Attempting to register hotplug callback.\n");
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
    dlog(LOG_ERROR, "Failed to register hotplug callback.\n");
    exit(1);
  }

  if (!kbdh) {
    kbdh = libusb_open_device_with_vid_pid(ctx, KBD_VID, KBD_PID);
    dlog(LOG_INFO, "Initial keyboard connection: %p\n", kbdh);
    if (!kbdh) {
      dlog(LOG_WARNING, "Failed to connect to the keyboard.\n");
    }
  }

  on_hotplug();

  int s = create_socket();
  dlog(LOG_INFO, "Socket created: fd=%d\n", s);

  for (;;) {
    libusb_handle_events_timeout_completed(ctx, &timeout, NULL);
    socklen_t len;

    if (kbd_hotplugged) {
      kbd_hotplugged = false;
      on_hotplug();
    }

    int rsock = accept(s, (struct sockaddr *) &addr, &len);

    if (rsock != -1) {
      char buf[SMALLBUFSZ] = {0};
      ssize_t rlen = recv(rsock, buf, SMALLBUFSZ, 0);
      dlog(LOG_DEBUG, "Received %zd bytes: %s\n", rlen, buf);

      if (!kbdh) {
        dlog(LOG_ERROR, "Keyboard handle is NULL.\n");
        send(rsock, status_str(ERR_KH_NULL), strlen(status_str(ERR_KH_NULL)), 0);
        close(rsock);
        continue;
      }

      enum status_t status = parse_command(buf);
      send(rsock, status_str(status), strlen(status_str(status)), 0);
      close(rsock);
    }

    usleep(100000);
  }

  return 0;
}

bool kbd_claim() {
  if (libusb_detach_kernel_driver(kbdh, CTL_INTERFACE)) return false;
  if (libusb_claim_interface(kbdh, CTL_INTERFACE)) return false;
  return true;
}

bool kbd_release() {
  // TODO: kbd_release status is LIBUSB_LOG_ERROR_NOT_FOUND but it seems to work ok
  if (libusb_release_interface(kbdh, CTL_INTERFACE)) return false;
  if (libusb_attach_kernel_driver(kbdh, CTL_INTERFACE)) return false;
  return true;
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
    dlog(LOG_ERROR, "Could not create socket.\n");
    exit(1);
  }

  if (bind(s, (struct sockaddr *) &addr, strlen(addr.sun_path) + sizeof (addr.sun_family) + 1) == -1) {
    dlog(LOG_ERROR, "Failed to bind to the socket.\n");
    exit(1);
  }

  dlog(LOG_INFO, "Setting permissions to the socket file.\n");
  chmod(config.socket_file, S_IRWXG | S_IRWXO | S_IRWXU);
  chown(config.socket_file, config.socket_uid, config.socket_gid);

  if (listen(s, 5) == -1) {
    dlog(LOG_ERROR, "Failed to listen.\n");
    exit(1);
  }

  return s;
}

int hotplug_handler(
    struct libusb_context *ctx,
    struct libusb_device *dev,
    libusb_hotplug_event event,
    void *user_data) {

  if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
    kbdh = libusb_open_device_with_vid_pid(ctx, KBD_VID, KBD_PID);
    dlog(LOG_INFO, "Keyboard hotplugged\n");
    kbd_hotplugged = true;

  } else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
    libusb_close(kbdh);
    kbdh = NULL;
    dlog(LOG_INFO, "Keyboard unplugged\n");
  }

  return 0;
}

void init_config(const char *rcfname) {
  dlog(LOG_INFO, "Initializing config\n");
  dlog(LOG_DEBUG, "Config file path: %s\n", rcfname);

  FILE *f = fopen(rcfname, "r");
  if (!f) {
    dlog(LOG_ERROR, "Config file not found (%s).\n", rcfname);
    exit(1);
    return;
  }

  char buf[BUFSZ] = {0};
  fread(buf, sizeof (char), BUFSZ, f);

  char *tok = strtok(buf, "=");

  while (tok != NULL) {
    char *key = tok;
    tok = strtok(NULL, "\n");

    if (strcmp(key, "SOCKET_FILE") == 0) {
      config.socket_file = calloc(strlen(tok) + 1, sizeof (char));
      sprintf(config.socket_file, "%s", tok);

    } else if (strcmp(key, "SOCKET_UID") == 0) {
      config.socket_uid = atoi(tok);

    } else if (strcmp(key, "SOCKET_GID") == 0) {
      config.socket_gid = atoi(tok);

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

    } else if (strcmp(key, "SRV_ENABLE") == 0) {
      config.srv_enable = atoi(tok);

    } else if (strcmp(key, "SRV_PORT") == 0) {
      config.srv_port = atoi(tok);

    } else if (strcmp(key, "SRV_DATA") == 0) {
      config.srv_data = calloc(strlen(tok) + 1, sizeof (char));
      sprintf(config.srv_data, "%s", tok);

    } else if (strcmp(key, "SRV_EXE") == 0) {
      config.srv_exe = calloc(strlen(tok) + 1, sizeof (char));
      sprintf(config.srv_exe, "%s", tok);

    } else {
      dlog(LOG_WARNING, "Unknown config key: %s\n", key);

    }

    tok = strtok(NULL, "=");
  }

  fclose(f);

  dlog(LOG_DEBUG, "socket file: %s\n", config.socket_file);
  dlog(LOG_DEBUG, "kmap file:   %s\n", config.kmap_file);
  dlog(LOG_DEBUG, "init mode:   %s\n", config.init_mode);
  dlog(LOG_DEBUG, "init color:  #%06x\n", config.init_color);
  dlog(LOG_DEBUG, "srv enable:  %s\n", config.srv_enable ? "true" : "false");
  dlog(LOG_DEBUG, "srv port:    %hu\n", config.srv_port);
  dlog(LOG_DEBUG, "srv data:    %s\n", config.srv_data);
}

enum status_t parse_command(char *cmdbuf) {
  dlog(LOG_DEBUG, "Parsing command\n");
  const char *delim = " \n";
  char *tok = strtok(cmdbuf, delim);

  if (!tok) {
    dlog(LOG_WARNING, "Empty command.\n");
    return ERR_CMD_NULL;
  }

  if (strcmp(tok, "quit") == 0) {
    dlog(LOG_INFO, "Quitting.\n");
    exit(0);

  } else if (strcmp(tok, "fkmap") == 0) {
    tok = strtok(NULL, delim);

    const char *kmap_file = config.kmap_file;

    if (tok != NULL) {
      kmap_file = tok;
    }

    struct kbd_keymap_t kmap;
    dlog(LOG_INFO, "Mapping keys to %s.\n", kmap_file);
    if (!kbd_read_keymap_file(kmap_file, &kmap)) {
      return ERR_KMAP_READ;
    }
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_remap(kbdh, &kmap)) {
      kbd_release();
      return ERR_KBD_KMAP;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "kmap") == 0) {
    uint8_t data[172] = {0};
    int dataptr = 0;

    tok = strtok(NULL, delim);
    while (tok != NULL) {
      data[dataptr++] = atoi(tok);
      tok = strtok(NULL, delim);
    }

    struct kbd_keymap_t kmap;
    memcpy(&kmap, data, 172);

    dlog(LOG_INFO, "Mapping keys from direct data.\n");
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_remap(kbdh, &kmap)) {
      kbd_release();
      return ERR_KBD_KMAP;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;

    return OK;

  } else if (strcmp(tok, "mode") == 0) {
    tok = strtok(NULL, delim);

    if (tok == NULL) {
      dlog(LOG_WARNING, "Mode command with no argument.\n");
      return ERR_CMD_ARGS_MISSING;
    }

    enum kbd_mode_t mode = kbd_get_mode(tok);

    if (mode == KBD_MODE_UNKNOWN) {
      dlog(LOG_WARNING, "Unknown mode: %s\n", tok);
      return ERR_MODE_UNKNOWN;
    }

    dlog(LOG_INFO, "Setting mode to %s.\n", tok);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_mode(kbdh, mode)) {
      kbd_release();
      return ERR_KBD_MODE;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "color") == 0) {
    int r = 0;
    int g = 0;
    int b = 0;

    tok = strtok(NULL, delim);
    if (tok != NULL) {
      r = atoi(tok);
      if (r < 0 || r > 255) {
        dlog(LOG_ERROR, "Color not in range 0-255.\n");
        return ERR_CMD_ARGS_RANGE;
      }

      tok = strtok(NULL, delim);
      if (tok != NULL) {
        g = atoi(tok);
        if (g < 0 || g > 255) {
          dlog(LOG_ERROR, "Color not in range 0-255.\n");
          return ERR_CMD_ARGS_RANGE;
        }

        tok = strtok(NULL, delim);
        if (tok != NULL) {
          b = atoi(tok);
          if (b < 0 || b > 255) {
            dlog(LOG_ERROR, "Color not in range 0-255.\n");
            return ERR_CMD_ARGS_RANGE;
          }
        }
      }
    }

    dlog(LOG_INFO, "Setting color to RGB(%d, %d, %d) HEX(#%02x%02x%02x).\n", r, g, b, r, g, b);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_color(kbdh, r, g, b)) {
      kbd_release();
      return ERR_KBD_COLOR;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "kcolor") == 0) {
    char keycode[128] = {0};
    int r = 0;
    int g = 0;
    int b = 0;

    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(LOG_WARNING, "Key color command with no KEY argument.\n");
      return ERR_CMD_ARGS_MISSING;
    }

    sprintf(keycode, "%s", tok);
    dlog(LOG_DEBUG, "kcolor keycode=%s\n", keycode);

    tok = strtok(NULL, delim);
    if (tok) {
      r = atoi(tok);
      if (r < 0 || r > 255) {
        dlog(LOG_ERROR, "Key color not in range 0-255.\n");
        return ERR_CMD_ARGS_RANGE;
      }

      tok = strtok(NULL, delim);
      if (tok) {
        g = atoi(tok);
        if (g < 0 || g > 255) {
          dlog(LOG_ERROR, "Key color not in range 0-255.\n");
          return ERR_CMD_ARGS_RANGE;
        }

        tok = strtok(NULL, delim);
        if (tok) {
          b = atoi(tok);
          if (b < 0 || b > 255) {
            dlog(LOG_ERROR, "Key color not in range 0-255.\n");
            return ERR_CMD_ARGS_RANGE;
          }
        }
      }
    }
    dlog(LOG_DEBUG, "COLORS PARSED: %d %d %d\n", r, g, b);

    const struct kbd_key_t *key = kbd_get_key(keycode);
    if (key == NULL) {
      dlog(LOG_WARNING, "Unknown key: %s\n", keycode);
      return ERR_KEY_UNKNOWN;
    }

    dlog(LOG_DEBUG, "KEY STRUCT=%p\n", key);

    dlog(
      LOG_INFO,
      "Setting key %s to RGB(%d, %d, %d) HEX(#%02x%02x%02x).\n",
      keycode, r, g, b, r, g, b);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_mode(kbdh, KBD_MODE_CUSTOM)) {
      kbd_release();
      return ERR_KBD_MODE;
    }
    uint32_t color = (r << 16) | (g << 8) | b;
    dlog(LOG_DEBUG, "color=%06x\n", color);
    if (!kbd_set_key_color(kbdh, key->val, color)) {
      kbd_release();
      return ERR_KBD_KCOLOR;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "brightness") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(LOG_WARNING, "Brightness command without LEVEL argument.\n");
      return ERR_CMD_ARGS_RANGE;
    }

    int level = atoi(tok);
    if (level < 0 || level > 4) {
      dlog(LOG_WARNING, "Brightness level not in range (0, 4).\n");
      return ERR_CMD_ARGS_RANGE;
    }

    dlog(LOG_INFO, "Setting brightness level to %d\n", level);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_brightness(kbdh, level)) {
      kbd_release();
      return ERR_KBD_BRIGHTNESS;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "speed") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(LOG_WARNING, "Speed command without LEVEL argument.\n");
      return ERR_CMD_ARGS_MISSING;
    }

    int level = atoi(tok);
    if (level < 0 || level > 4) {
      dlog(LOG_WARNING, "Speed level not in range (0, 4).\n");
      return ERR_CMD_ARGS_RANGE;
    }

    dlog(LOG_INFO, "Setting speed level to %d.\n", level);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_speed(kbdh, level)) {
      kbd_release();
      return ERR_KBD_SPEED;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "rate") == 0) {
    tok = strtok(NULL, delim);

    if (tok == NULL) {
      dlog(LOG_WARNING, "Report rate command without an argument.\n");
      return ERR_CMD_ARGS_MISSING;
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
      dlog(LOG_WARNING, "Report rate not available: %s.\n", tok);
      return ERR_CMD_ARGS_RANGE;
    }

    dlog(LOG_INFO, "Setting report rate to %s Hz.\n", tok);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_report_rate(kbdh, rate)) {
      kbd_release();
      return ERR_KBD_RATE;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "dir") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(LOG_WARNING, "Direction command without an argument.\n");
      return ERR_CMD_ARGS_MISSING;
    }

    // TODO: setting direction fails
    enum kbd_dir_t dir = atoi(tok);
    if (dir < 0 || dir > 1) {
      dlog(LOG_WARNING, "Direction not in range (0, 1).\n");
      return ERR_CMD_ARGS_RANGE;
    }

    dlog(LOG_INFO, "Setting direction to %d.\n", dir);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_direction(kbdh, dir)) {
      kbd_release();
      return ERR_KBD_DIR;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;

  } else if (strcmp(tok, "rainbow") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      dlog(LOG_WARNING, "Rainbow command without an argument.\n");
      return ERR_CMD_ARGS_MISSING;
    }

    bool rainbow;
    if (strcmp(tok, "true") == 0) {
      rainbow = true;
    } else if (strcmp(tok, "false") == 0) {
      rainbow = false;
    } else {
      dlog(LOG_WARNING, "Invalid argument for rainbow command.\n");
      return ERR_CMD_ARGS_RANGE;
    }

    dlog(LOG_INFO, "Setting rainbow to %s.\n", tok);
    if (!kbd_claim()) return ERR_KBD_CLAIM;
    if (!kbd_set_rainbow(kbdh, rainbow)) {
      kbd_release();
      return ERR_KBD_RAINBOW;
    }
    if (!kbd_release()) return ERR_KBD_RELEASE;
    return OK;
  }

  dlog(LOG_WARNING, "Unknown command: %s\n", cmdbuf);
  return ERR_CMD_UNKNOWN;
}

const char *status_str(enum status_t status) {
  switch (status) {
    case OK:
      return "OK";
    case ERR_KH_NULL:
      return "Keyboard handle is NULL.";
    case ERR_CMD_NULL:
      return "Empty command.";
    case ERR_CMD_UNKNOWN:
      return "Command unknown.";
    case ERR_CMD_ARGS_MISSING:
      return "Insufficient command arguments.";
    case ERR_CMD_ARGS_RANGE:
      return "Command arguments not in range.";
    case ERR_MODE_UNKNOWN:
      return "Color mode unknown.";
    case ERR_KEY_UNKNOWN:
      return "Key unknown.";
    case ERR_KBD_CLAIM:
      return "Failed to connect to the keyboard.";
    case ERR_KBD_RELEASE:
      return "Failed to kbd_release the keyboard.";
    case ERR_KBD_KMAP:
      return "Failed to map keys.";
    case ERR_KBD_MODE:
      return "Failed to set color mode.";
    case ERR_KBD_BRIGHTNESS:
      return "Failed to set brightness.";
    case ERR_KBD_COLOR:
      return "Failed to set color.";
    case ERR_KBD_KCOLOR:
      return "Failed to set key color.";
    case ERR_KMAP_READ:
      return "Failed reading kmap file.";
    case ERR_KBD_DIR:
      return "Failed to set direction.";
    case ERR_KBD_RAINBOW:
      return "Failed to set rainbow.";
    case ERR_KBD_RATE:
      return "Failed to set report rate.";
    case ERR_KBD_SPEED:
      return "Failed to set animation speed.";
    default:
      return "Unknown error.";
  }
}
