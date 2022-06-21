#include "config.h"


static struct config_t config;

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

struct config_t *config_get(void) {
  return &config;
}


void config_init(const char *rcfname) {
  dlog(LOG_INFO, "Initializing config\n");
  dlog(LOG_DEBUG, "Config file path: %s\n", rcfname);

  // Path to the volcano executable
  // Note that argv[0] may contain anything that the parent process put there
  // so we must use OS-specific methods.
  char vpath[SMALLBUFSZ] = {0};
  int status = -1;
  uint32_t vpathsz = SMALLBUFSZ;

#if defined(__APPLE__)
  status = _NSGetExecutablePath(vpath, &vpathsz);
#elif defined(__linux__)
  status = readlink("/proc/self/exe", vpath, vpathsz);
#endif

  if (status == -1) {
    dlog(
      LOG_WARNING,
      "Couldn't get volcano path. Needed buffer size (if on macOS): %u\n",
      vpathsz);
  }

  struct passwd *pw = getpwuid(VOLCANO_DEFAULT_UID);
  const char *homedir = pw->pw_dir;

  // The defaults
  config.kmap_file = calloc(SMALLBUFSZ, sizeof (char));
  config.socket_file = calloc(SMALLBUFSZ, sizeof (char));
  config.init_mode = calloc(SMALLBUFSZ, sizeof (char));
  config.srv_data = calloc(SMALLBUFSZ, sizeof (char));
  config.srv_exe = calloc(SMALLBUFSZ, sizeof (char));

  sprintf(config.socket_file, "%s/.volcano.sock", homedir);
  sprintf(config.init_mode, "%s", VOLCANO_DEFAULT_MODE);

  char pathbuf[SMALLBUFSZ] = {0};
  strcpy(pathbuf, vpath);
  droplastslash(pathbuf);
  droplastslash(pathbuf);
  sprintf(config.srv_data, "%s/www", pathbuf);

  strcpy(pathbuf, vpath);
  droplastslash(pathbuf);
  sprintf(config.srv_exe, "%s/volcanosrv", pathbuf);

  config.socket_uid = VOLCANO_DEFAULT_UID;
  config.socket_gid = VOLCANO_DEFAULT_GID;
  config.init_color = VOLCANO_DEFAULT_COLOR;
  config.srv_port = VOLCANO_DEFAULT_PORT;
  config.srv_enable = true;

  if (rcfname == NULL) {
    dlog(
      LOG_WARNING,
      "Config filename not provided. "
      "The default values may result in errors.\n");
  } else {
    struct stat statbuf;
    if (stat(rcfname, &statbuf) == 0) {
      config_read(rcfname);
    } else {
      dlog(LOG_WARNING, "Config file not found: %s.\n", rcfname);
    }
  }

  dlog(LOG_DEBUG, "[Config] KMAP_FILE:   %s\n", config.kmap_file);
  dlog(LOG_DEBUG, "[Config] SOCKET_FILE: %s\n", config.socket_file);
  dlog(LOG_DEBUG, "[Config] INIT_MODE:   %s\n", config.init_mode);
  dlog(LOG_DEBUG, "[Config] SRV_DATA:    %s\n", config.srv_data);
  dlog(LOG_DEBUG, "[Config] SRV_EXE:     %s\n", config.srv_exe);
  dlog(LOG_DEBUG, "[Config] SOCKET_UID:  %d\n", config.socket_uid);
  dlog(LOG_DEBUG, "[Config] SOCKET_GID:  %d\n", config.socket_gid);
  dlog(LOG_DEBUG, "[Config] INIT_COLOR:  #%06x\n", config.init_color);
  dlog(LOG_DEBUG, "[Config] SRV_PORT:    %hu\n", config.srv_port);
  dlog(LOG_DEBUG, "[Config] SRV_ENABLE:  %s\n", config.srv_enable ? "true" : "false");
}

void config_read(const char *rcfname) {
  char buf[BUFSZ] = {0};
  FILE *f = fopen(rcfname, "r");
  fread(buf, sizeof (char), BUFSZ, f);
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
      enum loglevel_t lvl = atoi(tok);
      dlog(LOG_DEBUG, "[Config] LOG_LEVEL:   %s\n", loglevelstr(lvl));
      set_loglevel(lvl);

    } else if (strcmp(key, "LOG_COLOR") == 0) {
      bool val = atoi(tok);
      dlog(LOG_DEBUG, "[Config] LOG_COLOR:   %s\n", val ? "true" : "false");
      set_logcolor(val);

    } else if (strcmp(key, "SRV_ENABLE") == 0) {
      config.srv_enable = atoi(tok);

    } else if (strcmp(key, "SRV_PORT") == 0) {
      config.srv_port = atoi(tok);

    } else if (strcmp(key, "SRV_DATA") == 0) {
      sprintf(config.srv_data, "%s", tok);

    } else if (strcmp(key, "SRV_EXE") == 0) {
      sprintf(config.srv_exe, "%s", tok);

    } else {
      dlog(LOG_WARNING, "[Config] Unknown config key: %s\n", key);
    }

    tok = strtok(NULL, "=");
  }
}
