#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

int create_db_header(int fd, struct dbheader_t **out_header) {
  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Malloc failed to create database header\n");
    return STATUS_ERROR;
  }

  header->version = 0x1;
  header->count = 0x0;
  header->magic = HEADER_MAGIC;
  header->filesize = sizeof(struct dbheader_t);

  *out_header = header;

  return STATUS_SUCCESS;
}

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
