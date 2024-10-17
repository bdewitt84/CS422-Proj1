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
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h> // basename

void lineBreak() {
  write(STDOUT_FILENO, "\n", strlen("\n"));
}

void listDir() { // ls
  // get cwd
  size_t buf_size = 1024;
  char buf[buf_size];
  char* cwd = getcwd(buf, sizeof(buf));

  // open dir
  DIR *dirp;
  dirp = opendir(cwd);

  struct dirent *read_file;

  while((read_file = readdir(dirp)) != NULL ) {
    write(STDOUT_FILENO, read_file->d_name, strlen(read_file->d_name));
    write(STDOUT_FILENO, " ", strlen(" "));
  }
  lineBreak();
  closedir(dirp);
}

void showCurrentDir() { // pwd
  int buf_size = 128;
  char buf[buf_size];
  char *result = getcwd(buf, sizeof(buf));
  write(STDOUT_FILENO, result, strlen(result));
  write(STDOUT_FILENO, "\n", strlen("\n"));
}

void makeDir(char *dirName) { // mkdir
  mode_t mode = 0755;
  if (mkdir(dirName, mode) == -1) {
    if (errno == EEXIST) {
      char msg[] = "Directory already exists!";
      write(STDOUT_FILENO, msg, strlen(msg));
      lineBreak();
    }
  }
}

void changeDir(char *dirName) { // cd
  if (chdir(dirName) == -1) {
    char msg[] = "error: cannot change directory to ";
    write(STDOUT_FILENO, msg, strlen(msg));
    write(STDOUT_FILENO, dirName, strlen(dirName));
    lineBreak();
  }
}

void copyFile(char *sourcePath, char *destinationPath) { // cp

  int sourceFile, destinationFile;

  // attempt to open source file
  if ((sourceFile = open(sourcePath, O_RDONLY)) == -1) {
    char msg[] = "Could not open source file!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return;
  }

  // get stat of destination so we can check if it is a dir
  struct stat statBuf;

  char fullDestPath[1024];
  if (stat(destinationPath, &statBuf) == -1) {
    // prevents checking of uninitialized val if stat fails
    statBuf.st_mode = 0;
  }

  // if dest is a dir, append filename to destination
  if (S_ISDIR(statBuf.st_mode)) {
    char* base = basename(sourcePath);
    snprintf(fullDestPath, sizeof(fullDestPath), "%s/%s", destinationPath, base);
  } 
  else {
  // otherwise just copy the original path
    strcpy(fullDestPath, destinationPath);
  }


  // attempt to open destination file
  if ((destinationFile = open(fullDestPath, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    char msg[] = "Could not open destination file!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    close(sourceFile);
    return;
  }

  // copy file
  char buffer[1024];
  size_t bytes_read, bytes_written;
  while ((bytes_read = read(sourceFile, buffer, sizeof(buffer))) != 0) {
    bytes_written = write(destinationFile, buffer, bytes_read);
    if (bytes_read != bytes_written) {
      char msg[] = "Error copying data!";
      write(STDOUT_FILENO, msg, strlen(msg));
      // should probably close and return here
    }
  }

  close(sourceFile);
  close(destinationFile);

}

void moveFile(char *sourcePath, char *destinationPath) { // mv

  // get stat of destination so we can check if it is a dir
  struct stat statBuf;

  char fullDestPath[1024];
  if (stat(destinationPath, &statBuf) == -1) {
    statBuf.st_mode = 0;
  }

  // if dest is a dir, append filename to destination
  if (S_ISDIR(statBuf.st_mode)) {
    char* base = basename(sourcePath);
    snprintf(fullDestPath, sizeof(fullDestPath), "%s/%s", destinationPath, base);
  }
  else {
  // otherwise just copy the original path
    strcpy(fullDestPath, destinationPath);
  }

  // move file
  // if (rename(sourcePath, destinationPath) == -1) {
  if (rename(sourcePath, fullDestPath) == -1) {
    char msg[] = "Could not move file!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
  }
}

void deleteFile(char *filename) { // rm
  if (access(filename, F_OK) != 0) {
    char msg[] = "File does not exist!";
    write(STDOUT_FILENO, msg, strlen(msg));
  }
  else if (unlink(filename) == -1) {
    char msg[] = "Could not delete file!";
    write(STDOUT_FILENO, msg, strlen(msg));
  }
  lineBreak();
}

void displayFile(char *filename) { // cat
  int fd;
  if ((fd = open(filename, O_RDONLY)) == -1) {
    char msg[] = "Could not open file!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    return;
  }

  char buffer[1024];
  size_t bytes;
  while ((bytes = read(fd, buffer, sizeof(buffer))) != 0) {
    write(STDOUT_FILENO, buffer, bytes);
  }
  close(fd);
}

