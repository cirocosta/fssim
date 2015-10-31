#include "fssim/file.h"

fs_file_t* fs_file_create(const char* fname, fs_file_type type,
                          fs_file_t* parent)

{
  fs_file_t* file = malloc(sizeof(*file));
  PASSERT(file, FS_ERR_MALLOC);

  file->fblock = UINT32_MAX;
  file->payload = NULL;
  file->children = NULL;
  file->children_count = 0;

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

// TODO test this following w/ a proper load()
void fs_file_serialize_dir(fs_file_t* file, unsigned char* buf, int n)
{
  ASSERT(n >= (file->children_count * 32),
         "`buf` must at least have %u bytes remaining",
         file->children_count * 32);

  fs_llist_t* tmp = file->children;
  fs_file_t* curr_file = NULL;
  unsigned counter = 0;
  unsigned offset = 0;

  while (tmp) {
    offset = counter * 32;
    curr_file = (fs_file_t*)tmp->data;

    serialize_uint8_t(buf + offset, curr_file->attrs.is_directory);
    memcpy(buf + offset + 1, curr_file->attrs.fname, 11);
    serialize_uint32_t(buf + offset + 12, curr_file->fblock);
    serialize_int32_t(buf + offset + 16, curr_file->attrs.ctime);
    serialize_int32_t(buf + offset + 20, curr_file->attrs.mtime);
    serialize_int32_t(buf + offset + 24, curr_file->attrs.atime);
    serialize_uint32_t(buf + offset + 28, curr_file->attrs.size);

    tmp = tmp->next;
  }
}

void fs_file_addchild(fs_file_t* dir, fs_file_t* other)
{
  ASSERT(dir->attrs.is_directory == 1,
         "File must be of type FS_FILE_DIRECTORY");
  other->parent = dir;

  if (!dir->children) {
    dir->children = fs_llist_create(other);
  } else {
    dir->children = fs_llist_append(fs_llist_create(other), dir->children);
  }

  dir->children_count++;
}
