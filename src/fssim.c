#include "fssim/cli.h"

int main(int argc, char *argv[])
{
  if (argc > 1) {
    fprintf(stderr, "%s\n", FS_CLI_HELP);
    exit(0);
  }

  fs_cli_run();

  return 0;
}
