//
// Created by willi on 04/12/2025.
//

#ifndef AOC2025_MAP_H
#define AOC2025_MAP_H

typedef struct {
    unsigned long long x;
    unsigned long long y;
    char value;
} PointChar;

typedef struct {
    unsigned long long x;
    unsigned long long y;
    unsigned long long value;
} PointNum;

typedef struct {
    unsigned long long width;
    unsigned long long height;
    PointNum **points;
} MapNum;

typedef struct {
    unsigned long long width;
    unsigned long long height;
    PointChar **points;
} MapChar;

void place_num(MapNum* map, unsigned long long x, unsigned long long y, unsigned long long value);

void place_char(MapChar* map, unsigned long long x, unsigned long long y, char value);

void replace_num(MapNum* map, unsigned long long old_value, unsigned long long new_value, unsigned long long count);

void replace_char(MapChar* map, char old_value, char new_value, unsigned long long count);

MapNum* create_map_num(const char *filename);
void free_map_num(MapNum* map);

MapChar* create_map_char(const char *filename);
void free_map_char(MapChar* map);

void print_map_char(MapChar* map);

void print_map_num(MapNum* map);

#endif //AOC2025_MAP_H