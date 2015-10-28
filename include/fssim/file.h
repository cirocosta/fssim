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
} fs_file_attr_t;

typedef struct fs_file_t {
  unsigned char is_directory;
  uint32_t entry;
  char* payload;
  struct fs_file_t* children; 
  struct fs_file_t* parent;
} fs_file_t;


#endif
