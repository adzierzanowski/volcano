#include "daemon.h"


static struct vlc_config_t *cfg;

// The daemon's UNIX socket
static int dsock = -1;

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
  vlc_log(VLC_LOG_INFO, "Remapping from file %s\n", fname);
  struct vlc_kbd_keymap_t kmap;
  if (!vlc_kbd_read_keymap_file(fname, &kmap)) {
    vlc_log(VLC_LOG_ERROR, "Failed reading kmap file.\n");
    return false;
  }
  vlc_kbd_remap(kbdh, &kmap);
  vlc_log(VLC_LOG_DEBUG, "Remapping complete\n");
  return true;
}

// Sends initial configuration commands to the keyboard when hotplugged
static void on_hotplug(void) {
  if (kbdh) {
    vlc_kbd_claim();
    vlc_log(VLC_LOG_INFO, "[Hotplug] Remapping keys.\n");
    remap(cfg->kmap_file);
    vlc_log(VLC_LOG_INFO, "[Hotplug] Setting initial mode to %s.\n", cfg->init_mode);
    enum vlc_kbd_mode_t mode = vlc_kbd_get_mode(cfg->init_mode);
    vlc_kbd_set_mode(kbdh, mode);
    vlc_kbd_release();
    vlc_kbd_claim();
    vlc_log(VLC_LOG_INFO,
      "[Hotplug] Setting initial color to #%06x.\n", cfg->init_color);
    vlc_kbd_set_color(
      kbdh, cfg->init_color >> 16, cfg->init_color >> 8, cfg->init_color);
    vlc_kbd_release();
  } else {
    vlc_log(
      VLC_LOG_ERROR,
      "[Hotplug] "
      "Keyboard has been hotplugged but the keyboard handle is NULL.\n");
  }
}

static void vlc_daemon_on_exit(void) {
  if (dsock > 0) {
    vlc_log(VLC_LOG_INFO, "[Exit] Closing the daemon socket.\n");
    close(dsock);
  }

  if (vlc_fexists(cfg->socket_file)) {
    vlc_log(
      VLC_LOG_INFO,
      "[Exit] Removing the daemon socket file: %s.\n",
      cfg->socket_file);
    remove(cfg->socket_file);
  }

  vlc_log(VLC_LOG_INFO, "[Exit] Quitting the volcano daemon.\n");
}

static void vlc_daemon_sigint_handler(int sig) {
  vlc_log(VLC_LOG_INFO, "[Exit] SIGINT\n");
  exit(0);
}

int main(int argc, const char *argv[]) {
  atexit(vlc_daemon_on_exit);
  signal(SIGINT, vlc_daemon_sigint_handler);
  vlc_set_loglevel(VLC_LOG_DEBUG);

  cfg = vlc_config_get();

  vlc_log(VLC_LOG_INFO, "\n");
  vlc_log(VLC_LOG_INFO, "Starting volcanod\n");
  char *cwd = getcwd(NULL, 0);
  vlc_log(VLC_LOG_DEBUG, "CWD: %s\n", cwd);
  free(cwd);

  for (int i = 0; i < argc; i++) {
    vlc_log(VLC_LOG_DEBUG, "argv[%d]=%s\n", i, argv[i]);
  }

  const char *cfg_fname = argc < 2 ? NULL : argv[1];
  vlc_config_init(cfg_fname);

  if (cfg->srv_enable) {
    pid_t pid = fork();

    if (pid == 0) {
      vlc_log(VLC_LOG_INFO, "Creating a child server.\n");
      int err = execl(cfg->srv_exe, "volcanosrv", cfg_fname, NULL);

      if (err) {
        vlc_log(VLC_LOG_ERROR, "execl failed: %s\n", strerror(errno));
        exit(1);
      }

    } else if (pid < 0) {
      vlc_log(VLC_LOG_ERROR, "Could not create a child process.\n");
      exit(1);

    } else {
      return vlc_daemon_main(argc, argv);
    }
  }

  return vlc_daemon_main(argc, argv);
}

