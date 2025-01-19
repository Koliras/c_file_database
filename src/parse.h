#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x474f4f53
struct dbheader_t {
  unsigned int magic;
  unsigned short version;
  unsigned short count;
  unsigned int filesize;
};

int create_db_header(int fd, struct dbheader_t **outHeader);
int validate_db_header(int fd, struct dbheader_t **h);

#endif
