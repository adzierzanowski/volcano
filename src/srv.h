#ifndef SRV_H
#define SRV_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

#include "keyboard.h"
#include "fmt.h"
#include "version.h"

#define SMALLBUFSZ 1024
#define BUFSZ 10240

const char *http_status_str(int status);
char *make_response(int status, const char *json_msg);
void on_exit(void);
void sigint_handler(int arg);
char **split_path(char *path, size_t *outsz, const char *path_delim);
void split_path_free(char **spath, size_t spathsz);
bool daemon_send(char *rxbuf, const char *fmt, ...);
char *dispatch_cmd(char **spath, size_t spathsz, char *body);
int main(int argc, const char *argv[]);

#endif
