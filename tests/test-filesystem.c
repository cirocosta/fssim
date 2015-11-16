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
  fs_filesystem_touch(fs, "/file.txt");

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
  fs_filesystem_touch(fs, "/hue.txt");
  fs_filesystem_touch(fs, "/lol.txt");
  fs_filesystem_ls(fs, "/", buf, BUFSIZE);

  ASSERT(!strcmp(buf, expected), "\n`\n%s`\n != \n`\n%s`\n", buf, expected);

  fs_filesystem_destroy(fs);
}

void test9()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_touch(fs, "/hue.txt");
  fs_filesystem_touch(fs, "/lol.txt");

  fs_file_t* file = fs_filesystem_find(fs, "/", "hue.txt");

  ASSERT(file, "file must be found");
  ASSERT(!strcmp(file->attrs.fname, "hue.txt"), "actually found %s",
         file->attrs.fname);
  ASSERT(!fs_filesystem_find(fs, "/", "NOT_FOUND.txt"), "file must be found");

  fs_filesystem_destroy(fs);
}

void test10()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_mkdir(fs, "/tmp");
  fs_file_t* tmp_dir = fs_filesystem_find(fs, "/", "tmp");
  fs_file_t* new_file = fs_file_create("fffu", FS_FILE_REGULAR, tmp_dir);
  fs_file_addchild(fs->cwd, new_file);

  ASSERT(fs_filesystem_find(fs, "/", "fffu") != NULL, "");
  ASSERT(!strcmp(fs_filesystem_find(fs, "/", "fffu")->attrs.fname, "fffu"), "");

  fs_filesystem_destroy(fs);
}

void test11()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_touch(fs, "/hue.txt");
  fs_filesystem_touch(fs, "/lol.txt");

  ASSERT(fs_filesystem_find(fs, "/", "hue.txt"), "file is there");
  ASSERT(fs_filesystem_rm(fs, "/hue.txt"), "");
  ASSERT(!fs_filesystem_find(fs, "/", "hue.txt"), "file is not there anymore!");

  fs_filesystem_destroy(fs);
}

void test12()
{
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);

  fs_filesystem_mount(fs, FS_TEST_FNAME);

  ASSERT(!fs_filesystem_find(fs, "/", "hue.txt"), "file is not there!");

  fs_filesystem_touch(fs, "/hue.txt");

  ASSERT(fs_filesystem_find(fs, "/", "hue.txt"), "file is there");
  ASSERT(fs_filesystem_rm(fs, "/hue.txt"), "");
  ASSERT(fs->root->children_count == 0, "empty root!");
  ASSERT(!fs_filesystem_find(fs, "/", "hue.txt"), "file is not there anymore!");

  fs_filesystem_destroy(fs);
}

