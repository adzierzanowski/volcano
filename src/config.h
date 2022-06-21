#ifndef CONFIG_H
#define CONFIG_H

#include <pwd.h>
#include <string.h>
#include <sys/stat.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include "fmt.h"


#ifdef __APPLE__
#define VOLCANO_DEFAULT_UID 501
#define VOLCANO_DEFAULT_GID 20
#else
#define VOLCANO_DEFAULT_UID 1000
#define VOLCANO_DEFAULT_GID 1000
#endif

#define VOLCANO_DEFAULT_COLOR 0xffffff
#define VOLCANO_DEFAULT_MODE "norm"
#define VOLCANO_DEFAULT_PORT 65226


struct config_t {
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
struct config_t *config_get(void);
// Initializes the configuration
void config_init(const char *rcfname);
// Reads the config from filename
void config_read(const char *rcfname);
// Parses an incoming command and acts accordingly
// Returns status of the action (`OK` on success, other status otherwise)

#endif
