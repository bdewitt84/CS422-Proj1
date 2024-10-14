/* 
 * command.c
 *
 * Author Brett DeWitt
 * Created 10/12/2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void listDir() { // ls
  printf("listDir()\n");
}

void showCurrentDir() { // pwd
  printf("showCurrentDir()\n");
}

void makeDir(char *dirName) { // mkdir
  printf("makeDir(%s)\n", dirName);
}

void changeDir(char *dirName) { // cd
  printf("changeDir(%s)\n", dirName);
}

void copyFile(char *sourcePath, char *destinationPath) { // cp
  printf("copyFile(%s, %s)\n", sourcePath, destinationPath);
}

void moveFile(char *sourcePath, char *destinationPath) { // mv
  printf("moveFile(%s, %s)\n", sourcePath, destinationPath);
}

void deleteFile(char *filename) { // rm
  printf("deleteFile(%s)\n", filename);
}

void displayFile(char *filename) { // cat
  printf("displayFile(%s)\n", filename);
}

