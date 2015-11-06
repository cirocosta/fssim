#include "fssim/file.h"

fs_file_t* fs_file_create(const char* fname, fs_file_type type,
                          fs_file_t* parent)

{
  fs_file_t* file = malloc(sizeof(*file));
  PASSERT(file, FS_ERR_MALLOC);

  file->children = NULL;
  file->children_count = 0;

  // dealing w/ root case
  file->fblock = !parent ? 0 : UINT32_MAX;
  file->parent = !parent ? file : parent;

  file->attrs = fs_zeroed_file_attrs;
  file->attrs.size = FS_BLOCK_SIZE;
  strncpy(file->attrs.fname, fname, FS_NAME_MAX);
  file->attrs.is_directory = type == FS_FILE_DIRECTORY ? 1 : 0;

  return file;
}

static void _remove_dir_content(fs_llist_t* d)
{
  fs_llist_t* td = NULL;
  fs_file_t* f = NULL;

  while (d) {
    f = (fs_file_t*)d->data;

    if (f->attrs.is_directory && f->children)
      _remove_dir_content(f->children);

    td = d;
    d = d->next;
    td->next = NULL;
    fs_llist_destroy(td, NULL);
    free(f);
  }
}

void fs_file_destroy(fs_file_t* file)
{
  if (file->children)
    _remove_dir_content(file->children);
  free(file);
}

int fs_file_serialize_dir(fs_file_t* file, unsigned char* buf, int n)
{
  int to_write = 32 + file->children_count * 32;

  ASSERT(n >= to_write, "`buf` must at least have %u bytes remaining. Has %d",
         to_write, n);

  fs_llist_t* tmp = file->children;
  fs_file_t* curr_file = NULL;
  unsigned counter = 0;
  unsigned offset = 0;

  serialize_uint8_t(buf, file->children_count);
  counter++;

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
    counter++;
  }

  return to_write;
}

void fs_file_load_dir(fs_file_t* file, unsigned char* buf)
{
  unsigned offset = 0;
  unsigned counter = 1;
  unsigned children_count = deserialize_uint8_t(buf);

  while (counter <= children_count) {
    fs_file_t* new_file = malloc(sizeof(*new_file));
    PASSERT(new_file, FS_ERR_MALLOC);
    *new_file = fs_zeroed_file;
    new_file->attrs = fs_zeroed_file_attrs;

    offset = counter * 32;

    new_file->attrs.is_directory = deserialize_uint8_t(buf + offset);
    memcpy(new_file->attrs.fname, buf + offset + 1, 11);
    new_file->fblock = deserialize_uint32_t(buf + offset + 12);
    new_file->attrs.ctime = deserialize_int32_t(buf + offset + 16);
    new_file->attrs.mtime = deserialize_int32_t(buf + offset + 20);
    new_file->attrs.atime = deserialize_int32_t(buf + offset + 24);
    new_file->attrs.size = deserialize_uint32_t(buf + offset + 28);

    fs_file_addchild(file, new_file);

    counter++;
  }
}

void fs_file_addchild(fs_file_t* dir, fs_file_t* other)
{
  ASSERT(dir->attrs.is_directory == 1,
         "File must be of type FS_FILE_DIRECTORY");
  other->parent = dir;

  if (!dir->children)
    dir->children = fs_llist_create(other);
  else
    dir->children = fs_llist_append(fs_llist_create(other), dir->children);

  dir->children_count++;
}
