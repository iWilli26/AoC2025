#ifndef BASE_H
#define BASE_H
#include <stdio.h>
#include <stdlib.h>

char **read_file_lines(const char *filename, int *num_lines);

void free_file_lines(char **lines, int num_lines);

#endif
