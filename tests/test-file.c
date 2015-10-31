#include "fssim/common.h"
#include "fssim/file.h"

void test1()
{
  fs_file_t* file = fs_file_create("/", FS_FILE_DIRECTORY, NULL);

  ASSERT(file->attrs.is_directory == 1, "");
  ASSERT(!strcmp(file->attrs.fname, "/"), "");
  ASSERT(file->parent == file, "");
  // TODO maybe this should actually be an empty linked list...
  ASSERT(file->children == NULL, "");
  ASSERT(file->children_count == 0, "");

  fs_file_destroy(file);
}

void test2()
{
  fs_file_t* dir = fs_file_create("/", FS_FILE_DIRECTORY, NULL);
  fs_file_t* file = fs_file_create("hue.scm", FS_FILE_REGULAR, NULL);
  fs_file_t* file2 = fs_file_create("other.scm", FS_FILE_REGULAR, NULL);

  fs_file_addchild(dir, file);

  ASSERT(dir->children != NULL, "");
  ASSERT(dir->children->data == file, "");

  fs_file_addchild(dir, file2);
  ASSERT(dir->children->data == file2, "");
  ASSERT(dir->children->next->data == file, "");
  ASSERT(dir->children_count == 2, "");

  fs_file_destroy(dir);
}

void test3()
{
  fs_file_t* dir = fs_file_create("/", FS_FILE_DIRECTORY, NULL);
  unsigned char* buf = calloc(512, sizeof(*buf));
  PASSERT(buf, FS_ERR_MALLOC);

  fs_file_serialize_dir(dir, buf, 512);
  fs_file_t* dir2 = fs_file_load(buf);

  ASSERT(dir2->children_count == 0, "");

  fs_file_destroy(dir);
  fs_file_destroy(dir2);
  free(buf);
}

void test4()
{
  fs_file_t* dir = fs_file_create("/", FS_FILE_DIRECTORY, NULL);
  fs_file_t* file = fs_file_create("hue.scm", FS_FILE_REGULAR, NULL);
  fs_file_t* file2 = fs_file_create("other.scm", FS_FILE_REGULAR, NULL);

  unsigned char* buf = calloc(512, sizeof(*buf));
  PASSERT(buf, FS_ERR_MALLOC);

  fs_file_addchild(dir, file);
  fs_file_addchild(dir, file2);

  fs_file_serialize_dir(dir, buf, 512);
  fs_file_t* dir2 = fs_file_load(buf);

  ASSERT(dir2->children_count == 2, "");
  ASSERT(dir2->children != NULL, "");
  fs_file_t* dir2_child1 = (fs_file_t*)dir2->children->data;
  fs_file_t* dir2_child2 = (fs_file_t*)dir2->children->next->data;

  ASSERT(!strcmp(dir2_child1->attrs.fname, "hue.scm"), "");
  ASSERT(dir2_child1->attrs.is_directory == 0, "");

  ASSERT(!strcmp(dir2_child2->attrs.fname, "other.scm"), "");
  ASSERT(dir2_child2->attrs.is_directory == 0, "");


  fs_file_destroy(dir);
  fs_file_destroy(dir2);
  free(buf);
}

int main(int argc, char* argv[])
{
  TEST(test1, "directory file - creation and deletion");
  TEST(test2, "directory file - addchild");
  TEST(test3, "directory file - (de)serialization - only root");
  TEST(test4, "directory file - (de)serialization - flat dir w/ files");

  return 0;
}
