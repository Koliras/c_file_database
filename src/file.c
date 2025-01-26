#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

int create_db_file(char *filepath) {
  int fd = open(filepath, O_RDONLY);
  if (fd != -1) {
    close(fd);
    printf("File already exists\n");
    return STATUS_ERROR;
  }

  fd = open(filepath, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }
  return fd;
}

int open_db_file(char *filepath) {
  int fd = open(filepath, O_RDWR, 0644);
  if (fd == -1) {
    perror("open");
    return STATUS_ERROR;
  }
  return fd;
}

int get_db_file_with_header(char *filepath, bool new_file,
                            struct dbheader_t *dbheader) {
  int dbfd = -1;
  if (new_file) {
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Could not create database file\n");
      return STATUS_ERROR;
    }

    create_db_header(dbheader);
    if (write_header_to_db(dbfd, dbheader) == STATUS_ERROR) {
      printf("Could not write header to db\n");
      close(dbfd);
      return STATUS_ERROR;
    };

    return dbfd;
  }

  dbfd = open_db_file(filepath);
  if (dbfd == STATUS_ERROR) {
    printf("Could not open database file\n");
    return STATUS_ERROR;
  }

  return dbfd;
}
