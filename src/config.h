#ifndef VLC_CONFIG_H
#define VLC_CONFIG_H

#include <pwd.h>
#include <string.h>
#include <sys/stat.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include "fmt.h"


#ifdef __APPLE__
#define VLC_DEFAULT_UID 501
#define VLC_DEFAULT_GID 20
#else
#define VLC_DEFAULT_UID 1000
#define VLC_DEFAULT_GID 1000
#endif

#define VLC_DEFAULT_COLOR 0xffffff
#define VLC_DEFAULT_MODE "norm"
#define VLC_DEFAULT_PORT 65226

#define VLC_SOCKET_TIMEOUT 1000


struct vlc_config_t {
  char *kmap_file;
  char *socket_file;
  char *init_mode;
  char *srv_data;
  char *srv_exe;
  int socket_uid;
  int socket_gid;
  uint32_t init_color;
  uint16_t srv_port;
  bool srv_enable;
};


// Returns config instance
struct vlc_config_t *vlc_config_get(void);

// Initializes the configuration
void vlc_config_init(const char *rcfname);

// Reads the config from filename
void vlc_config_read(const char *rcfname);


#endif
