#include "../lib/base.h"
#include "../lib/map.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int id;
  int x;
  int y;
  int z;
} Point3D;

typedef struct {
  char *hash;
  size_t dist;
} DistHash;

size_t calc_dist(Point3D a, Point3D b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) +
              (a.z - b.z) * (a.z - b.z));
}

bool isInHash(DistHash *dist_hashes, size_t dist_hash_count,
              size_t target_dist) {
  for (size_t i = 0; i < dist_hash_count; i++) {
    if (dist_hashes[i].dist == target_dist) {
      return true;
    }
  }
  return false;
}

char *hash_points(Point3D a, Point3D b) {
  size_t hash_size = 50;
  char *hash = (char *)malloc(hash_size * sizeof(char));
  snprintf(hash, hash_size, "%d,%d", a.id, b.id);
  return hash;
}

size_t add_hash(DistHash *dist_hashes, size_t *dist_hash_count, Point3D a,
                Point3D b) {
  if (isInHash(dist_hashes, *dist_hash_count, calc_dist(a, b))) {
    return dist_hashes[*dist_hash_count - 1].dist;
  }
  DistHash new_hash;
  size_t dist = calc_dist(a, b);
  new_hash.hash = hash_points(a, b);
  new_hash.dist = dist;
  dist_hashes[(*dist_hash_count)++] = new_hash;
  return dist;
}

int find_closest(Point3D point, Point3D *points, size_t num_points,
                 DistHash *dist_hashes, size_t *dist_hash_count) {
  size_t min_dist = SIZE_MAX;
  int closest_id = -1;
  for (size_t i = 0; i < num_points; i++) {
    if (points[i].id == point.id) {
      continue;
    }

    size_t dist = add_hash(dist_hashes, dist_hash_count, point, points[i]);

    if (dist < min_dist) {
      min_dist = dist;
      closest_id = points[i].id;
    }
  }
  return closest_id;
}

int ex81() {
  int num_lines = 0;
  char **lines = read_file_lines("./08/input.txt", &num_lines);
  DistHash *dist_hashes = (DistHash *)malloc(num_lines * sizeof(DistHash));
  int width = strlen(lines[0]);
  Point3D *points = (Point3D *)malloc(num_lines * sizeof(Point3D));
  size_t point_count = 0;
  for (int i = 0; i < num_lines; i++) {
    points[i].id = i;
    char *token = strtok(lines[i], ",");
    points[i].x = atoi(token);
    token = strtok(NULL, ",");
    points[i].y = atoi(token);
    token = strtok(NULL, ",");
    points[i].z = atoi(token);
  }
  printf("\n");

  int test =
      find_closest(points[0], points, num_lines, dist_hashes, &point_count);

  printf("Closest to point 0: id %d\n", test);
  Point3D target_point = points[test];
  printf("Closest is %d, %d, %d, %d", target_point.x, target_point.y,
         target_point.z, target_point.id);

  free_file_lines(lines, num_lines);
  free(points);
  return 0;
}