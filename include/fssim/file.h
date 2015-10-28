#ifndef FSSIM__FILE_H
#define FSSIM__FILE_H

#include "fssim/common.h"
#include "fssim/constants.h"

typedef struct fs_file_attrs_t {
  char fname[FS_NAME_MAX];
  long size;
  long time_creation;
  long time_modification;
  long time_access;
  unsigned char is_directory;
} fs_file_attr_t;

static const struct fs_file_attrs_t fs_zeroed_file_attrs = {0};

typedef struct fs_file_t {
  uint32_t entry;
  char* payload;
  struct fs_file_t* children;
  struct fs_file_t* parent;
  fs_file_attr_t attrs;
} fs_file_t;

fs_file_t* fs_file_create(const char* fname, fs_file_type type,
                          fs_file_t* parent);
void fs_file_destroy(fs_file_t* file);

#endif
