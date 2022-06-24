/* This is an optional server which serves a local control panel for
   communicating with the daemon. */

#ifndef VLC_SRV_H
#define VLC_SRV_H

#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
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

#include "config.h"
#include "fmt.h"
#include "keyboard.h"
#include "version.h"


int main(int argc, const char *argv[]);

// Returns a mime type string based on filename extension of `path`
const char *vlc_http_get_mimetype(const char *path);
// Returns an HTTP response with a JSON body
char *vlc_http_make_api_response(int status, const char *json_msg, size_t *ressz);
// Returns an HTTP response
char *vlc_http_make_response(
   int status,
   const char *mime_type,
   uint8_t *body,
   size_t *bodysz,
   size_t *ressz);
// Returns a string containing suitable string for HTTP status line
const char *vlc_http_status_str(int status);

// Creates the main TCP socket for the server
int vlc_srv_create_socket(void);
// Main server loop which listens for incoming HTTP requests
void vlc_srv_serve(int srv_socket);
// Handles incoming request
char *vlc_srv_handle_request(char *method, char *path, char *body, size_t *ressz);
// Handles GET requests
char *vlc_srv_handle_request_get(char *path, size_t *ressz);
// Handles POST requests
char *vlc_srv_handle_request_post(char *path, char *body, size_t *ressz);

// Sends a variadic command to the daemon UNIX socket
// Returns `true` if successful, `false` otherwise
// Saves the daemon response in `rxbuf`
// `fmt` is a printf-like format of the command
bool vlc_srv_daemon_send(char *rxbuf, const char *fmt, ...);
// Validates and sends a command to the daemon
// `rxbuf` is a buffer in which the daemon response is stored
// `spath` is the incoming HTTP request path
// `expected_spathsz` is the expected size of the called endpoint when split by '/'
// `ressz` is a pointer to the size of the returned HTTP response
char *vlc_srv_cmd_send(
   char *rxbuf,
   char **spath,
   size_t spathsz,
   size_t expected_spathsz,
   size_t *ressz);
// Converts HTTP request to a daemon command and sends it to the daemon
// Returns a HTTP response
// `spath` is the incoming HTTP request path split by slash characters
// `body` is the incoming HTTP request body
// `ressz` is a pointer to the size of the returned HTTP resonse
char *vlc_srv_cmd_handle(char **spath, size_t spathsz, char *body, size_t *ressz);

#endif
