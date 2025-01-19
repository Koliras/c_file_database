#include <arpa/inet.h>
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

int validate_db_header(int fd, struct dbheader_t **h) {
  if (fd < 0) {
    printf("Got a bad FD from user\n");
    return STATUS_ERROR;
  }

  struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
  if (header == NULL) {
    printf("Malloc failed to create space for database header\n");
    return STATUS_ERROR;
  }

  if (read(fd, header, sizeof(struct dbheader_t)) !=
      sizeof(struct dbheader_t)) {
    perror("open");
    printf("Malformed database file header\n");
    free(header);
    return STATUS_ERROR;
  }

  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);

  if (header->version != 1) {
    printf("Improper header version\n");
    free(header);
    return STATUS_ERROR;
  }
  if (header->magic != HEADER_MAGIC) {
    printf("Improper header magic\n");
    free(header);
    return STATUS_ERROR;
  }

  struct stat dbstat = {0};
  if (fstat(fd, &dbstat) != 0) {
    free(header);
    printf("Failed to read data about a file\n");
    return STATUS_ERROR;
  }

  if (header->filesize != dbstat.st_size) {
    printf("Corrupted database\n");
    free(header);
    return STATUS_ERROR;
  }

  return 0;
}
