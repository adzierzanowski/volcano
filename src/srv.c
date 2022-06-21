#include "srv.h"


static const char *srv_data;
static uint16_t srv_port;
static const char *srv_socket;


static void srv_on_exit() {
  dlog(LOG_INFO, "Quitting volcano server.\n");
}

static void sigint_handler(int arg) {
  exit(0);
}


int main(int argc, const char *argv[]) {
  set_loglevel(LOG_DEBUG);

  for (int i = 0; i < argc; i++) {
    dlog(LOG_DEBUG, "argv[%d]=%s\n", i, argv[i]);
  }

  if (argc < 4) {
    dlog(
      LOG_ERROR,
      "Volcano server needs 3 arguments: PORT, DATA_PATH, SOCKET_PATH.\n");
    exit(1);
  }

  srv_port = atoi(argv[1]);
  srv_data = argv[2];
  srv_socket = argv[3];

  dlog(LOG_INFO, "volcanosrv version %s.\n", VERSION);
  dlog(LOG_INFO, "Starting volcano server at 0.0.0.0:%hu.\n", srv_port);
  dlog(LOG_DEBUG, "Server assets path: %s\n", srv_data);
  signal(SIGINT, sigint_handler);
  atexit(srv_on_exit);

  int s = socket(AF_INET, SOCK_STREAM, 0);
  int one = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(srv_port);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(s, (struct sockaddr *) &addr, sizeof addr);
  listen(s, 5);

  for (;;) {
    int sock = accept(s, NULL, NULL);
    char buf[BUFSZ] = {0};
    recv(sock, buf, BUFSZ, 0);
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

    dlog(LOG_INFO, "%s %s\n", method, path);

    char *header = strtok(NULL, "\n");
    while (strcmp(header, "\r")) {
      dlog(LOG_DEBUG, "[Header] %s\n", header);
      header = strtok(NULL, "\n");
    }

    char *body = strtok(NULL, "");
    dlog(LOG_DEBUG, "[Body] %s\n", body);

    size_t response_size = 0;
    char *response = handle_request(method, path, body, &response_size);

    dlog(LOG_INFO, "Sending response:\n\n%s\n\n", response);
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
  char ppath[SMALLBUFSZ] = {0};
  sprintf(ppath, "%s", path);
  char **spath = strsplit(ppath, &spathsz, ".");

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

  strsplit_free(spath, spathsz);

  return mime;
}

char *http_make_api_response(int status, const char *json_msg, size_t *ressz) {
  char body[SMALLBUFSZ] = {0};
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

  char *res = calloc(BUFSZ, sizeof (char));
  char header[SMALLBUFSZ] = {0};

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
      return "OK";
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
  dlog(LOG_DEBUG, "Handling GET request\n");
  if (strmatch(path,
    "/", "/kmap", "/favicon.ico", "/main.css", "/index.js", "/kmap.js",
    "/volcano.js", "/keys.json",
    NULL)) {
    char fpath[SMALLBUFSZ] = {0};
    if (strcmp(path, "/") == 0) {
      sprintf(fpath, "%s/index.html", srv_data);

    } else if (strmatch(path, "/kmap", NULL)) {
      sprintf(fpath, "%s/%s.html", srv_data, path);

    } else {
      sprintf(fpath, "%s%s", srv_data, path);
    }

    dlog(LOG_DEBUG, "File request: %s\n", fpath);

    FILE *f = fopen(fpath, "rb");
    if (f == NULL) {
      return http_make_response(
        404, "text/plain", (uint8_t *) http_status_str(404), NULL, ressz);
    }
    // Subtracting some bytes to silence GCC warning about sprintfing
    // potentially larger buffer than dst size
    char fbuf[BUFSZ-256] = {0};
    size_t readsz = fread(fbuf, sizeof (uint8_t), BUFSZ-256, f);
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
  char **spath = strsplit(path, &spathsz, "/");

  dlog(LOG_DEBUG, "Path tokens:\n");
  for (int i = 0; i < spathsz; i++) {
    dlog(LOG_DEBUG, "  %s\n", spath[i]);
  }

  char *response = dispatch_cmd(spath, spathsz, body, ressz);
  strsplit_free(spath, spathsz);

  return response;
}

bool daemon_send(char *rxbuf, const char *fmt, ...) {
  int s = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, srv_socket);
  connect(s, (struct sockaddr *) &addr, sizeof addr);
  char msg[SMALLBUFSZ] = {0};

  va_list args;
  va_start(args, fmt);
  vsprintf(msg, fmt, args);
  va_end(args);

  send(s, msg, strlen(msg), 0);

  struct pollfd pfd = { .fd = s, .events = POLLIN };
  int pcnt;
  while ((pcnt = poll(&pfd, 1, 1000)) != 1);
  recv(s, rxbuf, SMALLBUFSZ, 0);
  close(s);

  return strcmp(rxbuf, "OK") == 0;
}

char *dispatch_cmd(char **spath, size_t spathsz, char *body, size_t *ressz) {
  char rxbuf[SMALLBUFSZ] = {0};

  if (spathsz == 0) {
    return http_make_api_response(400, "Empty command.", ressz);
  }

  if (strcmp(spath[0], "color") == 0) {
    if (spathsz != 4) {
      return http_make_api_response(
        400, "color command needs 3 arguments.", ressz);
    }

    int r = atoi(spath[1]);
    int g = atoi(spath[2]);
    int b = atoi(spath[3]);
    int status = daemon_send(rxbuf, "color %d %d %d", r, g, b) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "kcolor") == 0) {
    if (spathsz != 5) {
      return http_make_api_response(
        400, "color command needs 4 arguments.", ressz);
    }

    char *key = spath[1];
    int r = atoi(spath[2]);
    int g = atoi(spath[3]);
    int b = atoi(spath[4]);

    int status = daemon_send(rxbuf, "kcolor %s %d %d %d", key, r, g, b)
      ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "mode") == 0) {
    if (spathsz != 2) {
      return http_make_api_response(
        400, "mode command needs exactly one argument.", ressz);
    }

    int status = daemon_send(rxbuf, "mode %s", spath[1]) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "speed") == 0) {
    if (spathsz != 2) {
      return http_make_api_response(
        400, "speed command needs exactly one argument.", ressz);
    }

    int level = atoi(spath[1]);
    int status = daemon_send(rxbuf, "speed %d", level) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "brightness") == 0) {
    if (spathsz != 2) {
      return http_make_api_response(
        400, "brightness command needs exactly one argument.", ressz);
    }

    int level = atoi(spath[1]);
    int status = daemon_send(rxbuf, "brightness %d", level) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "dir") == 0) {
    if (spathsz != 2) {
      return http_make_api_response(
        400, "dir command needs exactly one argument.", ressz);
    }

    int dir = atoi(spath[1]);
    int status = daemon_send(rxbuf, "dir %d", dir) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "rainbow") == 0) {
    if (spathsz != 2) {
      return http_make_api_response(
        400, "rainbow command needs exactly one argument.", ressz);
    }

    int status = daemon_send(rxbuf, "rainbow %s", spath[1]) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "rate") == 0) {
    if (spathsz != 2) {
      return http_make_api_response(
        400, "rate command needs exactly 2 arguments.", ressz);
    }

    int rate = atoi(spath[1]);
    int status = daemon_send(rxbuf, "rate %d", rate) ? 200 : 400;
    return http_make_api_response(status, rxbuf, ressz);

  } else if (strcmp(spath[0], "kmap") == 0) {
    if (body[0] != '[') {
      return http_make_api_response(400, "Bad kmap format.", ressz);
    }

    char msg[SMALLBUFSZ] = {0};
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
