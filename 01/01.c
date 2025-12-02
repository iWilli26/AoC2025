#include "base.h"
#include <stdio.h>
#include <stdlib.h>

int absolute(int x) { return x >= 0 ? x : -x; }

int main(void) {
  int num_lines = 0;
  char **lines = read_file_lines("./01/input.txt", &num_lines);
  int dial = 50;
  int count = 0;
  for (int i = 0; i < num_lines; i++) {
    char *line = lines[i];
    char command = line[0];
    int number = atoi(&line[1]);
    switch (command) {
    case 'L':
      for (int j = 0; j < number; j++) {
        dial--;
        if (dial < 0) {
          dial = 99;
        }
        if (dial == 0) {
          count++;
        }
      }
      break;
    case 'R':
      for (int k = 0; k < number; k++) {
        dial = (dial + 1) % 100;
        if (dial == 0) {
          count++;
        }
      }
      break;
    default:
      break;
    }

    printf("%c %d %d\n", command, number, dial);
  }

  printf("Count: %d\n", count);

  free_file_lines(lines, num_lines);
  return 0;
}

// gcc -Wall -Wextra -g3 -I./lib ./01/01.c ./lib/base.c -o ./01/output/01.exe