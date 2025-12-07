#include <stdbool.h>

#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t ex61() {
    int num_lines = 0;
    size_t res=0;
    char **lines = read_file_lines("..\\06\\input.txt", &num_lines);
    printf("num_lines = %d\n", num_lines);
    char ***parsed_lines = malloc(num_lines * sizeof(char **));
    int parsed_lines_count = 0;
    int array_size = 0;
    for (int i = 0; i < num_lines; i++) {
        const int size = 10000;
        char **array = malloc(size * sizeof(char*));
        array_size = 0;
        char *line = lines[i];
        char *line_copy = strdup(line);

        const char *token = strtok(line_copy, " ");
        while (token != NULL) {
            array[array_size] = token;
            array_size++;
            token = strtok(NULL, " ");
        }
        parsed_lines[parsed_lines_count] = array;
        parsed_lines_count++;
    }

    printf("test");

    for (int j=0;j<array_size;j++) {
        size_t *numArray = malloc(parsed_lines_count * sizeof(size_t));
        int i=0;
        printf("Colonne %d:\n", j);
        for (; i < parsed_lines_count - 1; i++) {
            numArray[i] = strtoull(parsed_lines[i][j], NULL, 10);
            printf("%zu  ", numArray[i]);
        }

        printf("%c\n", parsed_lines[i][j][0]);
        size_t subRes = 0;
        if (parsed_lines[i][j][0] == '*') {
            subRes=1;
            for (int k=0;k<i;k++) {
                subRes = subRes * numArray[k];
            }
        }
        if (parsed_lines[i][j][0] == '+') {
            for (int k=0;k<i;k++) {
                subRes = subRes + numArray[k];
            }
        }
        printf("subRes = %zu\n", subRes);
        res += subRes;

        free(numArray);
    }
    free_file_lines(lines, num_lines);
    free(parsed_lines);
    printf("%zu\n", res);
    return res;
}

size_t ex62() {
    int num_lines = 0;
    size_t res=0;
    char **lines = read_file_lines("..\\06\\input.txt", &num_lines);
    printf("num_lines = %d\n", num_lines);
    char ***parsed_lines = malloc(num_lines * sizeof(char **));
    int parsed_lines_count = 0;
    int array_size = 0;
    for (int i = 0; i < num_lines; i++) {
        const int size = 10000;
        char **array = malloc(size * sizeof(char*));
        array_size = 0;
        char *line = lines[i];
        char *line_copy = strdup(line);

        const char *token = strtok(line_copy, " ");
        while (token != NULL) {
            array[array_size] = token;
            array_size++;
            token = strtok(NULL, " ");
        }
        parsed_lines[parsed_lines_count] = array;
        parsed_lines_count++;
    }

    char ***parsed_columns = malloc(array_size * sizeof(char **));

    for (int j=0;j<array_size;j++) {
        char **numArray = malloc(parsed_lines_count * sizeof(size_t));
        int i=0;
        for (; i < parsed_lines_count; i++) {
            numArray[i] = parsed_lines[i][j];
        }
        parsed_columns[j] = numArray;
    }

    for (int i=0;i<array_size;i++) {
        int maxLength = 0;
        for (int j=0;j<num_lines;j++) {
            const int len = strlen(parsed_columns[i][j]);
            if (len > maxLength) {
                maxLength = len;
            }
        }
        for (int j=0;j<num_lines - 1;j++) {
            const int valueLength = strlen(parsed_columns[i][j]);
            if(valueLength < maxLength) {
                char *new_str = malloc((valueLength + maxLength) * sizeof(char));
                const int diff = maxLength - valueLength;
                for (int k=0;k<diff;k++) {
                    new_str[k] = '0';
                }
                new_str[diff] = '\0';
                strcat(new_str, parsed_columns[i][j]);
                parsed_columns[i][j] = new_str;
            }
        }
    }

    // for (int i=0;i<array_size;i++) {
    //     printf("Colonne %d after padding:\n", i);
    //     for (int j=0;j<num_lines;j++) {
    //         printf("%s  ", parsed_columns[i][j]);
    //     }
    //     printf("\n");
    // }

    for (int i=0;i<array_size;i++) {
        size_t subRes = 0;
        if (parsed_columns[i][num_lines - 1][0] == '*') {
            subRes=1;
            for (int k=0;k<num_lines - 1;k++) {
                char *verticalString = malloc(num_lines - 1 * sizeof(char));
                int verticalStringIndex = 0;
                for (int h=0;h<strlen(parsed_columns[i][k])+1;h++) {
                    verticalString[verticalStringIndex] = parsed_columns[i][h][k];
                    verticalStringIndex++;
                }
                printf("%s\n", verticalString);
                const size_t val = strtoull(verticalString, NULL, 10);

                subRes = subRes * val;

            }
        }
        if (parsed_columns[i][num_lines - 1][0] == '+') {
            for (int k=0;k<num_lines - 1;k++) {
                char *verticalString = malloc(num_lines - 1 * sizeof(char));
                const int verticalStringIndex = 0;
                for (int h=strlen(parsed_columns[i][k]);h>0;h--) {
                    verticalString[verticalStringIndex] = parsed_columns[i][k][h-1];
                }
                // printf("%s ", verticalString);
                const size_t val = strtoull(verticalString, NULL, 10);
                subRes = subRes + val;
                free(verticalString);
            }
        }
        // printf("subRes = %zu\n", subRes);
        res += subRes;
    }

    printf("Resultat %zu", res);
    free_file_lines(lines, num_lines);

    return 0;
}