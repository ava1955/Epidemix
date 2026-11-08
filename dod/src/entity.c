#include "dod/include/entity.h"
#include "config.h"

void initDODState(DODState* state, int initialCapacity) {
    float initRadius = (float)config.CircleRadius;
    float padding = 2.0f;
    float cellSize = (initRadius * 2.0f) + padding;

    int cols = (int)(config.WINDOW_WIDTH / cellSize);
    int rows = (int)(config.WINDOW_HEIGHT / cellSize);

    if (cols <= 0) cols = 1;
    if (rows <= 0) rows = 1;

    int maxCapacity = cols * rows;

    if (initialCapacity > maxCapacity) initialCapacity = maxCapacity;

    state->count = 0;
    state->capacity = initialCapacity;
    state->pos_x = (float*)memalloc(initialCapacity * sizeof(float));
    state->pos_y = (float*)memalloc(initialCapacity * sizeof(float));
    state->directionVector = (float*)memalloc(initialCapacity * sizeof(float));
    state->canMove = (bool*)memalloc(initialCapacity * sizeof(bool));
    state->radius = (float*)memalloc(initialCapacity * sizeof(float));
    state->infected = (bool*)memalloc(initialCapacity * sizeof(bool));
    state->infectedTime = (float*)memalloc(initialCapacity * sizeof(float));

    // for (int i = 0; i < initialCapacity; i++) {
    //     state->pos_x[i] = (float)(rand() % config.WINDOW_WIDTH);
    //     state->pos_y[i] = (float)(rand() % config.WINDOW_HEIGHT);
    //     state->directionVector[i] = (float)(rand() % 360);
    //     state->canMove[i] = true;
    //     state->radius[i] = (float)config.CircleRadius;
    //     state->infected[i] = false;
    //     state->infectedTime[i] = 0.0f;
    //     state->count++;
    // }

    int* cellIndices = (int*)memalloc(maxCapacity * sizeof(int));
    for (int i = 0; i < maxCapacity; i++) {
        cellIndices[i] = i;
    }

    for (int i = maxCapacity - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = cellIndices[i];
        cellIndices[i] = cellIndices[j];
        cellIndices[j] = temp;
    }

    for (int i = 0; i < initialCapacity; i++) {
        int cellIndex = cellIndices[i];

        int col = cellIndex % cols;
        int row = cellIndex / cols;

        float baseX = (col * cellSize) + (cellSize / 2.0f);
        float baseY = (row * cellSize) + (cellSize / 2.0f);

        state->pos_x[i] = baseX;
        state->pos_y[i] = baseY;

        state->directionVector[i] = (float)(rand() % 360);
        state->canMove[i] = true;
        state->radius[i] = initRadius;
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

    memfree(cellIndices, maxCapacity * sizeof(int));
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
