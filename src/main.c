#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
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
  char *file_path = NULL;

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
      file_path = argv[i];
    }
  }

  if (file_path == NULL) {
    printf("Filepath is a required argument\n");
    print_usage(argv[0]);
    return -1;
  }
  printf("New file: %d\n", new_file);
  printf("Filepath: %s\n", file_path);
  return 0;
}
