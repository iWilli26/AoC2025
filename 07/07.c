#include <stdbool.h>

#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

long long descending_beam(const long long x,const  long long  y,const MapChar *map, long long count) {
    if (y>= map->height - 1 || x >= map->width -1 || x<0 || y<0) {
        return count;
    }
    if (map->points[y+1][x].value == '.') {
        map->points[y+1][x].value='|';
        return descending_beam(x, y+1, map, count);
    }
    if (map->points[y+1][x].value == '^') {
        map->points[y+1][x+1].value='|';
        map->points[y+1][x-1].value='|';
        const long long left_count = descending_beam(x-1, y+1, map, count);
        const long long right_count = descending_beam(x+1, y+1, map, count);
        return left_count + right_count + 1;
    }
    return count;
}

size_t ex71() {
    MapChar *map = create_map_char(("..\\07\\input.txt"));
    print_map_char(map);
    PointChar *S = (PointChar*)malloc(sizeof(PointChar));

    find_char(S, map, 'S');
    printf("Start position: (%zu, %zu)\n", S->x, S->y);
    printf("%zu, %zu, %c\n", map->points[7][2].x, map->points[7][2].y, map->points[7][2].value);
    long long res = descending_beam(S->x, S->y, map, 0);
    print_map_char(map);
    long long count = 0;
    for (long long x = 0; x < map->width; x++) {
        count += (map->points[map->height-1][x].value == '|') ? 1 : 0;
    }
    free_map_char(map);
    printf("Result: %lld\n", res);
    return 0;
}

typedef struct {
    char **paths;
    size_t *hashes;
} PathSet;


size_t hash_string(const char *str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

char** descending_beam2(const long long x, const long long y, const MapChar *map, char *path,
                        char **paths, size_t *path_count, size_t *max_paths,
                        size_t **path_hashes, size_t depth) {
    if (depth > 10000) {
        fprintf(stderr, "Max depth reached at (%lld, %lld)\n", x, y);
        return paths;
    }

    if (*path_count >= *max_paths) {
        *max_paths *= 2;
        paths = (char**)realloc(paths, (*max_paths) * sizeof(char*));
        *path_hashes = (size_t*)realloc(*path_hashes, (*max_paths) * sizeof(size_t));
        if (paths == NULL || *path_hashes == NULL) {
            fprintf(stderr, "Realloc failed!\n");
            exit(1);
        }
    }

    if (y >= map->height - 1 || x >= map->width - 1 || x < 0 || y < 0) {
        size_t hash = hash_string(path);
        bool found = false;
        for (size_t i = 0; i < *path_count; i++) {
            if ((*path_hashes)[i] == hash && strcmp(paths[i], path) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            paths[*path_count] = strdup(path);
            *path_hashes[*path_count] = hash;
            (*path_count)++;
        }
        return paths;
    }

    char next_value = map->points[y+1][x].value;

    if (next_value == '.') {
        return descending_beam2(x, y+1, map, path, paths, path_count, max_paths, path_hashes, depth + 1);
    }else if (next_value == '^') {
        char *left_path = (char*)malloc(strlen(path) + 2);
        char *right_path = (char*)malloc(strlen(path) + 2);
        strcpy(left_path, path);
        strcpy(right_path, path);
        strcat(left_path, "L");
        strcat(right_path, "R");

        paths = descending_beam2(x-1, y+1, map, left_path, paths, path_count, max_paths, path_hashes, depth + 1);
        paths = descending_beam2(x+1, y+1, map, right_path, paths, path_count, max_paths, path_hashes, depth + 1);

        free(left_path);
        free(right_path);
    }
    else {
        size_t hash = hash_string(path);
        bool found = false;
        for (size_t i = 0; i < *path_count; i++) {
            if ((*path_hashes)[i] == hash && strcmp(paths[i], path) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            paths[*path_count] = strdup(path);
            (*path_hashes)[*path_count] = hash;
            (*path_count)++;
        }
    }

    return paths;
}

size_t ex72() {
    size_t max_paths = 10000;
    char **paths = malloc(max_paths * sizeof(char*));
    size_t *path_hashes = malloc(max_paths * sizeof(size_t));
    size_t path_count = 0;
    MapChar *map = create_map_char(("..\\07\\input.txt"));
    PointChar *S = malloc(sizeof(PointChar));
    find_char(S, map, 'S');

    printf("Starting from position: (%zu, %zu)\n", S->x, S->y);
    paths = descending_beam2(S->x, S->y, map, "", paths, &path_count, &max_paths, &path_hashes, 0);

    printf("Total unique paths found: %zu\n", path_count);

    for (size_t i = 0; i < path_count; i++) {
        free(paths[i]);
    }
    free(paths);
    free(path_hashes);
    free_map_char(map);
    free(S);

    printf("Total unique paths: %zu\n", path_count);
    return path_count;
}

// Structure pour représenter un bloc avec sa position et sa valeur
typedef struct {
    long long x;
    long long y;
    long long value;
} Block;

long long descending_beam3(MapChar *map) {
    PointChar *S = malloc(sizeof(PointChar));
    find_char(S, map, 'S');

    printf("Starting from position: (%zu, %zu)\n", S->x, S->y);

    long long **blocks = (long long**)malloc(map->height * sizeof(long long*));
    for (size_t i = 0; i < map->height; i++) {
        blocks[i] = calloc(map->width, sizeof(long long));
    }

    blocks[S->y][S->x] = 1;

    for (long long y = S->y; y < map->height - 1; y++) {
        long long *next_row = calloc(map->width, sizeof(long long));
        for (long long x = 0; x < map->width; x++) {
            if (blocks[y][x] == 0) continue;

            long long current_value = blocks[y][x];
            char next_cell = map->points[y+1][x].value;

            if (next_cell == '.') {
                next_row[x] += current_value;
            } else if (next_cell == '^') {
                if (x - 1 >= 0) {
                    next_row[x-1] += current_value;
                }
                if (x + 1 < map->width) {
                    next_row[x+1] += current_value;
                }
            }
        }
        for (long long x = 0; x < map->width; x++) {
            blocks[y+1][x] += next_row[x];
        }
        free(next_row);
    }

    long long total = 0;
    printf("Final row blocks: ");
    for (long long x = 0; x < map->width; x++) {
        if (blocks[map->height - 1][x] > 0) {
            printf("colonne %lld=%lld ", x, blocks[map->height - 1][x]);
            total += blocks[map->height - 1][x];
        }
    }
    printf("\n");

    for (size_t i = 0; i < map->height; i++) {
        free(blocks[i]);
    }
    free(blocks);
    free(S);

    return total;
}

size_t ex73() {
    MapChar *map = create_map_char(("..\\07\\input.txt"));
    print_map_char(map);

    long long result = descending_beam3(map);

    printf("Total sum of blocks at bottom: %lld\n", result);

    free_map_char(map);
    return result;
}

