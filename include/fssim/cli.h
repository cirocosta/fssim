#ifndef FSSIM__CLI_H
#define FSSIM__CLI_H

#include "fssim/common.h"
#include "fssim/file_utils.h"
#include "fssim/simulator.h"

#include <readline/readline.h>
#include <readline/history.h>

typedef int (*fs_cli_command)(char** argument, unsigned argc,
                              fs_simulator_t* sim);
typedef struct fs_cli_command_t {
  char* key;
  fs_cli_command command;
} fs_cli_command_t;

int fs_cli_command_mount(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_cp(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_mkdir(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_rmdir(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_cat(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_touch(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_rm(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_ls(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_find(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_df(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_unmount(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_help(char** argv, unsigned argc, fs_simulator_t* sim);
int fs_cli_command_sai(char** argv, unsigned argc, fs_simulator_t* sim);

static const char* FS_CLI_PROMPT = "[ep3] ";

#define FS_CLI_COMMANDS_SIZE 13

const static char* FS_CLI_WELCOME =
    "\n"
    "Welcome to *fssim*! The Filesystem Simulator\n"
    "If you need help, type `help`.\n\n";

const static fs_cli_command_t FS_CLI_COMMANDS[] = {
  { "cat", &fs_cli_command_cat },
  { "cp", &fs_cli_command_cp },
  { "df", &fs_cli_command_df },
  { "find", &fs_cli_command_find },
  { "help", &fs_cli_command_help },
  { "ls", &fs_cli_command_ls },
  { "mkdir", &fs_cli_command_mkdir },
  { "mount", &fs_cli_command_mount },
  { "rm", &fs_cli_command_rm },
  { "rmdir", &fs_cli_command_rmdir },
  { "sai", &fs_cli_command_sai },
  { "touch", &fs_cli_command_touch },
  { "unmount", &fs_cli_command_unmount },
};

void fs_cli_run();

const static char* FS_CLI_HELP =
    "\nUSAGE: `$ ./fssim`\n"
    "  Starts a prompt which accepts the following commands:\n"
    "\n"
    "COMMANDS:\n"
    "  mount <fname>         mounts the fs in the given <fname>. In\n"
    "                        case <fname> already exists, countinues\n"
    "                        from where it stopped.\n"
    "\n"
    "  cp <src> <dest>       copies a file from the real system to the\n"
    "                        simulated filesystem (dest).\n"
    "\n"
    "  mkdir <dir>           creates a directory named <dir>\n"
    "\n"
    "  rmdir <dir>           removes the directory <dir> along with the\n"
    "                        files that were there (ir any)\n"
    ""
    "  cat <fname>           shows (stdout) the contents of <fname>\n"
    "\n"
    "  touch <fname>         creates a file <fname>. If it already exists,\n"
    "                        updates the file's last access time.\n"
    "\n"
    "  rm <fname>            remover a given <fname>\n"
    "\n"
    "  ls <dir>              lists the files and directories 'below'\n"
    "                        <dir>, showing $name, $size, $last_mod for\n"
    "                        files and an indicator for directories.\n"
    "\n"
    "  find <dir> <fname>    taking <dir> as root, recursively searches\n"
    "                        for a file named <fname>\n"
    "\n"
    "  df                    shows filesystem info, including number of\n"
    "                        diferectories, files, freespace and wasted\n"
    "                        space\n"
    "\n"
    "  unmount               unmounts the current filesystem\n"
    "\n"
    "  help                  shows this message\n"
    "\n"
    "  sai                   exits the simulator\n"
    "\n"
    "\n"
    "NOTES:\n"
    "  in the whole simulation all filenames must be specified as \n"
    "  absolute paths.\n"
    "\n"
    "AUTHOR:\n"
    "  Written by Ciro S. Costa <ciro dot costa at usp dot br>.\n"
    "  See `https://github.com/cirocosta/fssim`. Licensed with MPLv2.\n"
    "\n";

#endif
