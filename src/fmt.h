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

#define VLC_BUFSZ 20480
#define VLC_SMALLBUFSZ 2048


enum vlc_loglevel_t {
  VLC_LOG_ALWAYS = -1,
  VLC_LOG_ERROR = 0,
  VLC_LOG_WARNING = 1,
  VLC_LOG_INFO = 2,
  VLC_LOG_DEBUG = 3,
  VLC_LOG_SILLY = 4,
};


// Prints to stderr if `level` matches configured logging level
void vlc_log(enum vlc_loglevel_t level, const char *fmt, ...);

// Returns loglevel name
const char *vlc_loglevelstr(enum vlc_loglevel_t level);

// Sets the logging level
void vlc_set_loglevel(enum vlc_loglevel_t level);

// Sets ANSI-escape-code-colored logging option
void vlc_set_logcolor(bool do_color);


// Prints libusb error string based on libusb status code
void vlc_print_libusb_err(int status);

// Returns `true` if `str` matches any of passed arguments
// The argument list is delimited by passing `NULL` as a last arg
bool vlc_strmatch(char *str, ...);

// Returns an array of strings made by spliting `str` by `delim`
// The lenght of the returned array is saved in `outsz`
char **vlc_strsplit(char *str, size_t *outsz, const char *delim);

// Frees memory allocated for the array created with `vlc_strsplit`
void vlc_strsplit_free(char **spath, size_t spathsz);


#endif