int vlc_daemon_main(int argc, const char *argv[]) {
  vlc_log(VLC_LOG_ALWAYS, "volcanod version %s\n", VLC_VERSION);

  libusb_init(&ctx);
  libusb_hotplug_callback_handle cbhandle;

  vlc_log(VLC_LOG_DEBUG, "Attempting to register hotplug callback.\n");
  int status = libusb_hotplug_register_callback(
    ctx,
    LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
    0,
    VLC_KBD_VID,
    VLC_KBD_PID,
    LIBUSB_HOTPLUG_MATCH_ANY,
    vlc_hotplug_handler,
    NULL,
    &cbhandle);

  if (status != 0) {
    vlc_log(VLC_LOG_ERROR, "Failed to register hotplug callback.\n");
    exit(1);
  }

  if (!kbdh) {
    kbdh = libusb_open_device_with_vid_pid(ctx, VLC_KBD_VID, VLC_KBD_PID);
    vlc_log(VLC_LOG_DEBUG, "Initial keyboard connection: %p\n", kbdh);
    if (!kbdh) {
      vlc_log(VLC_LOG_WARNING, "Failed to connect to the keyboard.\n");
    }
  }

  on_hotplug();

  dsock = vlc_daemon_create_socket();
  vlc_log(VLC_LOG_INFO, "Socket created: fd=%d\n", dsock);

  for (;;) {
    libusb_handle_events_timeout_completed(ctx, &timeout, NULL);
    socklen_t len;

    if (kbd_hotplugged) {
      kbd_hotplugged = false;
      on_hotplug();
    }

    // The socket file descriptor is set to NONBLOCKING
    // So accept() returns immediately
    int rsock = accept(dsock, (struct sockaddr *) &addr, &len);

    if (rsock != -1) {
      char buf[VLC_SMALLBUFSZ] = {0};
      ssize_t rlen = recv(rsock, buf, VLC_SMALLBUFSZ, 0);
      vlc_log(VLC_LOG_DEBUG, "Received %zd bytes: %s\n", rlen, buf);

      if (!kbdh) {
        vlc_log(VLC_LOG_ERROR, "Keyboard handle is NULL.\n");
        send(rsock, vlc_status_str(VLC_ERR_KH_NULL), strlen(vlc_status_str(VLC_ERR_KH_NULL)), 0);
        close(rsock);
        continue;
      }

      enum vlc_status_t status = vlc_daemon_parse_command(buf);
      send(rsock, vlc_status_str(status), strlen(vlc_status_str(status)), 0);
      close(rsock);
    }

    usleep(100000);
  }

  return 0;
}

bool vlc_kbd_claim() {
  if (libusb_detach_kernel_driver(kbdh, VLC_CTL_INTERFACE)) return false;
  if (libusb_claim_interface(kbdh, VLC_CTL_INTERFACE)) return false;
  return true;
}

bool vlc_kbd_release() {
  if (libusb_release_interface(kbdh, VLC_CTL_INTERFACE)) return false;
  if (libusb_attach_kernel_driver(kbdh, VLC_CTL_INTERFACE)) return false;
  return true;
}

int vlc_daemon_create_socket() {
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, cfg->socket_file);

  int s = socket(AF_UNIX, SOCK_STREAM, 0);

  fcntl(s, F_SETFL, O_NONBLOCK);

  if (s == -1) {
    vlc_log(VLC_LOG_ERROR, "Could not create socket.\n");
    exit(1);
  }

  size_t addrsz = strlen(addr.sun_path) + sizeof (addr.sun_family) + 1;

  int status = bind(s, (struct sockaddr *) &addr, addrsz);
  if (status != 0) {
    vlc_log(VLC_LOG_ERROR, "Failed to bind to the socket.\n");
    exit(1);
  }

  vlc_log(VLC_LOG_DEBUG, "Setting permissions to the socket file.\n");

  if (chmod(cfg->socket_file, S_IRWXG | S_IRWXO | S_IRWXU) == -1) {
    vlc_log(
      VLC_LOG_WARNING,
      "Couldn't change the permissions of the socket file: %s",
      strerror(errno));
  }

  if (chown(cfg->socket_file, cfg->socket_uid, cfg->socket_gid) == -1) {
    vlc_log(
      VLC_LOG_WARNING,
      "Couldn't change the ownership of the socket file: %s",
      strerror(errno));
  }

  if (listen(s, 5) == -1) {
    vlc_log(VLC_LOG_ERROR, "Failed to listen.\n");
    exit(1);
  }

  return s;
}

