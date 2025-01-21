#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x474f4f53
#define PROGRAM_VERSION 0x1

struct dbheader_t {
  unsigned int magic;
  unsigned short version;
  unsigned short count;
  unsigned int filesize;
};

struct employee_t {
  char name[256];
  char address[256];
  unsigned short hours;
};

void create_db_header(struct dbheader_t *out_header);
int validate_db_header(int fd, struct dbheader_t *out_header);
int write_header_to_db(int fd, struct dbheader_t *header);

int read_employees(int fd, int count, struct employee_t *out_employees);

#endif
