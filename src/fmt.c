#include "fmt.h"

#define RED "\x1b[38;5;1m"
#define END "\x1b[0m"

static enum loglevel_t _loglevel = WARNING;

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

static const char *loglevelstr(enum loglevel_t level) {
  switch (level) {
    case ERROR:   return "ERROR";
    case WARNING: return "WARN ";
    case INFO:    return "INFO ";
    case DEBUG:   return "DEBUG";
  }
}

void dlog(enum loglevel_t level, const char *fmt, ...) {
  if (level <= _loglevel) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    va_list args;
    va_start(args, fmt);
    fprintf(
      stderr,
      "%d-%02d-%02d %02d:%02d:%02d    %s    ",
      tm.tm_year + 1900,
      tm.tm_mon+1,
      tm.tm_mday,
      tm.tm_hour,
      tm.tm_min,
      tm.tm_sec,
      loglevelstr(level));
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}

void set_loglevel(enum loglevel_t level) {
  _loglevel = level;
}
