/* String and logging utilities */

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

// Prints to stderr if `level` matches configured logging level
void dlog(enum loglevel_t level, const char *fmt, ...);
// Returns loglevel name
const char *loglevelstr(enum loglevel_t level);
// Sets the logging level
void set_loglevel(enum loglevel_t level);
// Sets ANSI-escape-code-colored logging option
void set_logcolor(bool do_color);

// Prints libusb error string based on libusb status code
void print_libusb_err(int status);

// Returns `true` if `str` matches any of passed arguments
// The argument list is delimited by passing `NULL` as a last arg
bool strmatch(char *str, ...);
// Returns an array of strings made by spliting `str` by `delim`
// The lenght of the returned array is saved in `outsz`
char **strsplit(char *str, size_t *outsz, const char *delim);
// Frees memory allocated for the array created with `strsplit`
void strsplit_free(char **spath, size_t spathsz);

#endif
