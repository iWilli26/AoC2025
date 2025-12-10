#include "../lib/base.h"
#include "../lib/map.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  bool *targetLights;
  int **lightsPerButton;
  int *sizeLightsPerButton;
  int numButtons;
  int *otherState;
} Machine;

void parseLine(char *line, Machine *machine) {
  machine->targetLights = NULL;
  machine->lightsPerButton = malloc(100 * sizeof(int *));
  machine->sizeLightsPerButton = malloc(100 * sizeof(int));
  machine->otherState = NULL;
  machine->numButtons = 0;
  char **buttons = malloc(100 * sizeof(char *));
  char *lineCopy = strdup(line);
  char *token = strtok(lineCopy, " ");
  machine->targetLights = malloc((strlen(token) - 2) * sizeof(int));
  int index = 0;
  for (size_t i = 1; i < strlen(token) - 1; i++) {
    machine->targetLights[index++] = token[i] == '.' ? false : true;
  }
  int numButtons = 0;
  while (token != NULL) {
    token = strtok(NULL, " ");
    while (token != NULL && token[0] != '{') {
      buttons[numButtons] = strdup(token);
      token = strtok(NULL, " ");
      numButtons++;
    }
  }
  machine->numButtons = numButtons - 1;
  free(lineCopy);

  for (int i = 0; i < machine->numButtons; i++) {
    printf("Button %d: %s\n", i + 1, buttons[i]);
    char *tokenBtn = strtok(buttons[i], ",");
    int lightCount = 0;
    int *lights = malloc(100 * sizeof(int));
    while (tokenBtn != NULL) {
      if (lightCount == 0) {
        tokenBtn++;
      }
      lights[lightCount++] = atoi(tokenBtn);
      printf("  Light: %d   %s\n", lights[lightCount - 1], tokenBtn);
      tokenBtn = strtok(NULL, ",");
    }
    machine->lightsPerButton[i] = malloc(lightCount * sizeof(int));
    memcpy(machine->lightsPerButton[i], lights, lightCount * sizeof(int));

    machine->sizeLightsPerButton[i] = lightCount;

    free(lights);
    free(buttons[i]);
  }

  printf("Parsed Machine with %d buttons. Target Lights: ",
         machine->numButtons);

  for (int i = 0; i < index; i++) {
    printf("%d ", machine->targetLights[i]);
  }
  printf("\n");
}

int ex101() {
  int num_lines;
  char **lines = read_file_lines("./10/input.txt", &num_lines);
  Machine *machines = malloc(num_lines * sizeof(Machine));

  for (int i = 0; i < num_lines; i++) {
    parseLine(lines[i], &machines[i]);
  }

  return 0;
}

int main(void) { return ex101(); }