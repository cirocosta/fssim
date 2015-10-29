#include "fssim/common.h"
#include "fssim/filesystem.h"
#include <arpa/inet.h>

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

static inline unsigned char* serialize_uint32_t(unsigned char* buffer,
                                                uint32_t value)
{
  buffer[0] = value >> 24;
  buffer[1] = value >> 16;
  buffer[2] = value >> 8;
  buffer[3] = value;

  return buffer + 4;
}

static inline unsigned char* deserialize_uint32_t(unsigned char* buffer,
                                                  uint32_t* value)
{
  *value |= buffer[0] << 24;
  *value |= buffer[1] << 16;
  *value |= buffer[2] << 8;
  *value |= buffer[3];

  return buffer + 4;
}

void test98()
{
  char* fname = "./fssim";
  uint32_t values[4] = { 10, 20, 30, 40 };
  uint32_t reads[4] = { 0 };
  unsigned n_values = 4;
  FILE* f;
  unsigned char* buf = calloc(16, sizeof(*buf));
  int buf_counter = 0;

  PASSERT(buf, FS_ERR_MALLOC);
  PASSERT((f = fopen(fname, "w+b")), "fopen:");

  for (int i = 0; i < n_values; i++) {
    serialize_uint32_t(buf + (i * 4), values[i]);
  }

  fwrite(buf, sizeof(*buf), 16, f);
  fflush(f);
  PASSERT(fclose(f) == 0, "fclose:");
  f = NULL;

  memset(buf, '\0', 16);
  PASSERT((f = fopen(fname, "rb")), "fopen:");
  fread(buf, sizeof(*buf), 16, f);

  for (int i = 0; i < n_values; i++) {
    deserialize_uint32_t(buf + (i * 4), &reads[i]);
  }

  ASSERT(values[0] == reads[0], "v0 = %u !== %u = r0", values[0], reads[0]);
  ASSERT(values[1] == reads[1], "v1 = %u !== %u = r1", values[1], reads[1]);
  ASSERT(values[2] == reads[2], "v2 = %u !== %u = r2", values[2], reads[2]);
  ASSERT(values[3] == reads[3], "v3 = %u !== %u = r3", values[3], reads[3]);

  PASSERT(fclose(f) == 0, "fclose:");

  free(buf);
}

void test99()
{
  char* fname = "./fssim";
  uint32_t values[4] = { 10, 20, 30, 40 };
  uint32_t reads[4] = { 0 };
  unsigned n_values = 4;
  FILE* f;
  unsigned char* buf = calloc(16, sizeof(*buf));
  int buf_counter = 0;

  PASSERT(buf, FS_ERR_MALLOC);
  PASSERT((f = fopen(fname, "w+b")), "fopen:");

  for (int i = 0; i < n_values; i++) {
    values[i] = htonl(values[i]);
  }

  fwrite(values, sizeof(*values), n_values, f);
  fflush(f);
  PASSERT(fclose(f) == 0, "fclose:");
  f = NULL;

  PASSERT((f = fopen(fname, "rb")), "fopen:");
  fread(reads, sizeof(*reads), n_values, f);

  for (int i = 0; i < n_values; i++) {
    reads[i] = ntohl(reads[i]);
    values[i] = ntohl(values[i]);
  }

  ASSERT(values[0] == reads[0], "v0 = %u !== %u = r0", values[0], reads[0]);
  ASSERT(values[1] == reads[1], "v1 = %u !== %u = r1", values[1], reads[1]);
  ASSERT(values[2] == reads[2], "v2 = %u !== %u = r2", values[2], reads[2]);
  ASSERT(values[3] == reads[3], "v3 = %u !== %u = r3", values[3], reads[3]);

  PASSERT(fclose(f) == 0, "fclose:");

  free(buf);
}

int main(int argc, char* argv[])
{
  TEST(test1, "creation and deletion");
  TEST(test2, "mounting w/out previous mount");
  TEST(test3, "ls - list empty root dir");
  TEST(test4, "touch - creating a file in empty root");

  TEST(test98, "uint32 write and read binary examples");
  TEST(test99, "uint32 write and read binary examples");

  return 0;
}
