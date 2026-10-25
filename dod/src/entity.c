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
}
