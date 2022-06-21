/* This is an optional server which serves a local control panel for
   communicating with the daemon. */

#ifndef SRV_H
#define SRV_H

#include <ctype.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "fmt.h"
#include "keyboard.h"
#include "version.h"


int main(int argc, const char *argv[]);

// Returns a mime type string based on filename extension of `path`
const char *http_get_mimetype(const char *path);
// Returns an HTTP response with a JSON body
char *http_make_api_response(int status, const char *json_msg, size_t *ressz);
// Returns an HTTP response
char *http_make_response(
   int status,
   const char *mime_type,
   uint8_t *body,
   size_t *bodysz,
   size_t *ressz);
// Returns a string containing suitable string for HTTP status line
const char *http_status_str(int status);

// Handles incoming request
char *handle_request(char *method, char *path, char *body, size_t *ressz);
// Handles GET requests
char *handle_request_get(char *path, size_t *ressz);
// Handles POST requests
char *handle_request_post(char *path, char *body, size_t *ressz);

// Sends a command to the daemon UNIX socket
// Returns `true` if successful, `false` otherwise
// Saves the daemon response in `rxbuf`
bool daemon_send(char *rxbuf, const char *fmt, ...);
// Converts HTTP request to a daemon command and sends it to the daemon
// `spath` is the incoming HTTP request path split by slash characters
// `body` is the incoming HTTP request body
char *dispatch_cmd(char **spath, size_t spathsz, char *body, size_t *ressz);

#endif
