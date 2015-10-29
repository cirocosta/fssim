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
  fs_filesystem_t* fs = fs_filesystem_create(10); // 40 KB

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
  const char* expected = "d 0 TEMPO .\n"
                         "d 0 TEMPO ..\n";
  char* buf = calloc(strlen(expected) + 1, sizeof(*buf));
  PASSERT(buf, FS_ERR_MALLOC);

  memset(buf, '\0', strlen(expected) + 1);

  fs_filesystem_t* fs = fs_filesystem_create(10); // 40 KB
  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_ls(fs, "/", buf, strlen(expected) + 1);

  ASSERT(!strcmp(buf, expected), "%s != %s", buf, expected);

  fs_filesystem_destroy(fs);
  free(buf);
}

void test4()
{
  fs_filesystem_t* fs = fs_filesystem_create(10); // 40 KB

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
  fs_filesystem_t* fs = fs_filesystem_create(666); // 40 KB
  unsigned char* buf = calloc(8, sizeof(*buf));

  PASSERT(buf, FS_ERR_MALLOC);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_superblock_2s(fs, buf, 8);

  ASSERT(deserialize_uint32_t(buf) == 4096, "");
  ASSERT(deserialize_uint32_t(buf + 4) == 666, "");

  fs_filesystem_destroy(fs);
  free(buf);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "mounting w/out previous mount");
  TEST(test3, "ls - list empty root dir");
  TEST(test4, "touch - creating a file in empty root");
  TEST(test5, "superblock serialization");

  return 0;
}
