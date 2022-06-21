#include "fmt.h"

#define RED "\x1b[38;5;1m"
#define END "\x1b[0m"

static enum loglevel_t log_level = LOG_DEBUG;
static bool log_colored = false;

void printerr(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "%s", RED);
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "%s", END);
  va_end(args);
}

void print_libusb_err(int status) {
  printerr("Error: %s\n", libusb_error_name(status));
}

void print64(uint8_t *buf) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 16; j++) {
      printf("%02x ", buf[i*16+j]);
    }
    printf("\n");
  }
}

const char *loglevelstr(enum loglevel_t level) {
  switch (level) {
    case LOG_ALWAYS:  return "     ";
    case LOG_ERROR:   return "ERROR";
    case LOG_WARNING: return "WARN ";
    case LOG_INFO:    return "INFO ";
    case LOG_DEBUG:   return "DEBUG";
    case LOG_SILLY:   return "SILLY";
  }
  return "NONE ";
}

void dlog(enum loglevel_t level, const char *fmt, ...) {
  static bool self_name_ready = false;
  static char self_name[SMALLBUFSZ] = {0};

  if (level <= log_level) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    pid_t pid = getpid();

    if (!self_name_ready) {
  #if defined(__APPLE__)
      proc_name(pid, self_name, SMALLBUFSZ);

  #elif defined(__linux__)
      char procfile[SMALLBUFSZ] = {0};
      sprintf(procfile, "/proc/%d/comm", pid);
      FILE *f = fopen(procfile, "r");
      if (f == NULL) {
        sprintf(self_name, "%d", pid);
      } else {
        size_t nread = fread(self_name, sizeof (char), SMALLBUFSZ, f);
        fclose(f);

        // Remove newline char
        self_name[nread-1] = 0;
      }

  #else
      sprintf(self_name, "%d", pid);

  #endif
      self_name_ready = true;
    }

    va_list args;
    va_start(args, fmt);
    if (log_colored) {
      fprintf(stderr, "\x1b[38;5;%dm", pid % 7 + 1);
    }

    fprintf(
      stderr,
      "%d-%02d-%02d %02d:%02d:%02d %-12s %-7s ",
      tm.tm_year + 1900,
      tm.tm_mon+1,
      tm.tm_mday,
      tm.tm_hour,
      tm.tm_min,
      tm.tm_sec,
      self_name,
      loglevelstr(level));
    vfprintf(stderr, fmt, args);
    va_end(args);
    if (log_colored) {
      fprintf(stderr, "\x1b[0m");
    }
  }
}

void set_loglevel(enum loglevel_t level) {
  log_level = level;
}

void set_logcolor(bool do_color) {
  log_colored = do_color;
}

bool strmatch(char *str, ...) {
  va_list args;
  va_start(args, str);
  const char *tok = va_arg(args, const char *);
  while (tok != NULL) {
    if (strcmp(str, tok) == 0) {
      va_end(args);
      return true;
    }
    tok = va_arg(args, const char *);
  }
  va_end(args);
  return false;
}

char **strsplit(char *str, size_t *outsz, const char *delim) {
  if (str == NULL) return NULL;

  *outsz = 0;
  char **out = NULL;

  char *tok = strtok(str, delim);
  while (tok != NULL) {
    char *elem = calloc(strlen(tok) + 1, sizeof (char));
    strcpy(elem, tok);
    *outsz = *outsz + 1;
    out = realloc(out, *outsz * sizeof (char *));
    out[*outsz-1] = elem;
    tok = strtok(NULL, delim);
  }

  return out;
}

void strsplit_free(char **sstr, size_t sstrsz) {
  for (int i = 0; i < sstrsz; i++) {
    free(sstr[i]);
  }
  free(sstr);
}
