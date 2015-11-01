#include "fssim/common.h"
#include "fssim/filesystem.h"

#define FS_TEST_FNAME "/tmp/test-fssim"

void test1()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);

  fs_filesystem_destroy(fs);
}

void test2()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);

  fs_utils_fdelete(FS_TEST_FNAME);
  ASSERT(fs_utils_fexists(FS_TEST_FNAME) == 0, "No previous file");

  fs_filesystem_mount(fs, FS_TEST_FNAME);

  ASSERT(fs_utils_fexists(FS_TEST_FNAME) == 1,
         "must create a file that represents the FS");
  // TODO check the size as well??

  fs_filesystem_destroy(fs);
}

void test3()
{
  //
  // is_dir size last_modified name
  // TODO change TEMPO for a real time
  // TODO change 0 size to real size
  const char* expected = "d 1 1969-12-31 21:00 .\n"
                         "d 1 1969-12-31 21:00 ..\n";
  char* buf = calloc(strlen(expected) + 1, sizeof(*buf));
  PASSERT(buf, FS_ERR_MALLOC);

  memset(buf, '\0', strlen(expected) + 1);

  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_ls(fs, "/", buf, strlen(expected) + 1);

  ASSERT(!strcmp(buf, expected), "%s != %s", buf, expected);

  fs_filesystem_destroy(fs);
  free(buf);
}

void test4()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_touch(fs, "file.txt");

  ASSERT(
      !strcmp(((fs_file_t*)fs->root->children->data)->attrs.fname, "file.txt"),
      "Child file must have the correct filename");

  fs_filesystem_destroy(fs);
}

void test5()
{
  fs_filesystem_t* fs = fs_filesystem_create(666);
  unsigned char* buf = calloc(8, sizeof(*buf));

  PASSERT(buf, FS_ERR_MALLOC);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_serialize_superblock(fs, buf, 8);

  ASSERT(deserialize_uint32_t(buf) == 4096, "");    // block size
  ASSERT(deserialize_uint32_t(buf + 4) == 666, ""); // blocks_num

  fs_filesystem_destroy(fs);
  free(buf);
}

void test6()
{
  fs_filesystem_t* fs = fs_filesystem_create(16);
  unsigned char* buf = calloc(512, sizeof(*buf));

  PASSERT(buf, FS_ERR_MALLOC);

  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_serialize(fs, buf, 512);
  fs_filesystem_t* fs2 = fs_filesystem_load(buf);

  ASSERT(fs2->blocks_num == fs->blocks_num, "%lu != %lu", fs2->blocks_num,
         fs->blocks_num);
  ASSERT(fs2->block_size == fs->block_size, "");
  ASSERT(fs2->root->attrs.is_directory == 1, "");
  ASSERT(fs2->root->fblock == fs->root->fblock, "");
  ASSERT(fs2->root->children_count == fs->root->children_count, "");

  // TODO we need a way of deciding during the `file_load' time that
  //      we're dealing with the root directory
  /* ASSERT(!strcmp(fs2->root->attrs.fname, fs->root->attrs.fname), ""); */

  fs_filesystem_destroy(fs);
  fs_filesystem_destroy(fs2);
  free(buf);
}

void test7()
{
  fs_filesystem_t* fs = fs_filesystem_create(16);
  unsigned char* buf = calloc(512, sizeof(*buf));

  PASSERT(buf, FS_ERR_MALLOC);

  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_touch(fs, "lol.txt");
  fs_filesystem_touch(fs, "lol2.txt");

  fs_filesystem_serialize(fs, buf, 512);
  fs_filesystem_t* fs2 = fs_filesystem_load(buf);

  ASSERT(fs2->root->children_count == 2, "");
  fs_file_t* f1 = (fs_file_t*)fs2->root->children->data; 
  fs_file_t* f2 = (fs_file_t*)fs2->root->children->next->data; 

  ASSERT(!strcmp(f1->attrs.fname, "lol.txt"), "");
  ASSERT(!strcmp(f2->attrs.fname, "lol2.txt"), "");

  fs_filesystem_destroy(fs);
  fs_filesystem_destroy(fs2);
  free(buf);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "mounting w/out previous mount");
  TEST(test3, "ls - list empty root dir");
  TEST(test4, "touch - creating a file in empty root");
  TEST(test5, "superblock serialization");
  TEST(test6, "full load w/ only root directory");
  TEST(test7, "full load w/ root + files");

  return 0;
}
