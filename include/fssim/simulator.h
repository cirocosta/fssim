#ifndef FSSIM__SIMULATOR_H
#define FSSIM__SIMULATOR_H

#include "fssim/common.h"
#include "fssim/filesystem.h"

#include <linux/limits.h>

typedef struct fs_simulator_t {
  fs_filesystem_t* fs;
  char mounted_at[PATH_MAX]; 
} fs_simulator_t;

fs_simulator_t* fs_simulator_create();
void fs_simulator_destroy(fs_simulator_t* sim);

#endif