void test13()
{
  // fs
  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  ASSERT(fs->root->children_count == 0, "");
  fs_filesystem_destroy(fs);
  fs = NULL;

  // fs2
  fs_filesystem_t* fs2 = fs_filesystem_create(0);
  fs_filesystem_mount(fs2, FS_TEST_FNAME);

  ASSERT(fs2->blocks_num == 10, "");
  ASSERT(fs2->block_size == 4096, "");
  ASSERT(fs_filesystem_find(fs2, "/", "hue.txt") == NULL, "file is not there");
  ASSERT(fs2->root->children_count == 0, "");

  fs_filesystem_touch(fs2, "/hue.txt");

  ASSERT(fs2->root->children_count == 1, "");
  ASSERT(fs_filesystem_find(fs2, "/", "hue.txt"), "now the file is :D");

  fs_filesystem_destroy(fs2);
  fs2 = NULL;

  // fs
  fs = fs_filesystem_create(0);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  ASSERT(fs->blocks_num == 10, "");
  ASSERT(fs->block_size == 4096, "");

  ASSERT(fs->root->children_count == 1, "");
  ASSERT(fs_filesystem_find(fs, "/", "hue.txt"), "file correctly retained");

  ASSERT(fs_filesystem_rm(fs, "/hue.txt"), "file removed properly");
  ASSERT(!fs_filesystem_find(fs, "/", "hue.txt"),
         "the file isn't there anymore!");
  fs_filesystem_destroy(fs);
  fs = NULL;

  // fs2
  fs2 = fs_filesystem_create(0);
  fs_filesystem_mount(fs2, FS_TEST_FNAME);

  ASSERT(fs2->root->children_count == 0, "");
  ASSERT(!fs_filesystem_find(fs2, "/", "hue.txt"),
         "last added file correctly not there anymore");

  fs_filesystem_destroy(fs2);
  fs2 = NULL;
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

void test14()
{
  fs_file_t* file = NULL;
  const char* FNAME = "test14f";
  const char* FNAME_FS = "/test14f";
  fs_filesystem_t* fs = fs_filesystem_create(10);
  _write_dumb_file(FNAME, 1 * FS_KILOBYTE);

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_cp(fs, FNAME, FNAME_FS);

  ASSERT((file = fs_filesystem_find(fs, "/", FNAME)), "file must be present");
  ASSERT(file->fblock > 0, "0 reserved to root");
  ASSERT(fs->fat->blocks[file->fblock] == file->fblock,
         "single block - small file");

  // flawled test

  fs_filesystem_destroy(fs);
}

void test15()
{
  fs_file_t* file = NULL;
  const char* FNAME = "test15-f";
  const char* FNAME_FS = "/test15-f";
  fs_filesystem_t* fs = fs_filesystem_create(300); // 300 blocks
  _write_dumb_file(FNAME, 1 * FS_MEGABYTE);
  int blocks = 0;

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_cp(fs, FNAME, FNAME_FS);

  ASSERT((file = fs_filesystem_find(fs, "/", FNAME)), "file must be present");
  ASSERT(file->fblock > 0, "0 reserved to root");
  ASSERT(fs->fat->blocks[file->fblock] != file->fblock,
         "more than one block to the file");
  ASSERT(file->attrs.size == 1 * FS_MEGABYTE, "actual: %d", file->attrs.size);

  for (int block = file->fblock;;) {
    blocks++;

    if (fs->fat->blocks[block] == block)
      break;

    block = fs->fat->blocks[block];
  }
  ASSERT(blocks == 256, "actually has %d", blocks);

  fs_filesystem_destroy(fs);
  fs = NULL;
  // close the file

  // re-open
  fs = fs_filesystem_create(0);
  fs_filesystem_mount(fs, FS_TEST_FNAME);

  ASSERT(fs->root->children_count == 1, "");
  ASSERT((file = fs_filesystem_find(fs, "/", FNAME)), "file must be present");
  ASSERT(file->attrs.size == 1 * FS_MEGABYTE, "actual: %d", file->attrs.size);
  ASSERT(file->fblock > 0, "0 reserved to root");
  ASSERT(fs->fat->blocks[file->fblock] != file->fblock,
         "more than one block to the file");

  fs_filesystem_destroy(fs);
}

void test16()
{
  const char* FNAME_IN = "17-in";
  const char* FNAME_IN_FS = "/17-in";
  const char* FNAME_OUT = "17-out";
  uint8_t block_buf[FS_BLOCK_SIZE];
  int blocks = 1;
  FILE* fout = NULL;
  fs_filesystem_t* fs = fs_filesystem_create(300); // 300 blocks

  _write_dumb_file(FNAME_IN, 1 * FS_MEGABYTE);

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_cp(fs, FNAME_IN, FNAME_IN_FS);

  PASSERT((fout = fopen(FNAME_OUT, "w+b")), "");
  fs_filesystem_cat(fs, FNAME_IN_FS, fileno(fout));
  PASSERT(fclose(fout) == 0, "fclose:");

  PASSERT((fout = fopen(FNAME_OUT, "rb")), "");
  int i = 1 * FS_MEGABYTE;
  int res = 0xff;
  while (i > 0) {
    fread(block_buf, sizeof(uint8_t), 1024, fout);
    for (int i = 0; i < 1024; i++)
      res &= block_buf[i];

    i -= 1024;
  }
  ASSERT(res == 0xff, "");

  fs_filesystem_destroy(fs);
  fclose(fout);
}

void test17()
{
  fs_filesystem_t* fs = fs_filesystem_create(300); // 300 blocks
  fs_file_t* tmp_dir = NULL;

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);

  ASSERT(fs->root->children_count == 0, "");
  fs_filesystem_mkdir(fs, "/tmp");
  ASSERT(fs->root->children_count == 1, "");

  tmp_dir = (fs_file_t*)fs->root->children->data;
  ASSERT(tmp_dir->attrs.is_directory == 1, "");
  ASSERT(tmp_dir->attrs.size == 4096, "");
  ASSERT(!strcmp(tmp_dir->attrs.fname, "tmp"), "%s is not the expected",
         tmp_dir->attrs.fname);
  fs_filesystem_destroy(fs);
  fs = NULL;
  tmp_dir = NULL;

  fs = fs_filesystem_create(0);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  ASSERT(fs->root->children_count == 1, "");

  tmp_dir = (fs_file_t*)fs->root->children->data;
  ASSERT(!strcmp(tmp_dir->attrs.fname, "tmp"), "%s is not the expected",
         tmp_dir->attrs.fname);

  fs_filesystem_destroy(fs);
}

