#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *log_file;

int serverfiles(char *filename) {

  //making a log entry of successfully entering the library
  log_file = fopen("serverfiles log.txt", "w");
  fprintf(log_file, "Successfully entered serverfiles.c.\n");
  fclose(log_file);
  return 0;
}
