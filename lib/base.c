#include "base.h"
#include <string.h>

char **read_file_lines(const char *filename, int *num_lines) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    *num_lines = 0;
    return NULL;
  }

  int line_count = 0;
  char buffer[4096];
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      len--;
    }
    if (len > 0 && buffer[len - 1] == '\r') {
      len--;
    }
    if (len > 0) {
      line_count++;
    }
  }
  printf("line_count = %d\n", line_count);
  char **lines = malloc(line_count * sizeof(char *));

  rewind(file);

  int index = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL && index < line_count) {
    int len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
      len--;
    }
    if (len > 0 && buffer[len - 1] == '\r') {
      buffer[len - 1] = '\0';
    }
    if (len == 0) {
      continue;
    }
    lines[index] = malloc((strlen(buffer) + 1) * sizeof(char));
    if (lines[index] == NULL) {
      for (int i = 0; i < index; i++) {
        free(lines[i]);
      }
      free(lines);
      fclose(file);
      *num_lines = 0;
      return NULL;
    }
    strcpy(lines[index], buffer);
    index++;
  }

  fclose(file);
  *num_lines = line_count;
  return lines;
}

void free_file_lines(char **lines, int num_lines) {
  if (lines == NULL)
    return;
  for (int i = 0; i < num_lines; i++) {
    free(lines[i]);
  }
  free(lines);
}
