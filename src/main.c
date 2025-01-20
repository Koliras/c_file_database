#include "common.h"
#include "file.h"
#include "parse.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void print_usage(char *executable) {
  printf("Usage: %s -n -f <database file>\n", executable);
  printf("\t -n - create a new database file\n");
  printf("\t -f - (required) path to the database file\n");
  return;
}

int main(int argc, char *argv[]) {
  bool new_file = false;
  char *filepath = NULL;
  struct employee_t *employees = NULL;

  int i = 1;
  for (i = 1; i < argc; i++) {
    char *arg = argv[i];
    if (strcmp(arg, "-n") == 0) {
      new_file = true;
    } else if (strcmp(arg, "-f") == 0) {
      i++;
      if (argv[i] == NULL) {
        printf("No filepath specified for `-f` flag\n");
        print_usage(argv[0]);
        return -1;
      }
      filepath = argv[i];
    }
  }

  if (filepath == NULL) {
    printf("Filepath is a required argument\n");
    print_usage(argv[0]);
    return -1;
  }

  int dbfd = -1;
  struct dbheader_t *dbheader = malloc(sizeof(struct dbheader_t));
  if (dbheader == NULL) {
    perror("malloc");
    printf("Failed to allocate memory for database header\n");
    return -1;
  }

  if (new_file) {
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Could not create database file\n");
      free(dbheader);
      return -1;
    }

    create_db_header(dbheader);
    if (write_header_to_db(dbfd, dbheader) == STATUS_ERROR) {
      printf("Could not write header to db\n");
      free(dbheader);
      close(dbfd);
      return -1;
    };
  } else {
    dbfd = open_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Could not open database file\n");
      free(dbheader);
      return -1;
    }

    if (validate_db_header(dbfd, dbheader) == STATUS_ERROR) {
      printf("Invalid database headers\n");
      free(dbheader);
      close(dbfd);
      return -1;
    }
  }

  if (read_employees(dbfd, dbheader, &employees) != STATUS_SUCCESS) {
    printf("Failed to read employees\n");
    return 0;
  }

  printf("New file: %d\n", new_file);
  printf("Filepath: %s\n", filepath);
  free(dbheader);
  free(employees);
  close(dbfd);
  return 0;
}
