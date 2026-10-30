#include "dod/include/entity.h"
#include "config.h"

void initDODState(DODState* state, int initialCapacity) {
    state->count = 0;
    state->capacity = initialCapacity;
    state->pos_x = (float*)memalloc(initialCapacity * sizeof(float));
    state->pos_y = (float*)memalloc(initialCapacity * sizeof(float));
    state->directionVector = (float*)memalloc(initialCapacity * sizeof(float));
    state->canMove = (bool*)memalloc(initialCapacity * sizeof(bool));
    state->radius = (float*)memalloc(initialCapacity * sizeof(float));
    state->infected = (bool*)memalloc(initialCapacity * sizeof(bool));
    state->infectedTime = (float*)memalloc(initialCapacity * sizeof(float));

    for (int i = 0; i < initialCapacity; i++) {
        state->pos_x[i] = (float)(rand() % config.WINDOW_WIDTH);
        state->pos_y[i] = (float)(rand() % config.WINDOW_HEIGHT);
        state->directionVector[i] = (float)(rand() % 360);
        state->canMove[i] = true;
        state->radius[i] = (float)config.CircleRadius;
        state->infected[i] = false;
        state->infectedTime[i] = 0.0f;
        state->count++;
    }
    if (config.Virus && initialCapacity > 0) {
        state->infected[0] = true;
    }

    state->obs_count = 0;
    state->obs_capacity = config.ObstacleCount;

    state->obs_pos_x = (float*)memalloc(config.ObstacleCount * sizeof(float));
    state->obs_pos_y = (float*)memalloc(config.ObstacleCount * sizeof(float));
    state->obs_size = (float*)memalloc(config.ObstacleCount * sizeof(float));
    state->obs_totalCollisions = (int*)memalloc(config.ObstacleCount * sizeof(int));
    state->obs_infected = (bool*)memalloc(config.ObstacleCount * sizeof(bool));

    for (int i = 0; i < config.ObstacleCount; i++) {
        state->obs_pos_x[i] = (float)config.Obstacles[i][0];
        state->obs_pos_y[i] = (float)config.Obstacles[i][1];
        state->obs_size[i] = (float)config.ObstacleSize;
        state->obs_totalCollisions[i] = 0;
        state->obs_infected[i] = false;
        state->obs_count++;
    }
}

void freeDODState(DODState* state) {
    if (state == NULL) return;
    int cap = state->capacity;
    memfree(state->pos_x, cap * sizeof(float));
    memfree(state->pos_y, cap * sizeof(float));
    memfree(state->directionVector, cap * sizeof(float));
    memfree(state->canMove, cap * sizeof(bool));
    memfree(state->radius, cap * sizeof(float));
    memfree(state->infected, cap * sizeof(bool));
    memfree(state->infectedTime, cap * sizeof(float));
    state->count = 0;
    state->capacity = 0;

    int obsCap = state->obs_capacity;
    if (obsCap > 0) {
        memfree(state->obs_pos_x, obsCap * sizeof(float));
        memfree(state->obs_pos_y, obsCap * sizeof(float));
        memfree(state->obs_size, obsCap * sizeof(float));
        memfree(state->obs_totalCollisions, obsCap * sizeof(int));
        memfree(state->obs_infected, obsCap * sizeof(bool));
    }

    state->count = 0;
    state->capacity = 0;
    state->obs_count = 0;
    state->obs_capacity = 0;
}
