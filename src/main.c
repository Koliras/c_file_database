#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *executable) {
  printf("Usage: %s -n -f <database file>\n", executable);
  printf("\t -n - create a new database file\n");
  printf("\t -f - (required) path to the database file\n");
  return;
}

int main(int argc, char *argv[]) {
  bool new_file = false;
  char *filepath = NULL;
  int dbfd = -1;
  struct dbheader_t dbheader;
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
    } else {
      printf("Unknown option %s\n", argv[i]);
      print_usage(argv[0]);
      return -1;
    }
  }

  dbfd = get_db_file_with_header(filepath, new_file, &dbheader);
  if (dbfd == STATUS_ERROR) {
    return -1;
  }

  if (!new_file && validate_db_header(dbfd, &dbheader) == STATUS_ERROR) {
    printf("Invalid database headers\n");
    close(dbfd);
    return -1;
  }

  employees = malloc(dbheader.count * sizeof(struct employee_t));
  if (employees == NULL) {
    printf("Failed to allocate memory for employees\n");
    return -1;
  }

  if (read_employees(dbfd, dbheader.count, employees) == STATUS_ERROR) {
    printf("Failed to read employees\n");
    free(employees);
    close(dbfd);
    return 0;
  }

  free(employees);
  close(dbfd);
  return 0;
}
