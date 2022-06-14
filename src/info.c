#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>

#define KBD_VID 0x0c45
#define KBD_PID 0x0820

int main(int argc, char *argv[]) {
  libusb_context *ctx = NULL;
  libusb_init(&ctx);
  libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);

  libusb_device_handle *kbdh = libusb_open_device_with_vid_pid(ctx, KBD_VID, KBD_PID);
  if (kbdh == NULL) {
    fprintf(stderr, "Keyboard not found\n");
    exit(1);
  }

  libusb_device *kbd = libusb_get_device(kbdh);

  struct libusb_config_descriptor *conf_desc;
  int res = libusb_get_active_config_descriptor(kbd, &conf_desc);
  if (res == 0) {
    printf("bNumInterfaces=%d\n", conf_desc->bNumInterfaces);

    for (int i = 0; i < conf_desc->bNumInterfaces; i++) {
      const struct libusb_interface iface = conf_desc->interface[i];
      printf("Iface #%d\n", i);
      printf("  num_altsetting=%d\n", iface.num_altsetting);

      for (int j = 0; j < iface.num_altsetting; j++) {
        printf("    altsetting #%d\n", j);
        const struct libusb_interface_descriptor iface_desc = iface.altsetting[j];
        printf("      bInterfaceNumber=%d\n", iface_desc.bInterfaceNumber);
        printf("      bNumEndpoints=%d\n", iface_desc.bNumEndpoints);

        for (int k = 0; k < iface_desc.bNumEndpoints; k++) {
          printf("        Endpoint #%d\n", k);
          const struct libusb_endpoint_descriptor endp_desc = iface_desc.endpoint[k];
          printf("        bEndpointAddress: %d\n", endp_desc.bEndpointAddress);
          printf("        wMaxPacketSize: %d\n", endp_desc.wMaxPacketSize);
          printf("        bmAttributes: %02x\n", endp_desc.bmAttributes);
          printf("        extraLength: %d\n", endp_desc.extra_length);

        }


      }
    }
  }

  libusb_free_config_descriptor(conf_desc);


  libusb_close(kbdh);

  printf("Does libusb have hotplug? %d\n", libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG));

  libusb_exit(ctx);

  return 0;
}
