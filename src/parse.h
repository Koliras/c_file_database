#ifndef PARSE_H
#define PARSE_H

struct dbheader_t {
  unsigned int magic;
  unsigned short version;
  unsigned short count;
  unsigned int filesize;
};

int read_db_header(int fd, struct dbheader_t *db_head);

int validate_header(int fd, struct dbheader_t *h);

#endif
