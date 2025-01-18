#ifndef FILE_H
#define FILE_H

struct database_header {
  unsigned short version;
  unsigned short employees;
  unsigned int filelength;
};

int read_db_header(int fd, struct database_header *db_head);

int validate_header(int fd, struct database_header *h);

#endif
