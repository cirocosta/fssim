#include "fssim/common.h"
#include "fssim/file_utils.h"

void test1()
{
  const char input[] = "/bin/ls/hue/lol.txt";
  const char* expected[] = { "bin", "ls", "hue", "lol.txt", NULL};

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

int main(int argc, char* argv[])
{
  TEST(test1, "splits path accordingly");
  TEST(test2, "splits path accordingly");

  return 0;
}
