#include "srv.h"


static struct vlc_config_t *cfg;

static void srv_on_exit() {
  vlc_log(VLC_LOG_INFO, "[Exit] Quitting volcano server.\n");
}

static void sigint_handler(int arg) {
  exit(0);
}


int main(int argc, const char *argv[]) {
  atexit(srv_on_exit);
  signal(SIGINT, sigint_handler);

  vlc_log(VLC_LOG_ALWAYS, "volcanosrv version %s.\n", VERSION);

  cfg = vlc_config_get();
  vlc_config_init(argc < 2 ? NULL : argv[1]);

  for (int i = 0; i < argc; i++) {
    vlc_log(VLC_LOG_DEBUG, "argv[%d]=%s\n", i, argv[i]);
  }

  vlc_log(VLC_LOG_INFO, "Starting volcano server at http://localhost:%hu\n", cfg->srv_port);
  vlc_log(VLC_LOG_DEBUG, "Server assets path: %s\n", cfg->srv_data);

  int s = socket(AF_INET, SOCK_STREAM, 0);
  int one = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(cfg->srv_port);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(s, (struct sockaddr *) &addr, sizeof addr);
  listen(s, 5);

  for (;;) {
    int sock = accept(s, NULL, NULL);
    char buf[VLC_BUFSZ] = {0};
    recv(sock, buf, VLC_BUFSZ, 0);
    char *method = strtok(buf, " ");
    if (method == NULL) {
      close(sock);
      continue;
    }
    char *path = strtok(NULL, " ");
    if (path == NULL) {
      close(sock);
      continue;
    }

    vlc_log(VLC_LOG_INFO, "%s %s\n", method, path);

    char *header = strtok(NULL, "\n");
    while (strcmp(header, "\r")) {
      vlc_log(VLC_LOG_SILLY, "[Header] %s\n", header);
      header = strtok(NULL, "\n");
    }

    char *body = strtok(NULL, "");
    vlc_log(VLC_LOG_SILLY, "[Body] %s\n", body);

    size_t response_size = 0;
    char *response = handle_request(method, path, body, &response_size);

    vlc_log(VLC_LOG_SILLY, "Sending response:\n\n%s\n\n", response);
    send(sock, response, response_size, 0);
    close(sock);
    free(response);
  }

  return 0;
}

const char *http_get_mimetype(const char *path) {
  if (strcmp(path, "/") == 0) {
    return "text/html";
  }

  size_t spathsz = 0;
  char ppath[VLC_SMALLBUFSZ] = {0};
  sprintf(ppath, "%s", path);
  char **spath = vlc_strsplit(ppath, &spathsz, ".");

  char *ext = spath[spathsz-1];

  const char *mime;

  if (strcmp(ext, "html") == 0) {
    mime = "text/html";
  } else if (strcmp(ext, "js") == 0) {
    mime = "application/javascript";
  } else if (strcmp(ext, "css") == 0) {
    mime = "text/css";
  } else if (strcmp(ext, "ico") == 0) {
    mime = "image/x-icon";
  } else if (strcmp(ext, "json") == 0) {
    mime = "application/json";
  } else {
    mime = "text/plain";
  }

  vlc_strsplit_free(spath, spathsz);

  return mime;
}

char *http_make_api_response(int status, const char *json_msg, size_t *ressz) {
  char body[VLC_SMALLBUFSZ] = {0};
  sprintf(
    body,
    "{\"status\": \"%s\", \"msg\": \"%s\"}",
    status == 200 ? "ok" : "error", json_msg);

  return http_make_response(
    status, "application/json", (uint8_t *) body, NULL, ressz);
}

char *http_make_response(
    int status,
    const char
    *content_type,
    uint8_t *body,
    size_t *bodysz,
    size_t *ressz) {
  const size_t body_size = bodysz == NULL ? strlen((char *) body) : *bodysz;

  char *res = calloc(VLC_BUFSZ, sizeof (char));
  char header[VLC_SMALLBUFSZ] = {0};

  sprintf(header,
    "HTTP/1.1 %d %s\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %zu\r\n"
    "Server: volcanosrv/%s\r\n\r\n",
    status, http_status_str(status),
    content_type,
    body_size,
    VERSION
  );

  size_t hlen = strlen(header);
  *ressz = hlen + body_size;

  for (int i= 0; i < *ressz; i++) {
    res[i] = i < hlen ? header[i] : body[i - hlen];
  }

  return res;
}

const char *http_status_str(int status) {
  switch (status) {
    case 200:
      return "VLC_OK";
    case 400:
      return "Bad Request";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 418:
      return "I'm a teapot";
    default:
      return "";
  }
}

char *handle_request(char *method, char *path, char *body, size_t *ressz) {
  if (strcmp(method, "POST") == 0) {
    return handle_request_post(path, body, ressz);
  } else if (strcmp(method, "GET") == 0) {
    return handle_request_get(path, ressz);
  } else {
    return http_make_response(
      405, "text/plain", (uint8_t *) http_status_str(405), NULL, ressz);
  }
}

