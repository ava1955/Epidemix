#include "dod/include/entity.h"

void initDODState(DODState* state, int initialCapacity) {
    state->count = 0;
    state->capacity = initialCapacity;
    state->pos_x = (float*)memalloc(initialCapacity * sizeof(float));
    state->pos_y = (float*)memalloc(initialCapacity * sizeof(float));
    state->directionVector = (float*)memalloc(initialCapacity * sizeof(float));
    state->canMove = (bool*)memalloc(initialCapacity * sizeof(bool));
    state->radius = (float*)memalloc(initialCapacity * sizeof(float));
    state->infected = (bool*)memalloc(initialCapacity * sizeof(bool));

    for (int i = 0; i < initialCapacity; i++) {
        state->pos_x[i] = (float)(rand() % 1800);
        state->pos_y[i] = (float)(rand() % 1200);
        state->directionVector[i] = (float)(rand() % 360);
        state->canMove[i] = true;
        state->radius[i] = 3.0f;
        state->infected[i] = false;
        state->count++;
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
    state->count = 0;
    state->capacity = 0;
}
