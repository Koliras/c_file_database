#ifndef FILE_H
#define FILE_H

#include "parse.h"
#include <stdbool.h>

int create_db_file(char *filepath);

int open_db_file(char *filepath);

int get_db_file_with_header(char *filepath, bool new_file,
                            struct dbheader_t *dbheader);

#endif
