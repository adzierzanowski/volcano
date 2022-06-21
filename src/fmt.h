#ifndef FMT_H
#define FMT_H

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <libusb.h>
#ifdef __APPLE__
#include <libproc.h>
#endif

#define BUFSZ 20480
#define SMALLBUFSZ 2048

enum loglevel_t {
  LOG_ALWAYS = -1,
  LOG_ERROR = 0,
  LOG_WARNING = 1,
  LOG_INFO = 2,
  LOG_DEBUG = 3,
  LOG_SILLY = 4,
};

void printerr(const char *fmt, ...);
void print_libusb_err(int status);
void print64(uint8_t *buf);
void dlog(enum loglevel_t level, const char *fmt, ...);
const char *loglevelstr(enum loglevel_t level);
void set_loglevel(enum loglevel_t level);
void set_logcolor(bool do_color);
bool strmatch(char *str, ...);
char **strsplit(char *path, size_t *outsz, const char *path_delim);
void strsplit_free(char **spath, size_t spathsz);

#endif
