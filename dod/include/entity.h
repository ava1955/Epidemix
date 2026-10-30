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

    float* obs_pos_x;
    float* obs_pos_y;
    float* obs_size;
    int* obs_totalCollisions;
    bool* obs_infected;
    int obs_count;
    int obs_capacity;
} DODState;

void initDODState(DODState* state, int initialCapacity);
void freeDODState(DODState* state);
