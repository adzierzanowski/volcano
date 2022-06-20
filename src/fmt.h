#ifndef FMT_H
#define FMT_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <libusb.h>

#define SMALLBUFSZ 1024
#define BUFSZ 10240

enum loglevel_t {
  LOG_ERROR = 0,
  LOG_WARNING = 1,
  LOG_INFO = 2,
  LOG_DEBUG = 3
};

void printerr(const char *fmt, ...);
void print_libusb_err(int status);
void print64(uint8_t *buf);
void dlog(enum loglevel_t level, const char *fmt, ...);
void set_loglevel(enum loglevel_t level);
bool strmatch(char *str, ...);

#endif
