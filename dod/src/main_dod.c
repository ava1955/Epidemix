#include "dod/include/main_dod.h"
#include "dod/include/benchmark.h"
#include "config.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

static bool fpsInitialized = false;

void initDOD(DODState* state) {
    if (state == NULL) return;
    initDODState(state, config.EntityCount);
    fpsInitialized = false;
}

void dod(DODState* state, float dt) {
    if (state == NULL || state->capacity == 0) return;
    if (!fpsInitialized) {
        FPSInit();
        fpsInitialized = true;
    }

    for (int i = 0; i < state->count; i++) {
        if (!state->infected[i]) continue;
        state->infectedTime[i] += dt;
        if (state->infectedTime[i] >= config.VirusLifetime) {
            int last = --state->count;
            state->pos_x[i] = state->pos_x[last];
            state->pos_y[i] = state->pos_y[last];
            state->directionVector[i] = state->directionVector[last];
            state->canMove[i] = state->canMove[last];
            state->radius[i] = state->radius[last];
            state->infected[i] = state->infected[last];
            state->infectedTime[i] = state->infectedTime[last];
            i--;
        }
    }

    float collisionDist = 2.0f * config.CircleRadius;
    int cols = (int)(config.WINDOW_WIDTH / collisionDist) + 1;
    int rows = (int)(config.WINDOW_HEIGHT / collisionDist) + 1;
    int* cellHead = (int*)memalloc(cols * rows * sizeof(int));
    int* next = (int*)memalloc(state->count * sizeof(int));
    int* cellOf = (int*)memalloc(state->count * sizeof(int));

    for (int c = 0; c < cols * rows; c++) cellHead[c] = -1;

    for (int i = 0; i < state->count; i++) {
        int cx = (int)(state->pos_x[i] / collisionDist);
        int cy = (int)(state->pos_y[i] / collisionDist);
        if (cx < 0) cx = 0;
        if (cx >= cols) cx = cols - 1;
        if (cy < 0) cy = 0;
        if (cy >= rows) cy = rows - 1;
        cellOf[i] = cy * cols + cx;
        next[i] = cellHead[cellOf[i]];
        cellHead[cellOf[i]] = i;
    }

    for (int j = 0; j < state->count; j++) {
        int cx = cellOf[j] % cols;
        int cy = cellOf[j] / cols;
        for (int ny = cy - 1; ny <= cy + 1; ny++) {
            for (int nx = cx - 1; nx <= cx + 1; nx++) {
                if (nx < 0 || nx >= cols || ny < 0 || ny >= rows) continue;
                for (int k = cellHead[ny * cols + nx]; k != -1; k = next[k]) {
                    if (k <= j) continue;
                    float dx = state->pos_x[j] - state->pos_x[k];
                    float dy = state->pos_y[k] - state->pos_y[j];
                    if (dx * dx + dy * dy < collisionDist * collisionDist) {
                        float tmp = state->directionVector[k];
                        state->directionVector[k] = state->directionVector[j];
                        state->directionVector[j] = tmp;
                        state->infected[j] = state->infected[k] = state->infected[j] || state->infected[k];
                    }
                }
            }
        }
    }

    memfree(cellHead, cols * rows * sizeof(int));
    memfree(next, state->count * sizeof(int));
    memfree(cellOf, state->count * sizeof(int));

    float circleR = (float)config.CircleRadius;
    for (int o = 0; o < state->obs_count; o++) {
        float ox = state->obs_pos_x[o];
        float oy = state->obs_pos_y[o];
        float size = state->obs_size[o];

        #pragma omp parallel for
        for (int i = 0; i < state->count; i++) {
            float dx = state->pos_x[i] - fminf(fmaxf(state->pos_x[i], ox), ox + size);
            float dy = state->pos_y[i] - fminf(fmaxf(state->pos_y[i], oy), oy + size);
            if (dx * dx + dy * dy < circleR * circleR) {
                if (fabsf(dx) > fabsf(dy)) {
                    state->pos_x[i] = dx > 0 ? ox + size + circleR : ox - circleR;
                    state->directionVector[i] = 180.0f - state->directionVector[i];
                }
                else {
                    state->pos_y[i] = dy > 0 ? oy + size + circleR : oy - circleR;
                    state->directionVector[i] = -state->directionVector[i];
                }
                #pragma omp atomic
                state->obs_totalCollisions[o]++;

                if (state->obs_totalCollisions[o] >= config.InfectionThreshold) {
                    state->obs_infected[o] = true;
                }

                if (state->obs_infected[o] && !state->infected[i]) {
                    #pragma omp critical
                    {
                        if (GetRandomValue(1, 100) <= config.InfectionChance) {
                            state->infected[i] = true;
                        }
                    }
                }
            }
        }
    }

    float speed = config.CircleSpeed;
    float w = (float)GetScreenWidth();
    float h = (float)GetScreenHeight();
    #pragma omp parallel for
    for (int i = 0; i < state->count; i++) {
        if (!state->canMove[i]) continue;
        float rad = state->directionVector[i] * DEG2RAD;
        state->pos_x[i] += cosf(rad) * speed * dt;
        state->pos_y[i] += sinf(rad) * speed * dt;
        float r = state->radius[i];
        if (state->pos_x[i] < r) {
            state->pos_x[i] = r;
            state->directionVector[i] = 180.0f - state->directionVector[i];
        }
        else if (state->pos_x[i] > w - r) {
            state->pos_x[i] = w - r;
            state->directionVector[i] = 180.0f - state->directionVector[i];
        }

        if (state->pos_y[i] < r) {
            state->pos_y[i] = r;
            state->directionVector[i] = -state->directionVector[i];
        }
        else if (state->pos_y[i] > h - r) {
            state->pos_y[i] = h - r;
            state->directionVector[i] = -state->directionVector[i];
        }
        if (state->directionVector[i] < 0.0f)   state->directionVector[i] += 360.0f;
        if (state->directionVector[i] >= 360.0f) state->directionVector[i] -= 360.0f;
    }

    for (int i = 0; i < state->count; i++) {
        DrawCircle((int)state->pos_x[i], (int)state->pos_y[i], state->radius[i], state->infected[i] ? GREEN : RED);
    }

    for (int o = 0; o < config.ObstacleCount; o++) {
        Color color = state->obs_infected[o] ? DARKGREEN : DARKGRAY;
        DrawRectangle((int)config.Obstacles[o][0], (int)config.Obstacles[o][1], config.ObstacleSize, config.ObstacleSize, color);
    }
}
