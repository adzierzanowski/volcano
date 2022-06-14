#ifndef FMT_H
#define FMT_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include <libusb.h>

enum loglevel_t {
  ERROR = 0,
  WARNING = 1,
  INFO = 2,
  DEBUG = 3
};

void printerr(const char *fmt, ...);
void print_libusb_err(int status);
void print64(uint8_t *buf);
void dlog(enum loglevel_t level, const char *fmt, ...);
void set_loglevel(enum loglevel_t level);

#endif
