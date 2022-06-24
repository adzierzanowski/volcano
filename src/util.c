#include "util.h"


bool vlc_fexists(const char *fname) {
  if (fname == NULL) return false;
  struct stat statbuf;
  return stat(fname, &statbuf) == 0;
}
