#include "fssim/cli.h"

#define _F_CHECK_MOUNTED(__simulator)                                          \
  if (!__simulator->fs) {                                                      \
    fprintf(stderr, "%s\n", "Filesystem must have been previously mounted.\n"  \
                            "Enter `help` if you need help.\n");               \
    return 1;                                                                  \
  }

#define _F_CHECK_ARGC(__argc, __n)                                             \
  if (__argc != __n) {                                                         \
    fprintf(stderr, "ERROR: Wrong arguments specification\n"                   \
                    "Must have `%d` arguments.\n"                              \
                    "Enter `help` if you need help.\n",                        \
            __n);                                                              \
    return 1;                                                                  \
  }

void fs_cli_run()
{
  unsigned argc = 0;
  char* input = NULL;
  char** argv = NULL;
  fs_cli_command cmd = NULL;
  fs_simulator_t* sim = fs_simulator_create();

  rl_bind_key('\t', rl_complete);
  fprintf(stderr, "%s", FS_CLI_WELCOME);

  while (1) {
    argc = 0;
    input = readline(FS_CLI_PROMPT);
    if (!input)
      continue;

    add_history(input);
    argv = history_tokenize(input);
    if (!argv) {
      free(input);
      continue;
    }

    cmd = fs_cli_search_command(argv[0]);
    if (cmd != NULL) {
      while (argv[argc]) // !!
        argc++;
      cmd(argv, argc, sim);
    } else {
      fprintf(stderr, "Couldn't find command %s\n", argv[0]);
    }

    free(input);
    while (argc-- > 0)
      free(argv[argc]);
    free(argv);
  }

  fs_simulator_destroy(sim);
}

fs_cli_command fs_cli_search_command(const char* cmd)
{
  int bottom = 0;
  int top = FS_CLI_COMMANDS_SIZE - 1;
  int mid;
  int res;

  while (bottom <= top) {
    mid = (bottom + top) / 2;

    if (!(res = strcmp(FS_CLI_COMMANDS[mid].key, cmd)))
      return FS_CLI_COMMANDS[mid].command;
    else if (res > 0)
      top = mid - 1;
    else if (res < 0)
      bottom = mid + 1;
  }

  return NULL;
}

int fs_cli_command_mount(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_ARGC(argc, 2);

  if (sim->fs) {
    fprintf(stderr, "Filesystem already mounted at %s.\n"
                    "If you wish to mount another fs, enter `unmount` first.\n"
                    "Enter `help` if you need help.\n",
            sim->mounted_at);
    return 1;
  }

  sim->fs = fs_filesystem_create(FS_BLOCKS_NUM);
  fs_filesystem_mount(sim->fs, argv[1]);
  strncpy(sim->mounted_at, argv[1], PATH_MAX);
  fprintf(stderr, "Filesystem sucessfully mounted at %s\n", sim->mounted_at);

  return 0;
}

int fs_cli_command_cp(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 3);

  return 0;
}

int fs_cli_command_mkdir(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 2);

  return 0;
}

int fs_cli_command_rmdir(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 2);

  return 0;
}

int fs_cli_command_cat(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 2);

  return 0;
}

int fs_cli_command_touch(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 2);

  return 0;
}

int fs_cli_command_rm(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 2);

  return 0;
}

int fs_cli_command_ls(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 2);

  return 0;
}

int fs_cli_command_find(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 3);

  return 0;
}

int fs_cli_command_df(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  return 0;
}

int fs_cli_command_unmount(char** argv, unsigned argc, fs_simulator_t* sim)
{
  _F_CHECK_MOUNTED(sim);
  _F_CHECK_ARGC(argc, 1);

  fs_filesystem_destroy(sim->fs);
  sim->fs = NULL;

  return 0;
}

int fs_cli_command_help(char** argv, unsigned argc, fs_simulator_t* sim)
{
  fprintf(stderr, "%s", FS_CLI_HELP);
  return 0;
}

int fs_cli_command_sai(char** argv, unsigned argc, fs_simulator_t* sim)
{
  fprintf(stderr, "%s\n", "Bye Bye!");
  exit(0);
}