void test18()
{
  fs_filesystem_t* fs = fs_filesystem_create(300); // 300 blocks
  fs_file_t* tmp_dir = NULL;

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_mkdir(fs, "/tmp");
  fs_file_t* file_hue = fs_filesystem_touch(fs, "/tmp/hue.br");

  ASSERT(!strcmp(file_hue->parent->attrs.fname, "tmp"), "actually: %s",
         file_hue->parent->attrs.fname);

  tmp_dir = fs_filesystem_find(fs, "/", "tmp");
  ASSERT(tmp_dir != NULL, "");
  ASSERT(!strcmp(tmp_dir->attrs.fname, "tmp"), "");
  ASSERT(tmp_dir->children_count == 1, "");
  ASSERT(
      !strcmp(((fs_file_t*)(tmp_dir->children->data))->attrs.fname, "hue.br"),
      "");

  ASSERT(fs_filesystem_find(fs, "/tmp", "hue.br"), "");

  fs_filesystem_destroy(fs);
  fs = NULL;
  file_hue = NULL;

  fs = fs_filesystem_create(0);
  fs_filesystem_mount(fs, FS_TEST_FNAME);

  ASSERT(fs->root->children_count == 1, "");
  ASSERT((tmp_dir = fs_filesystem_find(fs, "/", "tmp")), "");
  ASSERT(tmp_dir->children_count == 1, "");
  ASSERT(fs_filesystem_find(fs, "/tmp", "hue.br"), "");

  fs_filesystem_destroy(fs);
}

void test19()
{
  fs_filesystem_t* fs = fs_filesystem_create(300); // 300 blocks

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);

  //         _______root______
  //         |       |       |
  //        hue     lol     tmp
  //                        / \
  //                      hue  br
  fs_filesystem_mkdir(fs, "/lol");
  fs_filesystem_mkdir(fs, "/hue");
  fs_filesystem_mkdir(fs, "/tmp");
  fs_filesystem_mkdir(fs, "/tmp/hue");
  fs_filesystem_mkdir(fs, "/tmp/br");

  ASSERT(fs_filesystem_find(fs, "/", "tmp") != NULL, "");
  ASSERT(fs->root->children_count == 3, "");
  ASSERT(fs_filesystem_rmdir(fs, "/lol"), "");
  ASSERT(fs->root->children_count == 2, "");

  /* fs_file_t* tmp_dir = fs_filesystem_find(fs, "/", "tmp"); */
  /* ASSERT(tmp_dir->children_count == 2, ""); */
  fs_filesystem_rmdir(fs, "/tmp");
  /* ASSERT(fs->root->children_count == 1, ""); */
  /* ASSERT(fs_filesystem_find(fs, "/", "tmp") == NULL, ""); */
  /* ASSERT(fs_filesystem_find(fs, "/", "br") == NULL, ""); */

  fs_filesystem_destroy(fs);
}

