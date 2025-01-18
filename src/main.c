#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "header.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 0;
  }

  int fd = open(argv[1], O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  struct database_header db_head = {0};
  struct stat db_stat = {0};
  if (read_db_header(fd, &db_head) != 0) {
    close(fd);
    return -1;
  };
  if (validate_header(fd, &db_head) != 0) {
    close(fd);
    return -1;
  }

  printf("Database version: %u\n", db_head.version);
  printf("Database employees: %u\n", db_head.employees);
  printf("Database length: %u\n", db_head.filelength);

  close(fd);
  return 0;
}
