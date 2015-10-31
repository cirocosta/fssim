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

int main(int argc, char* argv[])
{
  TEST(test1, "splits path accordingly");
  TEST(test2, "splits path accordingly");
  TEST(test3, "deals properly w/ uin32_t (de)serialization");
  TEST(test3, "deals properly w/ in32_t (de)serialization");

  return 0;
}