void test20()
{
  const char* expected = "Files:              0\n"
                         "Directories:        4\n"
                         "Free Space:     384.0KB\n"
                         "Wasted Space:     0.0 B\n";
  char buf[FS_DF_FORMAT_SIZE] = { 0 };
  // 100 blocks ==> 4KB * 100 ==> 400KB.
  fs_filesystem_t* fs = fs_filesystem_create(100);

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_mkdir(fs, "/lol");
  fs_filesystem_mkdir(fs, "/tmp");
  fs_filesystem_mkdir(fs, "/tmp/hue");
  fs_filesystem_mkdir(fs, "/tmp/br");

  fs_filesystem_df(fs, buf, FS_DF_FORMAT_SIZE);

  ASSERT(!strcmp(buf, expected), "`\n%s\n` != `\n%s\n`", buf, expected);

  fs_filesystem_destroy(fs);
}

void test21()
{
  fs_file_t* file = NULL;
  const char* FNAME = "test21f";
  const char* FNAME_FS = "/test21f";
  fs_filesystem_t* fs = fs_filesystem_create(10);
  _write_dumb_file(FNAME, 1 * FS_KILOBYTE);

  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_cp(fs, FNAME, FNAME_FS);
  fs_filesystem_mkdir(fs, "/lol");

  ASSERT((file = fs_filesystem_find(fs, "/", FNAME)), "file must be present");
  ASSERT(file->attrs.size == 1 * FS_KILOBYTE, "has the correct size");

  fs_filesystem_destroy(fs);

  fs = NULL;
  fs = fs_filesystem_create(0);
  fs_filesystem_mount(fs, FS_TEST_FNAME);

  ASSERT((file = fs_filesystem_find(fs, "/", FNAME)), "file must be present");
  ASSERT(file->attrs.size == 1 * FS_KILOBYTE, "has the correct size");
  ASSERT((file = fs_filesystem_find(fs, "/", "lol")), "dir must be present");
  ASSERT(file->attrs.is_directory, "dir must be dir");

  fs_filesystem_destroy(fs);
}

void test22()
{
  const char* expected = "d   4.0KB 1969-12-31 21:00 .         \n"
                         "d   4.0KB 1969-12-31 21:00 ..        \n"
                         "d   4.0KB 1969-12-31 21:00 hue       \n";
  const unsigned BUFSIZE = FS_LS_FORMAT_SIZE * 3;
  char* buf = calloc(BUFSIZE, sizeof(*buf));
  PASSERT(buf, FS_ERR_MALLOC);

  memset(buf, '\0', BUFSIZE);

  fs_filesystem_t* fs = fs_filesystem_create(10);
  fs_utils_fdelete(FS_TEST_FNAME);
  fs_filesystem_mount(fs, FS_TEST_FNAME);
  fs_filesystem_mkdir(fs, "/lol");
  fs_filesystem_mkdir(fs, "/lol/hue");
  fs_filesystem_ls(fs, "/lol", buf, BUFSIZE);

  STRNCMP(buf, expected);

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
  TEST(test8, "ls - root w/ children");
  TEST(test9, "find - check for file in current layer");
  TEST(test10, "find revamped - recursive search");
  TEST(test11, "rm - remove file");
  TEST(test12, "rm - add and remove sole file");
  TEST(test13, "mount - file persistence - single level");
  TEST(test14, "cp - copy from real fs to sim - 1KB");
  TEST(test15, "cp - copy from real fs to sim - 1MB");
  TEST(test16, "cat which actually copies out to real fs");
  TEST(test17, "dir - single level");
  TEST(test18, "dir - multiple levels");
  TEST(test19, "multiple `mkdir` && `rmdir`");
  TEST(test20, "`df` display");
  TEST(test21, "restore `fs` after `cp`");
  TEST(test22, "ls - nested fs");

  return 0;
}
