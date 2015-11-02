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
  const char* expected = "d   4.0KB 1969-12-31 21:00 .         \n"
                         "d   4.0KB 1969-12-31 21:00 ..        \n";
  const unsigned BUFSIZE = FS_LS_FORMAT_SIZE * 2;
  char* buf = calloc(BUFSIZE, sizeof(*buf));
  PASSERT(buf, FS_ERR_MALLOC);

  memset(buf, '\0', BUFSIZE);

  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_ls(fs, "/", buf, BUFSIZE);

  ASSERT(!strcmp(buf, expected), "\n`\n%s`\n != \n`\n%s`\n", buf, expected);

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

  fs_filesystem_t* fs2 = fs_filesystem_create(0);
  fs_filesystem_load(fs2, buf);

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

  fs_filesystem_t* fs2 = fs_filesystem_create(0);
  fs_filesystem_load(fs2, buf);

  ASSERT(fs2->root->children_count == 2, "");
  fs_file_t* f1 = (fs_file_t*)fs2->root->children->data;
  fs_file_t* f2 = (fs_file_t*)fs2->root->children->next->data;

  ASSERT(!strcmp(f1->attrs.fname, "lol.txt"), "");
  ASSERT(!strcmp(f2->attrs.fname, "lol2.txt"), "");

  fs_filesystem_destroy(fs);
  fs_filesystem_destroy(fs2);
  free(buf);
}

void test8()
{
  const char* expected = "d   4.0KB 1969-12-31 21:00 .         \n"
                         "d   4.0KB 1969-12-31 21:00 ..        \n"
                         "f   4.0KB 1969-12-31 21:00 lol.txt   \n"
                         "f   4.0KB 1969-12-31 21:00 hue.txt   \n";
  const unsigned BUFSIZE = FS_LS_FORMAT_SIZE * 4;
  char buf[BUFSIZE] = { 0 };

  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_touch(fs, "hue.txt");
  fs_filesystem_touch(fs, "lol.txt");
  fs_filesystem_ls(fs, "/", buf, BUFSIZE);

  ASSERT(!strcmp(buf, expected), "\n`\n%s`\n != \n`\n%s`\n", buf, expected);

  fs_filesystem_destroy(fs);
}

static void _write_dumb_file(const char* fname, size_t size)
{
  FILE* file = NULL;
  char* buf = calloc(size, sizeof(*buf));

  memset(buf, 0xff, size);

  PASSERT((file = fopen(fname, "w")), "fopen:");
  PASSERT(fwrite(buf, sizeof(*buf), size, file) > 0, "fwrite:");
  PASSERT(fclose(file) == 0, "fclose error:");

  free(buf);
}

void test9()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_touch(fs, "hue.txt");
  fs_filesystem_touch(fs, "lol.txt");

  fs_file_t* file = fs_filesystem_find(fs, "/", "hue.txt");

  ASSERT(file, "file must be found");
  ASSERT(!strcmp(file->attrs.fname, "hue.txt"), "file must be found");
  ASSERT(!fs_filesystem_find(fs, "/", "NOT_FOUND.txt"), "file must be found");

  fs_filesystem_destroy(fs);
}

/* void test10() */
/* { */
/*   const char* FNAME = "test9-file"; */
/*   fs_filesystem_t* fs = fs_filesystem_create(10); */
/*   _write_dumb_file(FNAME, 1 * FS_KILOBYTE); */

/*   fs_utils_fdelete(FS_TEST_FNAME); */
/*   fs_filesystem_mount(fs, FS_TEST_FNAME); */
/*   fs_filesystem_cp(fs, FNAME, FNAME); */

/*   ASSERT(fs_filesystem_find(fs, "/", FNAME), "file must be present"); */

/*   fs_filesystem_destroy(fs); */
/* } */

void test11()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_touch(fs, "hue.txt");
  fs_filesystem_touch(fs, "lol.txt");

  ASSERT(fs_filesystem_find(fs, "/", "hue.txt"), "file is there");
  ASSERT(fs_filesystem_rm(fs, "hue.txt"), "");
  ASSERT(!fs_filesystem_find(fs, "/", "hue.txt"), "file is not there anymore!");

  fs_filesystem_destroy(fs);
}

void test12()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_destroy(fs);

  // 10 is a hint about the number of blocks
  fs_filesystem_t* fs2 = fs_filesystem_create(10);
  fs_filesystem_mount(fs2, FS_TEST_FNAME);

  ASSERT(fs2->blocks_num == 10, "");
  ASSERT(fs2->block_size == 4096, "");

  fs_filesystem_destroy(fs2);
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
  TEST(test8, "ls - root w/ children");
  TEST(test9, "find - check for file in current layer");
  TEST(test11, "rm - remove file");
  TEST(test12, "mount - file persistence");
  /* TEST(test10, "cp - copy from real fs to sim"); */

  return 0;
}
