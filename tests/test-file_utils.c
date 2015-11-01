#include "fssim/common.h"
#include "fssim/file_utils.h"

void test1()
{
  const char input[] = "/bin/ls/hue/lol.txt";
  const char* expected[] = { "bin", "ls", "hue", "lol.txt", NULL };

  unsigned argc = 0;
  char** argv = fs_utils_splitpath(input, &argc);

  ASSERT(argc == 4, "");
  for (unsigned i = 0; i < argc; i++)
    ASSERT(strcmp(argv[i], expected[i]) == 0, "%s != %s", argv[i], expected[i]);
  ASSERT(argv[argc] == NULL, "Must end with a NULL");

  FREE_ARR(argv, argc);
}

void test2()
{
  const char input[] = "/";
  const char* expected[] = { NULL };

  unsigned argc = 0;
  char** argv = fs_utils_splitpath(input, &argc);

  ASSERT(argc == 0, "%d != %d", argc, 2);
  ASSERT(argv[argc] == NULL, "Must end with a NULL");

  FREE_ARR(argv, argc);
}

void test3()
{
  const unsigned n_values = 4;
  const char* fname = "./test-file_utils-test3";
  const uint32_t values[4] = { 10, 20, 30, 40 };

  FILE* f;
  unsigned char* buf = calloc(16, sizeof(*buf));
  int buf_counter = 0;

  PASSERT(buf, FS_ERR_MALLOC);
  PASSERT((f = fopen(fname, "w+b")), "fopen:");

  for (int i = 0; i < n_values; i++)
    serialize_uint32_t(buf + (i * 4), values[i]);

  fwrite(buf, sizeof(*buf), 16, f);
  PASSERT(fclose(f) == 0, "fclose:");
  f = NULL;

  memset(buf, '\0', 16);
  PASSERT((f = fopen(fname, "rb")), "fopen:");
  fread(buf, sizeof(*buf), 16, f);

  for (int i = 0; i < n_values; i++) {
    ASSERT(deserialize_uint32_t(buf + (i * 4)) == values[i], "");
  }

  PASSERT(fclose(f) == 0, "fclose:");
  free(buf);
}

void test4()
{
  const unsigned n_values = 4;
  const char* fname = "./test-file_utils-test4";
  const int32_t values[4] = { -10, -20, 30, 40 };

  FILE* f;
  unsigned char* buf = calloc(16, sizeof(*buf));
  int buf_counter = 0;

  PASSERT(buf, FS_ERR_MALLOC);
  PASSERT((f = fopen(fname, "w+b")), "fopen:");

  for (int i = 0; i < n_values; i++)
    serialize_int32_t(buf + (i * 4), values[i]);

  fwrite(buf, sizeof(*buf), 16, f);
  PASSERT(fclose(f) == 0, "fclose:");
  f = NULL;

  memset(buf, '\0', 16);
  PASSERT((f = fopen(fname, "rb")), "fopen:");
  fread(buf, sizeof(*buf), 16, f);

  for (int i = 0; i < n_values; i++) {
    ASSERT(deserialize_int32_t(buf + (i * 4)) == values[i], "");
  }

  PASSERT(fclose(f) == 0, "fclose:");
  free(buf);
}

void test5()
{
  const char* expected = "1969-12-31 21:00";
  char buf[FS_DATE_FORMAT_SIZE] = { 0 };

  fs_utils_secs2str(0, buf, FS_DATE_FORMAT_SIZE);

  ASSERT(!strcmp(expected, buf), "%s != %s", expected, buf);
}

void test6()
{
  const char* expected = "123.0 B";
  char buf[FS_FILESIZE_FORMAT_SIZE] = {0};

  fs_utils_filesize2str(123, buf, FS_FILESIZE_FORMAT_SIZE);

  ASSERT(!strcmp(expected, buf), "%s != %s", expected, buf);
}

void test7()
{
  const char* expected = " 16.0KB";
  char buf[FS_FILESIZE_FORMAT_SIZE] = {0};

  fs_utils_filesize2str(16*FS_KILOBYTE, buf, FS_FILESIZE_FORMAT_SIZE);

  ASSERT(!strcmp(expected, buf), "%s != %s", expected, buf);
}

void test8()
{
  const char* expected = "  2.3MB";
  char buf[FS_FILESIZE_FORMAT_SIZE] = {0};

  fs_utils_filesize2str(2.3 * FS_MEGABYTE, buf, FS_FILESIZE_FORMAT_SIZE);

  ASSERT(!strcmp(expected, buf), "%s != %s", expected, buf);
}

int main(int argc, char* argv[])
{
  TEST(test1, "splits path accordingly");
  TEST(test2, "splits path accordingly");
  TEST(test3, "deals properly w/ uin32_t (de)serialization");
  TEST(test4, "deals properly w/ in32_t (de)serialization");
  TEST(test5, "time utilities");
  TEST(test6, "file size utilities - bytes");
  TEST(test7, "file size utilities - KB");
  TEST(test8, "file size utilities - megabytes");

  return 0;
}
