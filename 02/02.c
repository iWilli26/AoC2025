#include "../lib/base.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 6464646464 lengthToCheck = 2

bool checkValuePart1(char *valueStr) {
  int lengthToCheck = strlen(valueStr) / 2;
  if (strlen(valueStr) % 2 != 0) {
    return false;
  }
  bool isDuplicated = true;
  for (int i = 0; i < lengthToCheck; i++) {
    if (valueStr[i] != valueStr[i + lengthToCheck]) {
      isDuplicated = false;
      break;
    }
  }
  return isDuplicated;
}

bool checkValuePart2(char *valueStr, int lengthToCheck) {
  if (lengthToCheck == 0 || strlen(valueStr) % lengthToCheck != 0) {
    return false;
  }
  bool isDuplicated = true;
  for (int i = 0; i < lengthToCheck; i++) {
    for (int j = 0; j < strlen(valueStr) / lengthToCheck; j++) {
      if (valueStr[i] != valueStr[i + j * lengthToCheck]) {
        isDuplicated = false;
        break;
      }
    }
  }
  return isDuplicated;
}

// int main(void) {
//   int num_lines = 0;
//   char **lines = read_file_lines("./02/input.txt", &num_lines);
//   char *line = lines[0];
//   char **ranges = malloc(num_lines * sizeof(char *));
//   int range_count = 0;
//   long long sum = 0;
//   unsigned long long sum2 = 0;
//   char *token = strtok(line, ",");
//
//   while (token != NULL) {
//     ranges[range_count] = token;
//     token = strtok(NULL, ",");
//     range_count++;
//   }
//
//   for (int i = 0; i < range_count; i++) {
//     char *copy = malloc((strlen(ranges[i]) + 1) * sizeof(char));
//     strcpy(copy, ranges[i]);
//
//     printf("Processing Range %d/%d: %s\n", i + 1, range_count, copy);
//     long long minRange = atoll(strtok(copy, "-"));
//     long long maxRange = atoll(strtok(NULL, "-"));
//
//     for (long long val = minRange; val <= maxRange; val++) {
//       char valueStr[24];
//       sprintf(valueStr, "%lld", val);
//       if (checkValuePart1(valueStr)) {
//         sum += val;
//       }
//
//       for (int len = 1; len <= strlen(valueStr) / 2; len++) {
//         if (checkValuePart2(valueStr, len)) {
//           printf("  Found Part 2 Duplicate: %s (len=%d)\n", valueStr, len);
//           sum2 += val;
//           break;
//         }
//       }
//     }
//     free(copy);
//   }
//   printf("Sum of Invalid Values Part 1: %lld\n", sum);
//   printf("Sum of Invalid Values Part 2: %llu\n", sum2);
//   free_file_lines(lines, num_lines);
//   return 0;
// }

// gcc -Wall -Wextra -g3 -I./lib ./08/08.c ./lib/base.c -o ./08/e.exe
// ./01/e.exe