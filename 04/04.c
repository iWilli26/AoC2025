#include "map.h"
#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool checkPoint(PointChar point, MapChar *map) {
    int rolls_count = 0;
    for (int dy =-1 ; dy <=1; dy++) {
        for (int dx =-1; dx <=1; dx++) {
            long long check_x = point.x + dx;;
            long long check_y = point.y + dy;
            if (check_x < 0 || check_x >= map->width || check_y < 0 || check_y >= map->height) {
                continue;
            }
            if (map->points[check_y][check_x].value == '@') {
                rolls_count++;
            }
            if (rolls_count>4) {
                return false;
            }
        }
    }
    printf("%llu, %llu\n", point.x, point.y);
    return true;
}

unsigned long long remove_rolls(MapChar *map) {
    unsigned long long removed_rolls = 0;
    for (int i=0; i< map->height; i++) {
        for (int j=0; j< map->width; j++) {
            if (map->points[i][j].value == '@') {
                if (checkPoint(map->points[i][j], map)) {
                    map->points[i][j].value = '.';
                    removed_rolls++;
                }
            }
        }
    }
    return removed_rolls;
}

int ex41() {
    MapChar *map = create_map_char("..\\04\\input.txt");
    int accessible_rolls = 0;
    int num_rolls = 0;
    for (int i=0; i< map->height; i++) {
        for (int j=0; j< map->width; j++) {
            if (map->points[i][j].value == '@') {
                if (checkPoint(map->points[i][j], map)) {
                    accessible_rolls++;
                }
                num_rolls++;
            }
        }
    }
    free_map_char(map);
    printf("%d rolls, %d accessible rolls\n",num_rolls, accessible_rolls);
    return 0;
}

unsigned long long ex42() {
    MapChar *map = create_map_char("..\\04\\input.txt");
    unsigned long long res=0;
    while (true) {
        unsigned long long removed_count = remove_rolls(map);
        res+=removed_count;
        if (removed_count==0) {
            break;
        }
    }
    free_map_char(map);
    printf("Removed rolls: %llu\n", res);
    return res;
}