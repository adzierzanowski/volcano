#ifndef VLC_UTIL_H
#define VLC_UTIL_H

#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>


// Returns `true` if the file named `fname` exists, `false` otherwise
bool vlc_fexists(const char *fname);


#endif
