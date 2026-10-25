#include "dod/include/main_dod.h"
#include "dod/include/benchmark.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    #define _POSIX_C_SOURCE 199309L
    #include <time.h>
#endif

static double getLocalTime(void) {
#if defined(_WIN32) || defined(_WIN64)
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (float)counter.QuadPart / (float)freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (float)ts.tv_sec + (float)ts.tv_nsec / 1000000000.0;
#endif
}

static float deleteTimer = 0.0f;
static bool fpsInitialized = false;

void initDOD(DODState* state) {
    if (state == NULL) return;
    initDODState(state, 100000);
    deleteTimer = 0.0f;
    fpsInitialized = false;
}

void dod(DODState* state, float dt) {
    if (state == NULL || state->capacity == 0) return;
    if (!fpsInitialized) {
        DODFPSCounter* fpsInst = FPSCounterGetInstance();
        if (fpsInst) {
            fpsInst->last_time = getLocalTime();
        }
        fpsInitialized = true;
    }

    deleteTimer += dt;
    if (deleteTimer >= 2.0f) {
        int toRemove = 100;
        if (state->count < toRemove) {
            toRemove = state->count;
        }
        state->count -= toRemove;
        deleteTimer = 0.0f;
    }

    float speed = 100.0f;
    float w = (float)GetScreenWidth();
    float h = (float)GetScreenHeight();
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
        DrawCircle((int)state->pos_x[i], (int)state->pos_y[i], state->radius[i], RED);
    }
}
