#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parse.h"

int read_db_header(int fd, struct dbheader_t *db_head) {
  if (read(fd, db_head, sizeof(*db_head)) != sizeof(*db_head)) {
    perror("read");
    return -1;
  }
  return 0;
}

int validate_header(int fd, struct dbheader_t *h) {
  struct stat db_stat;

  if (fstat(fd, &db_stat) == -1) {
    perror("fstat");
    return -1;
  }
  if (db_stat.st_size != h->filesize) {
    printf("MALFORMED FILE!");
    return -1;
  }

  return 0;
}
