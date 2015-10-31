#ifndef FSSIM__FILE_H
#define FSSIM__FILE_H

#include "fssim/common.h"
#include "fssim/constants.h"
#include "fssim/llist.h"
#include "fssim/file_utils.h"

typedef struct fs_file_attrs_t {
  uint8_t is_directory;
  char fname[FS_NAME_MAX];
  int32_t ctime;
  int32_t mtime;
  int32_t atime;
  uint32_t size;
} fs_file_attr_t;

static const struct fs_file_attrs_t fs_zeroed_file_attrs = { 0 };

typedef struct fs_file_t {
  uint32_t fblock;
  uint8_t* payload;
  fs_file_attr_t attrs;

  struct fs_file_t* parent;
  fs_llist_t* children;
  uint8_t children_count;
} fs_file_t;

// TODO remove parent arg
//      identify root with FILE_TYPE, which makes
//      much more sense.
fs_file_t* fs_file_create(const char* fname, fs_file_type type,
                          fs_file_t* parent);
void fs_file_destroy(fs_file_t* file);
void fs_file_addchild(fs_file_t* dir, fs_file_t* other);

inline static void fs_file_destructor(void* data)
{
  fs_file_destroy((fs_file_t*)data);
}

#endif
