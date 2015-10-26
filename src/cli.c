#include "fssim/cli.h"

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
  if (argc != 2) {
    fprintf(stderr, "ERROR: Wrong arguments specification\n"
                    "Must be: mount <fname>\n");
    return 1;
  }

  if (!fexists(argv[1])) {
    /* sim->fs = fs_filesystem_create(argv[1]); */
  } else {
    /* sim->fs = fs_filesystem_load(argv[1]); */
  }

  return 0;
}

int fs_cli_command_cp(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_mkdir(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_rmdir(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_cat(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_touch(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_rm(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_ls(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_find(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_df(char** argv, unsigned argc, fs_simulator_t* sim)
{
  return 0;
}

int fs_cli_command_unmount(char** argv, unsigned argc, fs_simulator_t* sim)
{
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
