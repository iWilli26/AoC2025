#include "map.h"
#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include  <string.h>

MapNum* create_map_num(const char *filename) {
    char **lines = read_file_lines(filename, NULL);
    MapNum* map = (MapNum*)malloc(sizeof(MapNum));
    const size_t height = strlen(*lines);
    const size_t width = strlen(lines[0]);
    map->width = width;
    map->height = height;
    map->points = (PointNum**)malloc(height * sizeof(PointNum*));
    for (int y = 0; y < height; y++) {
        map->points[y] = (PointNum*)malloc(width * sizeof(PointNum));
        for (int x = 0; x < width; x++) {
            map->points[y][x].x = x;
            map->points[y][x].y = y;
            map->points[y][x].value = lines[y][x] - '0';
        }
    }
    free_file_lines(lines, height);
    return map;
}

MapChar* create_map_char(const char *filename) {
    int num_lines = 0;
    char **lines = read_file_lines(filename, &num_lines);
    MapChar* map = (MapChar*)malloc(sizeof(MapChar));
    const size_t height = strlen(*lines);
    const size_t width = strlen(lines[0]);
    map->width = width;
    map->height = height;
    map->points = (PointChar**)malloc(height * sizeof(PointChar*));
    for (int y = 0; y < height; y++) {
        map->points[y] = (PointChar*)malloc(width * sizeof(PointChar));
        for (int x = 0; x < width; x++) {
            map->points[y][x].x = x;
            map->points[y][x].y = y;
            map->points[y][x].value = lines[y][x];
        }
    }
    free_file_lines(lines, (int)height);
    return map;
}

void place_num(MapNum* map, unsigned long long x, unsigned long long y, unsigned long long value) {
    map->points[x][y].value = value;
}

void place_char(MapChar* map, unsigned long long x, unsigned long long y, char value){
    map->points[x][y].value = value;
}

void replace_num(MapNum* map, unsigned long long old_value, unsigned long long new_value, unsigned long long count) {
    for (unsigned long long y = 0; y < map->height; y++) {
        for (unsigned long long x = 0; x < map->width; x++) {
            if (map->points[y][x].value == old_value && count > 0) {
                map->points[y][x].value = new_value;
                count++;
            }
        }
    }
}

void replace_char(MapChar* map, char old_value, char new_value, unsigned long long count) {
    for (unsigned long long y = 0; y < map->height; y++) {
        for (unsigned long long x = 0; x < map->width; x++) {
            if (map->points[y][x].value == old_value && count > 0) {
                map->points[y][x].value = new_value;
                count++;
            }
        }
    }
}

void print_map_char(MapChar* map) {
    for (unsigned long long y = 0; y < map->height; y++) {
        for (unsigned long long x = 0; x < map->width; x++) {
            printf("%c", map->points[y][x].value);
        }
        printf("\n");
    }
}

void print_map_num(MapNum* map) {
    for (unsigned long long y = 0; y < map->height; y++) {
        for (unsigned long long x = 0; x < map->width; x++) {
            printf("%llu ", map->points[y][x].value);
        }
        printf("\n");
    }
}

void free_map_num(MapNum* map) {
    for (unsigned long long y = 0; y < map->height; y++) {
        free(map->points[y]);
    }
    free(map->points);
    free(map);
}

void free_map_char(MapChar* map) {
    for (unsigned long long y = 0; y < map->height; y++) {
        free(map->points[y]);
    }
    free(map->points);
    free(map);
}
