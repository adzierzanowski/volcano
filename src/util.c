#include "util.h"


bool fexists(const char *fname) {
  if (fname == NULL) return false;
  struct stat statbuf;
  return stat(fname, &statbuf) == 0;
}