char *handle_request_get(char *path, size_t *ressz) {
  vlc_log(VLC_LOG_DEBUG, "Handling GET request\n");
  if (vlc_strmatch(path,
    "/", "/kmap", "/favicon.ico", "/main.css", "/index.js", "/kmap.js",
    "/volcano.js", "/keys.json",
    NULL)) {
    char fpath[VLC_SMALLBUFSZ] = {0};
    if (strcmp(path, "/") == 0) {
      sprintf(fpath, "%s/index.html", cfg->srv_data);

    } else if (vlc_strmatch(path, "/kmap", NULL)) {
      sprintf(fpath, "%s/%s.html", cfg->srv_data, path);

    } else {
      sprintf(fpath, "%s%s", cfg->srv_data, path);
    }

    vlc_log(VLC_LOG_DEBUG, "File request: %s\n", fpath);

    FILE *f = fopen(fpath, "rb");
    if (f == NULL) {
      return http_make_response(
        404, "text/plain", (uint8_t *) http_status_str(404), NULL, ressz);
    }
    // Subtracting some bytes to silence GCC warning about sprintfing
    // potentially larger buffer than dst size
    char fbuf[VLC_BUFSZ-256] = {0};
    size_t readsz = fread(fbuf, sizeof (uint8_t), VLC_BUFSZ-256, f);
    fclose(f);

    return http_make_response(
      200, http_get_mimetype(fpath), (uint8_t *) fbuf, &readsz, ressz);

  } else {
    return http_make_response(
      418, "text/plain", (uint8_t *) http_status_str(418), NULL, ressz);
  }
}

char *handle_request_post(char *path, char *body, size_t *ressz) {
  size_t spathsz = 0;
  char **spath = vlc_strsplit(path, &spathsz, "/");

  vlc_log(VLC_LOG_DEBUG, "Path tokens:\n");
  for (int i = 0; i < spathsz; i++) {
    vlc_log(VLC_LOG_DEBUG, "  %s\n", spath[i]);
  }

  char *response = dispatch_cmd(spath, spathsz, body, ressz);
  vlc_strsplit_free(spath, spathsz);

  return response;
}

bool daemon_send(char *rxbuf, const char *fmt, ...) {
  int s = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, cfg->socket_file);
  connect(s, (struct sockaddr *) &addr, sizeof addr);
  char msg[VLC_SMALLBUFSZ] = {0};

  va_list args;
  va_start(args, fmt);
  vsprintf(msg, fmt, args);
  va_end(args);

  send(s, msg, strlen(msg), 0);

  struct pollfd pfd = { .fd = s, .events = POLLIN };
  int pcnt;
  while ((pcnt = poll(&pfd, 1, 1000)) != 1);
  recv(s, rxbuf, VLC_SMALLBUFSZ, 0);
  close(s);

  return strcmp(rxbuf, "VLC_OK") == 0;
}

static char *dispatch_helper(
  char *rxbuf,
  char **spath,
  size_t spathsz,
  size_t expected_spathsz,
  size_t *ressz) {
  if (spathsz < expected_spathsz-1) {
    char buf[VLC_SMALLBUFSZ] = {0};
    sprintf(buf, "%s command needs %zu arguments.", spath[0], expected_spathsz-1);
    return http_make_api_response(400, buf, ressz);
  }

  char cmdbuf[VLC_SMALLBUFSZ] = {0};
  for (int i = 0; i < spathsz; i++) {
    strcat(cmdbuf, spath[i]);
    if (i < spathsz - 1) {
      strcat(cmdbuf, " ");
    }
  }

  int status = daemon_send(rxbuf, cmdbuf) ? 200 : 400;
  return http_make_api_response(status, rxbuf, ressz);
}

char *dispatch_cmd(char **spath, size_t spathsz, char *body, size_t *ressz) {
  char rxbuf[VLC_SMALLBUFSZ] = {0};

  if (spathsz == 0) {
    return http_make_api_response(400, "Empty command.", ressz);
  }

  if (strcmp(spath[0], "color") == 0) {
    return dispatch_helper(rxbuf, spath, spathsz, 4, ressz);

  } else if (strcmp(spath[0], "kcolor") == 0) {
    return dispatch_helper(rxbuf, spath, spathsz, 5, ressz);

  } else if (vlc_strmatch(spath[0], "rate", "mode", "speed", "brightness", "dir", "rainbow", NULL)) {
    return dispatch_helper(rxbuf, spath, spathsz, 2, ressz);

  } else if (strcmp(spath[0], "kmap") == 0) {
    if (body[0] != '[') {
      return http_make_api_response(400, "Bad kmap format.", ressz);
    }

    char msg[VLC_SMALLBUFSZ] = {0};
    int msgptr = 0;

    for (int i = 0; i < strlen(body); i++) {
      if (isdigit(body[i])) {
        msg[msgptr++] = body[i];
      } else if (body[i] == ',') {
        msg[msgptr++] = ' ';
      }
    }

    int status = daemon_send(rxbuf, "kmap %s", msg) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);
  }

  return http_make_api_response(400, "Unknown command.", ressz);
}