int vlc_hotplug_handler(
    struct libusb_context *ctx,
    struct libusb_device *dev,
    libusb_hotplug_event event,
    void *user_data) {

  if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
    kbdh = libusb_open_device_with_vid_pid(ctx, VLC_KBD_VID, VLC_KBD_PID);
    vlc_log(VLC_LOG_INFO, "Keyboard hotplugged\n");
    kbd_hotplugged = true;

  } else if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT) {
    libusb_close(kbdh);
    kbdh = NULL;
    vlc_log(VLC_LOG_INFO, "Keyboard unplugged\n");
  }

  return 0;
}

enum vlc_status_t vlc_daemon_parse_command(char *cmdbuf) {
  vlc_log(VLC_LOG_DEBUG, "Parsing command\n");
  const char *delim = " \n";
  char *tok = strtok(cmdbuf, delim);

  if (!tok) {
    vlc_log(VLC_LOG_WARNING, "Empty command.\n");
    return VLC_ERR_CMD_NULL;
  }

  if (strcmp(tok, "quit") == 0) {
    vlc_log(VLC_LOG_INFO, "Quitting.\n");
    exit(0);

  } else if (strcmp(tok, "fkmap") == 0) {
    tok = strtok(NULL, delim);

    const char *kmap_file = cfg->kmap_file;

    if (tok != NULL) {
      kmap_file = tok;
    }

    struct vlc_kbd_keymap_t kmap;
    vlc_log(VLC_LOG_INFO, "Mapping keys to %s.\n", kmap_file);
    if (!vlc_kbd_read_keymap_file(kmap_file, &kmap)) {
      return VLC_ERR_KMAP_READ;
    }
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_remap(kbdh, &kmap)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_KMAP;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;

  } else if (strcmp(tok, "kmap") == 0) {
    uint8_t data[172] = {0};
    int dataptr = 0;

    tok = strtok(NULL, delim);
    while (tok != NULL) {
      data[dataptr++] = atoi(tok);
      tok = strtok(NULL, delim);
    }

    struct vlc_kbd_keymap_t kmap;
    memcpy(&kmap, data, 172);

    vlc_log(VLC_LOG_INFO, "Mapping keys from direct data.\n");
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_remap(kbdh, &kmap)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_KMAP;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;

    return VLC_OK;

  } else if (strcmp(tok, "mode") == 0) {
    tok = strtok(NULL, delim);

    if (tok == NULL) {
      vlc_log(VLC_LOG_WARNING, "Mode command with no argument.\n");
      return VLC_ERR_CMD_ARGS_MISSING;
    }

    enum vlc_kbd_mode_t mode = vlc_kbd_get_mode(tok);

    if (mode == VLC_KBD_MODE_UNKNOWN) {
      vlc_log(VLC_LOG_WARNING, "Unknown mode: %s\n", tok);
      return VLC_ERR_MODE_UNKNOWN;
    }

    vlc_log(VLC_LOG_INFO, "Setting mode to %s.\n", tok);
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_mode(kbdh, mode)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_MODE;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;

  } else if (strcmp(tok, "color") == 0) {
    int r = 0;
    int g = 0;
    int b = 0;

    tok = strtok(NULL, delim);
    if (tok != NULL) {
      r = atoi(tok);
      if (r < 0 || r > 255) {
        vlc_log(VLC_LOG_ERROR, "Color not in range 0-255.\n");
        return VLC_ERR_CMD_ARGS_RANGE;
      }

      tok = strtok(NULL, delim);
      if (tok != NULL) {
        g = atoi(tok);
        if (g < 0 || g > 255) {
          vlc_log(VLC_LOG_ERROR, "Color not in range 0-255.\n");
          return VLC_ERR_CMD_ARGS_RANGE;
        }

        tok = strtok(NULL, delim);
        if (tok != NULL) {
          b = atoi(tok);
          if (b < 0 || b > 255) {
            vlc_log(VLC_LOG_ERROR, "Color not in range 0-255.\n");
            return VLC_ERR_CMD_ARGS_RANGE;
          }
        }
      }
    }

    vlc_log(
      VLC_LOG_INFO,
      "Setting color to RGB(%d, %d, %d) HEX(#%02x%02x%02x).\n",
      r, g, b, r, g, b);

    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_color(kbdh, r, g, b)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_COLOR;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;

    return VLC_OK;

  } else if (strcmp(tok, "kcolor") == 0) {
    char keycode[128] = {0};
    int r = 0;
    int g = 0;
    int b = 0;

    tok = strtok(NULL, delim);
    if (tok == NULL) {
      vlc_log(VLC_LOG_WARNING, "Key color command with no KEY argument.\n");
      return VLC_ERR_CMD_ARGS_MISSING;
    }

    sprintf(keycode, "%s", tok);
    vlc_log(VLC_LOG_DEBUG, "kcolor keycode=%s\n", keycode);

    tok = strtok(NULL, delim);
    if (tok) {
      r = atoi(tok);
      if (r < 0 || r > 255) {
        vlc_log(VLC_LOG_ERROR, "Key color not in range 0-255.\n");
        return VLC_ERR_CMD_ARGS_RANGE;
      }

      tok = strtok(NULL, delim);
      if (tok) {
        g = atoi(tok);
        if (g < 0 || g > 255) {
          vlc_log(VLC_LOG_ERROR, "Key color not in range 0-255.\n");
          return VLC_ERR_CMD_ARGS_RANGE;
        }

        tok = strtok(NULL, delim);
        if (tok) {
          b = atoi(tok);
          if (b < 0 || b > 255) {
            vlc_log(VLC_LOG_ERROR, "Key color not in range 0-255.\n");
            return VLC_ERR_CMD_ARGS_RANGE;
          }
        }
      }
    }
    vlc_log(VLC_LOG_DEBUG, "COLORS PARSED: %d %d %d\n", r, g, b);

    const struct vlc_kbd_key_t *key = vlc_kbd_get_key(keycode);
    if (key == NULL) {
      vlc_log(VLC_LOG_WARNING, "Unknown key: %s\n", keycode);
      return VLC_ERR_KEY_UNKNOWN;
    }

    vlc_log(VLC_LOG_DEBUG, "KEY STRUCT=%p\n", key);

    vlc_log(
      VLC_LOG_INFO,
      "Setting key %s to RGB(%d, %d, %d) HEX(#%02x%02x%02x).\n",
      keycode, r, g, b, r, g, b);
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_mode(kbdh, VLC_KBD_MODE_CUSTOM)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_MODE;
    }
    uint32_t color = (r << 16) | (g << 8) | b;
    vlc_log(VLC_LOG_DEBUG, "color=%06x\n", color);
    if (!vlc_kbd_set_key_color(kbdh, key->val, color)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_KCOLOR;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;

  } else if (strcmp(tok, "brightness") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      vlc_log(VLC_LOG_WARNING, "Brightness command without LEVEL argument.\n");
      return VLC_ERR_CMD_ARGS_RANGE;
    }

