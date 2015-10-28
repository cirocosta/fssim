#include "fssim/file.h"

fs_file_t* fs_file_create(const char* fname, fs_file_type type,
                          fs_file_t* parent)

{
  fs_file_t* file = malloc(sizeof(*file));
  PASSERT(file, FS_ERR_MALLOC);

  file->entry = UINT32_MAX;
  file->payload = NULL;
  file->children = NULL;

  // dealing w/ root case
  file->parent = parent == NULL ? file : parent;

  file->attrs = fs_zeroed_file_attrs;

  strncpy(file->attrs.fname, fname, FS_NAME_MAX);
  file->attrs.is_directory = type == FS_FILE_DIRECTORY ? 1 : 0;

  return file;
}

void fs_file_destroy(fs_file_t* file)
{
  // TODO traverse the tree and free whatever is bellow?
  if (file->children) 
    fs_llist_destroy(file->children, fs_file_destructor);
  free(file);
}

void fs_file_addchild(fs_file_t* dir, fs_file_t* other)
{
  other->parent = dir;

  if (!dir->children) {
    dir->children = fs_llist_create(other);
  } else {
    dir->children = fs_llist_append(fs_llist_create(other), dir->children);
  }
}
