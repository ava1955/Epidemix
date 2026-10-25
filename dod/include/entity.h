#pragma once
#include <stdbool.h>
#include "dod/include/benchmark.h"
#include <raylib.h>
#include <math.h>

typedef struct {
    float* pos_x;
    float* pos_y;
    float* directionVector;
    bool* canMove;
    float* radius;
    bool* infected;
    float* infectedTime;
    int count;
    int capacity;
} DODState;

void initDODState(DODState* state, int initialCapacity);
void freeDODState(DODState* state);