    int level = atoi(tok);
    if (level < 0 || level > 4) {
      vlc_log(VLC_LOG_WARNING, "Brightness level not in range (0, 4).\n");
      return VLC_ERR_CMD_ARGS_RANGE;
    }

    vlc_log(VLC_LOG_INFO, "Setting brightness level to %d\n", level);
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_brightness(kbdh, level)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_BRIGHTNESS;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;

  } else if (strcmp(tok, "speed") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      vlc_log(VLC_LOG_WARNING, "Speed command without LEVEL argument.\n");
      return VLC_ERR_CMD_ARGS_MISSING;
    }

    int level = atoi(tok);
    if (level < 0 || level > 4) {
      vlc_log(VLC_LOG_WARNING, "Speed level not in range (0, 4).\n");
      return VLC_ERR_CMD_ARGS_RANGE;
    }

    vlc_log(VLC_LOG_INFO, "Setting speed level to %d.\n", level);
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_speed(kbdh, level)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_SPEED;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;

  } else if (strcmp(tok, "rate") == 0) {
    tok = strtok(NULL, delim);

    if (tok == NULL) {
      vlc_log(VLC_LOG_WARNING, "Report rate command without an argument.\n");
      return VLC_ERR_CMD_ARGS_MISSING;
    }

    enum vlc_kbd_rate_t rate;
    if (strcmp(tok, "125") == 0) {
      rate = VLC_KBD_RATE_125HZ;
    } else if (strcmp(tok, "250") == 0) {
      rate = VLC_KBD_RATE_250HZ;
    } else if (strcmp(tok, "500") == 0) {
      rate = VLC_KBD_RATE_500HZ;
    } else if (strcmp(tok, "1000") == 0) {
      rate = VLC_KBD_RATE_1000HZ;
    } else {
      vlc_log(VLC_LOG_WARNING, "Report rate not available: %s.\n", tok);
      return VLC_ERR_CMD_ARGS_RANGE;
    }

    vlc_log(VLC_LOG_INFO, "Setting report rate to %s Hz.\n", tok);
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_report_rate(kbdh, rate)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_RATE;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;

  } else if (strcmp(tok, "dir") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      vlc_log(VLC_LOG_WARNING, "Direction command without an argument.\n");
      return VLC_ERR_CMD_ARGS_MISSING;
    }

    // TODO: setting direction fails
    enum vlc_kbd_dir_t dir = atoi(tok);
    if (dir < 0 || dir > 1) {
      vlc_log(VLC_LOG_WARNING, "Direction not in range (0, 1).\n");
      return VLC_ERR_CMD_ARGS_RANGE;
    }

    vlc_log(VLC_LOG_INFO, "Setting direction to %d.\n", dir);
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_direction(kbdh, dir)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_DIR;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;

  } else if (strcmp(tok, "rainbow") == 0) {
    tok = strtok(NULL, delim);
    if (tok == NULL) {
      vlc_log(VLC_LOG_WARNING, "Rainbow command without an argument.\n");
      return VLC_ERR_CMD_ARGS_MISSING;
    }

    bool rainbow;
    if (strcmp(tok, "true") == 0) {
      rainbow = true;
    } else if (strcmp(tok, "false") == 0) {
      rainbow = false;
    } else {
      vlc_log(VLC_LOG_WARNING, "Invalid argument for rainbow command.\n");
      return VLC_ERR_CMD_ARGS_RANGE;
    }

    vlc_log(VLC_LOG_INFO, "Setting rainbow to %s.\n", tok);
    if (!vlc_kbd_claim()) return VLC_ERR_KBD_CLAIM;
    if (!vlc_kbd_set_rainbow(kbdh, rainbow)) {
      vlc_kbd_release();
      return VLC_ERR_KBD_RAINBOW;
    }
    if (!vlc_kbd_release()) return VLC_ERR_KBD_RELEASE;
    return VLC_OK;
  }

  vlc_log(VLC_LOG_WARNING, "Unknown command: %s\n", cmdbuf);
  return VLC_ERR_CMD_UNKNOWN;
}

