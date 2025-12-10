#include "../lib/base.h"
#include "../lib/map.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int id;
  size_t x;
  size_t y;
  size_t z;
} Point3D;

typedef struct {
  int id_a;
  int id_b;
  size_t dist;
} DistHash;

size_t calc_dist(Point3D a, Point3D b) {
  double dx = (double)((long long)a.x - (long long)b.x);
  double dy = (double)((long long)a.y - (long long)b.y);
  double dz = (double)((long long)a.z - (long long)b.z);
  double d = sqrt(dx * dx + dy * dy + dz * dz);
  return (size_t)d;
}

size_t find_hash_dist(DistHash *dist_hashes, size_t dist_hash_count,
                      size_t target_dist) {
  for (size_t i = 0; i < dist_hash_count; i++) {
    if (dist_hashes[i].dist == target_dist) {
      return dist_hashes[i].dist;
    }
  }
  return SIZE_MAX;
}


size_t add_hash(DistHash **dist_hashes_ptr, size_t *dist_hash_count,
                size_t *dist_hash_capacity, Point3D a, Point3D b) {
  size_t dist = calc_dist(a, b);
  size_t existing = find_hash_dist(*dist_hashes_ptr, *dist_hash_count, dist);
  if (existing != SIZE_MAX) {
    return existing;
  }

  if (*dist_hash_count >= *dist_hash_capacity) {
    *dist_hash_capacity *= 2;
    DistHash *tmp = realloc(*dist_hashes_ptr, *dist_hash_capacity * sizeof(DistHash));
    if (!tmp) {
      perror("realloc");
      exit(EXIT_FAILURE);
    }
    *dist_hashes_ptr = tmp;
  }

  DistHash new_hash;
  new_hash.id_a = a.id;
  new_hash.id_b = b.id;
  new_hash.dist = dist;
  (*dist_hashes_ptr)[(*dist_hash_count)++] = new_hash;
  return dist;
}

int find_closest(Point3D point, Point3D *points, size_t num_points,
                 DistHash **dist_hashes, size_t *dist_hash_count,
                 size_t *dist_hash_capacity) {
  size_t min_dist = SIZE_MAX;
  int closest_id = -1;
  for (size_t i = 0; i < num_points; i++) {
    if (points[i].id == point.id) {
      continue;
    }

    size_t dist = add_hash(dist_hashes, dist_hash_count, dist_hash_capacity,
                           point, points[i]);

    if (dist < min_dist) {
      min_dist = dist;
      closest_id = points[i].id;
    }
  }
  return closest_id;
}

int ex81() {
  clock_t start_total = clock();
  clock_t start, end;

  start = clock();
  int num_lines = 0;
  char **lines = read_file_lines("..\\08\\input.txt", &num_lines);
  end = clock();
  printf("Lecture du fichier: %.3f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);

  size_t dist_hash_capacity = num_lines * num_lines;
  DistHash *dist_hashes = (DistHash *)malloc(dist_hash_capacity * sizeof(DistHash));
  Point3D *points = (Point3D *)malloc(num_lines * sizeof(Point3D));
  size_t point_count = 0;

  start = clock();
  for (int i = 0; i < num_lines; i++) {
    points[i].id = i;
    char *line_copy = strdup(lines[i]);
    char *token = strtok(line_copy, ",");
    points[i].x = strtoull(token, NULL, 10);
    token = strtok(NULL, ",");
    points[i].y = strtoull(token, NULL, 10);
    token = strtok(NULL, ",");
    points[i].z = strtoull(token, NULL, 10);
    free(line_copy);
    point_count++;
  }
  printf("\n");
  end = clock();
  printf("Parsing des points: %.3f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);

  size_t dist_hash_count = 0;

  start = clock();
  for (size_t i = 0; i < point_count; i++) {
    find_closest(points[i], points, point_count, &dist_hashes,
                 &dist_hash_count, &dist_hash_capacity);
  }
  end = clock();
  printf("Calcul des distances: %.3f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);

  start = clock();
  for (size_t i = 0; i < dist_hash_count - 1; i++) {
    for (size_t j = i + 1; j < dist_hash_count; j++) {
      if (dist_hashes[i].dist > dist_hashes[j].dist) {
        DistHash temp = dist_hashes[i];
        dist_hashes[i] = dist_hashes[j];
        dist_hashes[j] = temp;
      }
    }
  }
  end = clock();
  printf("Tri des distances: %.3f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);

  start = clock();
  int **circuits = (int **)malloc(sizeof(int *) * num_lines);
  int circuit_count = 1;
  int circuit_capacity = num_lines;
  int *circuit_size = (int *)malloc(circuit_capacity * sizeof(int));
  circuits[0] = (int *)malloc(sizeof(int) * num_lines);
  circuits[0][0] = dist_hashes[0].id_a;
  circuits[0][1] = dist_hashes[0].id_b;
  circuit_size[0] = 2;

  for (size_t i = 1; i < 1000; i++) {
      const int id_a = dist_hashes[i].id_a;
      const int id_b = dist_hashes[i].id_b;
      int circuit_a = -1;
      int circuit_b = -1;

      for (int j = 0; j < circuit_count; j++) {
        for (int k = 0; k < circuit_size[j]; k++) {
          if (circuits[j][k] == id_a) {
            circuit_a = j;
          }
          if (circuits[j][k] == id_b) {
            circuit_b = j;
          }
        }
      }

      if (circuit_a != -1 && circuit_b != -1) {
        if (circuit_a != circuit_b) {
          for (int k = 0; k < circuit_size[circuit_b]; k++) {
            bool already_exists = false;
            for (int m = 0; m < circuit_size[circuit_a]; m++) {
              if (circuits[circuit_a][m] == circuits[circuit_b][k]) {
                already_exists = true;
                break;
              }
            }
            if (!already_exists) {
              circuits[circuit_a][circuit_size[circuit_a]] = circuits[circuit_b][k];
              circuit_size[circuit_a]++;
            }
          }
          free(circuits[circuit_b]);
          for (int k = circuit_b; k < circuit_count - 1; k++) {
            circuits[k] = circuits[k + 1];
            circuit_size[k] = circuit_size[k + 1];
          }
          circuit_count--;
        }
      } else if (circuit_a != -1) {
        circuits[circuit_a][circuit_size[circuit_a]] = id_b;
        circuit_size[circuit_a]++;
      } else if (circuit_b != -1) {
        circuits[circuit_b][circuit_size[circuit_b]] = id_a;
        circuit_size[circuit_b]++;
      } else {
        circuits[circuit_count] = (int *)malloc(sizeof(int) * num_lines);
        circuits[circuit_count][0] = id_a;
        circuits[circuit_count][1] = id_b;
        circuit_size[circuit_count] = 2;
        circuit_count++;
      }
  }
  end = clock();
  printf("Creation des circuits: %.3f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);

  int res = 1;
  for (int i=0; i < 3; i++) {
    printf("Circuit %d: ", i);
    for (int j=0;j < circuit_size[i]; j++) {
      printf("%d ", circuits[i][j]);
    }
    res *= circuit_size[i];
    printf("\n");
  }


  printf("Resultat %d\n", res);

  clock_t end_total = clock();
  printf("\n=== TEMPS TOTAL: %.3f ms ===\n", (double)(end_total - start_total) * 1000.0 / CLOCKS_PER_SEC);

  free(dist_hashes);
  free(circuit_size);
  free(circuits);
  free_file_lines(lines, num_lines);
  free(points);
  return 0;
}