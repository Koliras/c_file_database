#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "parse.h"

void create_db_header(struct dbheader_t *header) {
  header->version = PROGRAM_VERSION;
  header->count = 0x0;
  header->magic = HEADER_MAGIC;
  header->filesize = sizeof(struct dbheader_t);
}

int validate_db_header(int fd, struct dbheader_t *header) {
  if (read(fd, header, sizeof(struct dbheader_t)) !=
      sizeof(struct dbheader_t)) {
    perror("read");
    printf("Malformed database file header\n");
    return STATUS_ERROR;
  }

  header->version = ntohs(header->version);
  header->count = ntohs(header->count);
  header->magic = ntohl(header->magic);
  header->filesize = ntohl(header->filesize);

  if (header->version != PROGRAM_VERSION) {
    printf("Improper header version\n");
    return STATUS_ERROR;
  }
  if (header->magic != HEADER_MAGIC) {
    printf("Improper header magic\n");
    return STATUS_ERROR;
  }

  struct stat dbstat = {0};
  if (fstat(fd, &dbstat) != 0) {
    printf("Failed to read data about the file\n");
    return STATUS_ERROR;
  }

  if (header->filesize != dbstat.st_size) {
    printf("Corrupted database\n");
    return STATUS_ERROR;
  }

  return STATUS_SUCCESS;
}

int write_header_to_db(int fd, struct dbheader_t *out_header) {
  struct dbheader_t header;
  header.magic = htonl(out_header->magic);
  header.filesize = htonl(out_header->filesize);
  header.count = htons(out_header->count);
  header.version = htons(out_header->version);

  if (lseek(fd, 0, SEEK_SET) == -1) {
    perror("lseek");
    return STATUS_ERROR;
  }
  if (write(fd, &header, sizeof(struct dbheader_t)) == -1) {
    perror("write");
    return STATUS_ERROR;
  }
  return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *header,
                   struct employee_t **out_employees) {
  int count = header->count;
  struct employee_t *employees = calloc(count, sizeof(struct employee_t));
  if (employees == NULL) {
    printf("Malloc failed to allocate data for employees\n");
    return STATUS_ERROR;
  }

  if (read(fd, employees, count * sizeof(struct employee_t)) == -1) {
    perror("read");
    free(employees);
    return STATUS_ERROR;
  }

  int i = 0;
  for (i = 0; i < count; i++) {
    employees[i].hours = htons(employees[i].hours);
  }

  *out_employees = employees;
  return STATUS_SUCCESS;
}