const char *vlc_status_str(enum vlc_status_t status) {
  switch (status) {
    case VLC_OK:
      return "VLC_OK";
    case VLC_ERR_KH_NULL:
      return "Keyboard handle is NULL.";
    case VLC_ERR_CMD_NULL:
      return "Empty command.";
    case VLC_ERR_CMD_UNKNOWN:
      return "Command unknown.";
    case VLC_ERR_CMD_ARGS_MISSING:
      return "Insufficient command arguments.";
    case VLC_ERR_CMD_ARGS_RANGE:
      return "Command arguments not in range.";
    case VLC_ERR_MODE_UNKNOWN:
      return "Color mode unknown.";
    case VLC_ERR_KEY_UNKNOWN:
      return "Key unknown.";
    case VLC_ERR_KBD_CLAIM:
      return "Failed to connect to the keyboard.";
    case VLC_ERR_KBD_RELEASE:
      return "Failed to vlc_kbd_release the keyboard.";
    case VLC_ERR_KBD_KMAP:
      return "Failed to map keys.";
    case VLC_ERR_KBD_MODE:
      return "Failed to set color mode.";
    case VLC_ERR_KBD_BRIGHTNESS:
      return "Failed to set brightness.";
    case VLC_ERR_KBD_COLOR:
      return "Failed to set color.";
    case VLC_ERR_KBD_KCOLOR:
      return "Failed to set key color.";
    case VLC_ERR_KMAP_READ:
      return "Failed reading kmap file.";
    case VLC_ERR_KBD_DIR:
      return "Failed to set direction.";
    case VLC_ERR_KBD_RAINBOW:
      return "Failed to set rainbow.";
    case VLC_ERR_KBD_RATE:
      return "Failed to set report rate.";
    case VLC_ERR_KBD_SPEED:
      return "Failed to set animation speed.";
    default:
      return "Unknown error.";
  }
}
