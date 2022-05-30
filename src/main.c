#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <stdbool.h>

#define KBD_VID 0x0c45
#define KBD_PID 0x0820
#define CTL_REQUEST_TYPE 0x21
#define CTL_REQUEST 0x09
#define CTL_REQUEST_VALUE 0x0204
#define CTL_INTERFACE 0x01
#define CTL_ENDPOINT 0x82

void print64(uint8_t *buf) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 16; j++) {
      printf("%02x ", buf[i*16+j]);
    }
    printf("\n");
  }
}

bool send(libusb_device_handle *dev, size_t len, ...) {
  uint8_t msg[64] = {0};

  va_list args;
  va_start(args, len);
  for (int i = 0; i < len; i++) {
    msg[i] = va_arg(args, int);
  }
  va_end(args);

  int transferred = libusb_control_transfer(
    dev,
    CTL_REQUEST_TYPE,
    CTL_REQUEST,
    CTL_REQUEST_VALUE,
    CTL_INTERFACE,
    msg,
    64,
    1000);

  if (transferred != 64) {
    fprintf(stderr, "\x1b[38;5;1mError: %s\n", libusb_error_name(transferred));
    return false;
  }

  char buf[64] = {0};
  int status = libusb_bulk_transfer(dev, CTL_ENDPOINT, (unsigned char *) buf, 64, &transferred, 1000);
  if (status != 0) {
    fprintf(stderr, "\x1b[38;5;1mError: %s\n", libusb_error_name(status));
    return false;
  }

  for (int i = 0; i < 64; i++) {
    if (i == 7) {
      if (buf[i] != 1) {
        fprintf(stderr, "\x1b[28;5;1mError: NACK\x1b[0m\n");
        return false;
      }
    } else {
      if ((uint8_t) buf[i] != (uint8_t) msg[i]) {
        fprintf(stderr, "\x1b[28;5;1mError: received data does not match\x1b[0m\n");
        printf("i=%d s:%d != r:%d\n", i, msg[i], buf[i]);
        puts("Transferred:");
        print64(msg);
        puts("Received:");
        print64(buf);
        return false;
      }
    }
  }

  return true;
}

int main(int argc, char *argv[]) {
  int retcode=0;

  libusb_context *ctx = NULL;
  libusb_init(&ctx);
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);

  libusb_device_handle *kbdh = libusb_open_device_with_vid_pid(ctx, KBD_VID, KBD_PID);
  if (kbdh == NULL) {
    fprintf(stderr, "Keyboard not found\n");
    exit(1);
  }

  libusb_set_configuration(kbdh, 1);
  libusb_detach_kernel_driver(kbdh, 1);
  libusb_claim_interface(kbdh, 1);

  // Set "Normally on sequence"
  bool status;
  status = send(kbdh, 4, 0x04, 0x01, 0x00, 0x01);
  if (!status) goto quit;
  status = send(kbdh, 9, 0x04, 0x0D, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, 0x06);
  if (!status) goto quit;
  status = send(kbdh, 4, 0x04, 0x02, 0x00, 0x02);
  if (!status) goto quit;

  // Set the backlight color
  uint8_t r = 0xff;
  uint8_t g = 0x7f;
  uint8_t b = 0;
  status = send(kbdh, 4, 0x04, 0x01, 0x00, 0x01);
  if (!status) goto quit;
  status = send(kbdh, 6, 0x04, 0x0b, 0x00, 0x06, 0x01, 0x04);
  if (!status) goto quit;
  status = send(kbdh, 11, 0x04, 0x0d, 0x01, 0x06, 0x03, 0x05, 0x00, 0x00, r, b, g);
  if (!status) goto quit;
  status = send(kbdh, 4, 0x04, 0x02, 0x00, 0x02);
  if (!status) goto quit;

quit:
  libusb_release_interface(kbdh, 1);
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_ERROR);
  libusb_attach_kernel_driver(kbdh, 1);
  libusb_close(kbdh);
  libusb_exit(ctx);

  return retcode;
}
