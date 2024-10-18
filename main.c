/*
 * main.c
 *
 * Created on 10/12/2024
 * Author Brett DeWitt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // not sure why yet
#include "command.h"
#include "string_parser.h"


// Signatures
void interpretCommands(char* prompt);

void usage(char const *argv[]);

int processCommandLine(command_line const *cmd);


// Constants
int MAX_SIZE_INPUT = 1024;


int main(int argc, char const *argv[]) {
  // TODO: attempt to force printf to write imediately
  setvbuf(stdout, NULL, _IONBF, 0);
  if (argc == 1) {
    // interactive shell mode
    char prompt[] = ">>> ";
    interpretCommands(prompt);
  }
  else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
    // file mode
    char prompt[] = "";
    freopen(argv[2], "r", stdin);
    freopen("./output.txt", "w", stdout);
    interpretCommands(prompt);
    freopen("/dev/tty", "r", stdin);
    freopen("/dev/tty", "w", stdout);
  }
  else {
    usage(argv);
  }
	return 0;
}

void interpretCommands(char* prompt) {

  char *input = NULL;
  size_t len = 0;

  // initial prompt
  write(STDOUT_FILENO, prompt, strlen(prompt));

  while(getline (&input, &len, stdin) != -1) {
  
    // exit or process command
    if (strcmp(input, "exit\n") == 0) {
      // free(input);
      break;
    }

    // tokenize input;
    command_line large_token_buffer;
    command_line small_token_buffer;
    int line_num = 0;

    large_token_buffer = str_filler(input, ";");
    for (int i = 0; large_token_buffer.command_list[i] != NULL; ++i) {
      small_token_buffer = str_filler (large_token_buffer.command_list[i], " ");

      // process command (behavior for out.txt)
      // this is the behavior recommended by TA Albie
      if (processCommandLine(&small_token_buffer) == -1) {
        free_command_line(&small_token_buffer);
        break;
      }

      // process command (behavior for screenshot)
      // processCommandLine(&small_token_buffer);

      // flush buffer to keep up with write
      fflush(stdout);

      // free small token buffer memory
      free_command_line(&small_token_buffer);
    }

    // free large token buffer
    free_command_line(&large_token_buffer);

    // continue prompt
    write(STDOUT_FILENO, prompt, strlen(prompt));
  }
  free(input);
}

void usage(char const *argv[]) {
  printf("usage()\n");
}


int processCommandLine(command_line const *cmd_line) {
  char *cmd = cmd_line->command_list[0];
  int num_args = cmd_line->num_token;
  char args_err[] = "Error! Unsupported parameters for command: ";

  if (cmd == NULL) {
    // check for null token
    return 0;
  }

  else if (strcmp(cmd, "ls") == 0) {
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
    printf("%s%s\n", err, cmd);
    return -1;
  }
  return 0;
}

