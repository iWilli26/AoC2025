#include <stdbool.h>

#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t minRange;
    size_t maxRange;
} Range;

bool ranges_overlap_or_adjacent(Range a, Range b) {
    if (a.minRange <= b.maxRange && b.minRange <= a.maxRange) {
        return true;
    }
    if (b.minRange > 0 && a.maxRange == b.minRange - 1) {
        return true;
    }
    if (a.minRange > 0 && b.maxRange == a.minRange - 1) {
        return true;
    }
    return false;
}

int ex51() {
    int num_lines = 0;
    char **lines = read_file_lines("..\\05\\input.txt", &num_lines);
    int capacity = 1000;
    Range *ranges = malloc(capacity * sizeof(Range));
    int range_count = 0;

    int i=0;
    for (; i < num_lines; i++) {
        char *line = lines[i];
        if (strcmp(line, "\0") == 0)  {
            printf("Empty line found, stopping processing.\n");
            i++;
            break;
        }

        const char *token = strtok(line, "-");
        const char *nextToken = strtok(NULL, "-");

        ranges[range_count].minRange = strtoull(token, NULL, 10);
        ranges[range_count].maxRange = strtoull(nextToken, NULL, 10);
        range_count++;
    }
    int count = 0;
    for (; i<num_lines; i++) {
        char *line = lines[i];
        size_t value = strtoull(line, NULL, 10);
        for (int j=0; j < range_count; j++) {
            if (value >= ranges[j].minRange && value <= ranges[j].maxRange) {
                printf("Value %zu is in range %zu-%zu\n", value, ranges[j].minRange, ranges[j].maxRange);
                count++;
                break;
            }
        }
    }
    printf("Result: %d\n", count);
    free(ranges);
    free_file_lines(lines, num_lines);
    return 0;
}

size_t merge_ranges(Range **ranges, size_t *range_count) {
    Range *merged_ranges = malloc(*range_count * sizeof(Range));
    if (merged_ranges == NULL) {
        return 0;
    }

    size_t merged_count = 0;
    size_t nb_of_merges = 0;

    for (size_t i = 0; i < *range_count; i++) {
        Range current = (*ranges)[i];
        bool merged = false;

        for (size_t j = 0; j < merged_count; j++) {
            if (ranges_overlap_or_adjacent(merged_ranges[j], current)) {
                size_t new_min = (current.minRange < merged_ranges[j].minRange) ?
                                  current.minRange : merged_ranges[j].minRange;
                size_t new_max = (current.maxRange > merged_ranges[j].maxRange) ?
                                  current.maxRange : merged_ranges[j].maxRange;

                merged_ranges[j].minRange = new_min;
                merged_ranges[j].maxRange = new_max;
                merged = true;
                nb_of_merges++;
                break;
            }
        }

        if (!merged) {
            merged_ranges[merged_count] = current;
            merged_count++;
        }
    }

    for (size_t i = 0; i < merged_count; i++) {
        (*ranges)[i] = merged_ranges[i];
    }

    Range *temp = realloc(*ranges, merged_count * sizeof(Range));
    if (temp != NULL) {
        *ranges = temp;
    }

    *range_count = merged_count;
    free(merged_ranges);
    return nb_of_merges;
}

unsigned long long ex52() {
    int num_lines = 0;
    char **lines = read_file_lines("..\\05\\input.txt", &num_lines);
    Range *ranges = malloc(1000 * sizeof(Range));
    size_t range_count = 0;

    for (int i=0; i < num_lines; i++) {
        char *line = lines[i];
        if (strcmp(line, "\0") == 0)  {
            printf("Empty line found, stopping processing.\n");
            break;
        }

        const char *token = strtok(line, "-");
        const char *nextToken = strtok(NULL, "-");

        ranges[range_count].minRange = strtoull(token, NULL, 10);
        ranges[range_count].maxRange = strtoull(nextToken, NULL, 10);
        range_count++;
    }
    printf("////////////////\n");
    for (size_t i=0; i < range_count; i++) {
        printf("Range %zu: %zu-%zu\n", i, ranges[i].minRange, ranges[i].maxRange);
    }
    printf("\n");
    int iter = 0;
    while (true) {
        const size_t merges = merge_ranges(&ranges, &range_count);
        printf("Iteration %d: Merges = %zu\n", iter, merges);
        printf("////////////////\n");
        for (size_t i=0; i < range_count; i++) {
            printf("Range %zu: %zu-%zu\n", i, ranges[i].minRange, ranges[i].maxRange);
        }
        printf("\n");
        iter++;
        if (merges == 0) {
            break;
        }
    }

    size_t count = 0;
    for (int i=0; i < range_count; i++) {
        printf("Range %d: %zu-%zu\n", i, ranges[i].minRange, ranges[i].maxRange);
        count += (ranges[i].maxRange - ranges[i].minRange + 1);
        if (ranges[i].maxRange == ranges[i].minRange) {
            count++;
        }
    }
    printf("%zu\n", range_count);
    printf("Result: %zu\n", count);

    free(ranges);
    free_file_lines(lines, num_lines);
    return 0;
}