/*
 * main.c
 *
 * Created on 10/12/2024
 * Authoer Brett DeWitt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // not sure why yet
#include "command.h"
#include "string_parser.h"


// Signatures
void interactiveShell();

void fileMode(char const *path);

void usage(char const *argv[]);

int processCommandLine(command_line const *cmd);


int validateCommand(char *cmd, char *compare_str, int num_args, int expected_args);


// Constants
int MAX_SIZE_INPUT = 1024;


int main(int argc, char const *argv[]) {
  if (argc == 1) {
    // interactive shell mode
    interactiveShell(stdin, STDOUT_FILENO);
  }
  else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
    fileMode(argv[2]);
  }
  else {
    usage(argv);
  }
	return 0;
}

void interactiveShell() {

  char *input = NULL;
  size_t len = 0;

  // initial prompt
  char prompt[] = ">>>";
  write(STDOUT_FILENO, prompt, strlen(prompt));

  while(getline (&input, &len, stdin) != -1) {
  
    // exit or process command
    if (strcmp(input, "exit\n") == 0) {
      free(input);
      break;
    }

    // tokenize input;

    command_line large_token_buffer;
    command_line small_token_buffer;

    int line_num = 0;

    large_token_buffer = str_filler(input, ";");
    for (int i = 0; large_token_buffer.command_list[i] != NULL; ++i) {
      small_token_buffer = str_filler (large_token_buffer.command_list[i], " ");
      // process command
      if (processCommandLine(&small_token_buffer) == -1){
        break;
      }
      // free small token buffer memory
      free_command_line(&small_token_buffer);
      memset(&small_token_buffer, 0, 0);
    }

    // free large token buffer
    free_command_line(&large_token_buffer);
    memset (&large_token_buffer, 0, 0);

    // continue prompt
    write(STDOUT_FILENO, prompt, strlen(prompt));
  }
}

void fileMode(char const *path) {
  // only accepts filenames
  // all output written to ./output.txt
  printf("fileMode()\n");
}

void usage(char const *argv[]) {
  printf("usage()\n");
}


int processCommandLine(command_line const *cmd_line) {
  char *cmd = cmd_line->command_list[0];
  int num_args = cmd_line->num_token;
  char args_err[] = "Error! Unsupported parameters for command: ";

  if (strcmp(cmd, "ls") == 0) {
    if (num_args != 1) {
      printf("%s%s\n",args_err, cmd);
      return -1;
    }
    listDir();
  }

  else if (strcmp(cmd, "pwd") == 0) {
    if (num_args != 1) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
    showCurrentDir();
  }

  else if (strcmp(cmd, "mkdir") == 0) {
    if (num_args != 2) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
    makeDir(cmd_line->command_list[1]);
  }

  else if (strcmp(cmd, "cd") == 0) {
    if (num_args != 2) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
    changeDir(cmd_line->command_list[1]);
  }

  else if (strcmp(cmd, "cp") == 0) {
    if (num_args != 3) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
    copyFile(cmd_line->command_list[1], cmd_line->command_list[2]);
  }

  else if (strcmp(cmd, "mv") == 0) {
    if (num_args != 3) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
    moveFile(cmd_line->command_list[1], cmd_line->command_list[2]);
  }

  else if (strcmp(cmd, "rm") == 0) {
    if (num_args != 2) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
    deleteFile(cmd_line->command_list[1]);
  }

  else if (strcmp(cmd, "cat") == 0) {
    if (num_args != 2) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
    displayFile(cmd_line->command_list[1]);
  }

  // command not supported
  else {
    char err[] = "Error! Unrecognized command: ";
    write(STDOUT_FILENO, err, strlen(err));
    write(STDOUT_FILENO, cmd, strlen(cmd));
    write(STDOUT_FILENO, "\n", 2);
    return -1;
  }
  return 0;
}

// not really necessary
int validateCommand(char *cmd, char *compare_str, int num_args, int expected_args) {
  char args_err[] = "!!!Error! Unsupported parameters for command: ";
  if (strcmp(cmd, compare_str) == 0) {
    if (num_args != expected_args) {
      printf("%s%s\n", args_err, cmd);
      return -1;
    }
  }
  return 0;
}
