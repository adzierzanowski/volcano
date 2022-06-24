#include "config.h"


static struct vlc_config_t config;

static void droplastslash(char *buf) {
  int last_slash = -1;
  for (int i = 0; i < strlen(buf); i++) {
    if (buf[i] == '/') {
      last_slash = i;
    }
  }

  if (last_slash > -1) {
    buf[last_slash] = 0;
  }
}

struct vlc_config_t *vlc_config_get(void) {
  return &config;
}


void vlc_config_init(const char *rcfname) {
  vlc_log(VLC_LOG_INFO, "Initializing config\n");
  vlc_log(VLC_LOG_DEBUG, "Config file path: %s\n", rcfname);

  // Path to the volcano executable
  // Note that argv[0] may contain anything that the parent process put there
  // so we must use OS-specific methods.
  char vpath[VLC_SMALLBUFSZ] = {0};
  int status = -1;
  uint32_t vpathsz = VLC_SMALLBUFSZ;

#if defined(__APPLE__)
  status = _NSGetExecutablePath(vpath, &vpathsz);
#elif defined(__linux__)
  status = readlink("/proc/self/exe", vpath, vpathsz);
#endif

  if (status == -1) {
    vlc_log(
      VLC_LOG_WARNING,
      "Couldn't get volcano path. Needed buffer size (if on macOS): %u\n",
      vpathsz);
  }

  struct passwd *pw = getpwuid(VLC_DEFAULT_UID);
  const char *homedir = pw->pw_dir;

  // The defaults
  config.kmap_file = calloc(VLC_SMALLBUFSZ, sizeof (char));
  config.socket_file = calloc(VLC_SMALLBUFSZ, sizeof (char));
  config.init_mode = calloc(VLC_SMALLBUFSZ, sizeof (char));
  config.srv_data = calloc(VLC_SMALLBUFSZ, sizeof (char));
  config.srv_exe = calloc(VLC_SMALLBUFSZ, sizeof (char));

  sprintf(config.socket_file, "%s/.volcano.sock", homedir);
  sprintf(config.init_mode, "%s", VLC_DEFAULT_MODE);

  char pathbuf[VLC_SMALLBUFSZ] = {0};
  strcpy(pathbuf, vpath);
  droplastslash(pathbuf);
  droplastslash(pathbuf);
  sprintf(config.srv_data, "%s/www", pathbuf);

  strcpy(pathbuf, vpath);
  droplastslash(pathbuf);
  sprintf(config.srv_exe, "%s/volcanosrv", pathbuf);

  config.socket_uid = VLC_DEFAULT_UID;
  config.socket_gid = VLC_DEFAULT_GID;
  config.init_color = VLC_DEFAULT_COLOR;
  config.srv_port = VLC_DEFAULT_PORT;
  config.srv_enable = true;

  if (rcfname == NULL) {
    vlc_log(
      VLC_LOG_WARNING,
      "Config filename not provided. "
      "The default values may result in errors.\n");
  } else {
    struct stat statbuf;
    if (stat(rcfname, &statbuf) == 0) {
      vlc_config_read(rcfname);
    } else {
      vlc_log(VLC_LOG_WARNING, "Config file not found: %s.\n", rcfname);
    }
  }

  vlc_log(VLC_LOG_DEBUG, "[Config] KMAP_FILE:   %s\n", config.kmap_file);
  vlc_log(VLC_LOG_DEBUG, "[Config] SOCKET_FILE: %s\n", config.socket_file);
  vlc_log(VLC_LOG_DEBUG, "[Config] INIT_MODE:   %s\n", config.init_mode);
  vlc_log(VLC_LOG_DEBUG, "[Config] SRV_DATA:    %s\n", config.srv_data);
  vlc_log(VLC_LOG_DEBUG, "[Config] SRV_EXE:     %s\n", config.srv_exe);
  vlc_log(VLC_LOG_DEBUG, "[Config] SOCKET_UID:  %d\n", config.socket_uid);
  vlc_log(VLC_LOG_DEBUG, "[Config] SOCKET_GID:  %d\n", config.socket_gid);
  vlc_log(VLC_LOG_DEBUG, "[Config] INIT_COLOR:  #%06x\n", config.init_color);
  vlc_log(VLC_LOG_DEBUG, "[Config] SRV_PORT:    %hu\n", config.srv_port);
  vlc_log(VLC_LOG_DEBUG, "[Config] SRV_ENABLE:  %s\n", config.srv_enable ? "true" : "false");
}

void vlc_config_read(const char *rcfname) {
  char buf[VLC_BUFSZ] = {0};
  FILE *f = fopen(rcfname, "r");
  fread(buf, sizeof (char), VLC_BUFSZ, f);
  fclose(f);

  char *tok = strtok(buf, "=");

  while (tok != NULL) {
    char *key = tok;
    tok = strtok(NULL, "\n");

    if (strcmp(key, "SOCKET_FILE") == 0) {
      sprintf(config.socket_file, "%s", tok);

    } else if (strcmp(key, "SOCKET_UID") == 0) {
      config.socket_uid = atoi(tok);

    } else if (strcmp(key, "SOCKET_GID") == 0) {
      config.socket_gid = atoi(tok);

    } else if (strcmp(key, "KMAP_FILE") == 0) {
      sprintf(config.kmap_file, "%s", tok);

    } else if (strcmp(key, "INIT_MODE") == 0) {
      sprintf(config.init_mode, "%s", tok);

    } else if (strcmp(key, "INIT_COLOR") == 0) {
      config.init_color = strtol(tok, NULL, 16);

    } else if (strcmp(key, "LOG_LEVEL") == 0) {
      enum vlc_loglevel_t lvl = atoi(tok);
      vlc_log(VLC_LOG_DEBUG, "[Config] LOG_LEVEL:   %s\n", vlc_loglevelstr(lvl));
      vlc_set_loglevel(lvl);

    } else if (strcmp(key, "LOG_COLOR") == 0) {
      bool val = atoi(tok);
      vlc_log(VLC_LOG_DEBUG, "[Config] LOG_COLOR:   %s\n", val ? "true" : "false");
      vlc_set_logcolor(val);

    } else if (strcmp(key, "SRV_ENABLE") == 0) {
      config.srv_enable = atoi(tok);

    } else if (strcmp(key, "SRV_PORT") == 0) {
      config.srv_port = atoi(tok);

    } else if (strcmp(key, "SRV_DATA") == 0) {
      sprintf(config.srv_data, "%s", tok);

    } else if (strcmp(key, "SRV_EXE") == 0) {
      sprintf(config.srv_exe, "%s", tok);

    } else {
      vlc_log(VLC_LOG_WARNING, "[Config] Unknown config key: %s\n", key);
    }

    tok = strtok(NULL, "=");
  }
}
