#ifndef FMT_H
#define FMT_H

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include <libusb.h>

void printerr(const char *fmt, ...);
void print_libusb_err(int status);
void print64(uint8_t *buf);

#endif
