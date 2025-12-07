//
// Created by willi on 04/12/2025.
//

#ifndef AOC2025_MAP_H
#define AOC2025_MAP_H

typedef struct {
    long long x;
    long long y;
    char value;
} PointChar;

typedef struct {
    long long x;
    long long y;
    long long value;
} PointNum;

typedef struct {
    long long width;
    long long height;
    PointNum **points;
} MapNum;

typedef struct {
    long long width;
    long long height;
    PointChar **points;
} MapChar;

void place_num(MapNum* map, long long x, long long y, long long value);

void place_char(MapChar* map, long long x, long long y, char value);

void replace_num(MapNum* map, long long old_value, long long new_value, unsigned long long count);

void replace_char(MapChar* map, char old_value, char new_value, unsigned long long count);

MapNum* create_map_num(const char *filename);
void free_map_num(MapNum* map);

MapChar* create_map_char(const char *filename);
void free_map_char(MapChar* map);

void print_map_char(MapChar* map);

void print_map_num(MapNum* map);

void find_char(PointChar *res, const MapChar* map,const char target);

#endif //AOC2025_MAP_H