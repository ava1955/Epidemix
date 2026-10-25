#include "dod/include/benchmark.h"
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    static LARGE_INTEGER frequency;
static int is_frequency_initialized = 0;
#else
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#endif

static double getTime(void) {
#if defined(_WIN32) || defined(_WIN64)
    if (!is_frequency_initialized) {
        QueryPerformanceFrequency(&frequency);
        is_frequency_initialized = 1;
    }
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#endif
}

static DODMemoryUsage MemoryUsageInstance = { .memory = 0 };
static DODFPSCounter FPSCounterInstance = { .current_fps = 0.0f };

void FPSInit(void) {
    FPSCounterInstance.last_time = getTime();
}

void* memalloc(size_t size) {
    addAllocation(&MemoryUsageInstance, size);
    return malloc(size);
}

void memfree(void* memory, size_t size) {
    if (memory != NULL) {
        removeAllocation(&MemoryUsageInstance, size);
        free(memory);
    }
}

void fpsUpdate(DODFPSCounter* counter) {
    if (counter == NULL) return;
    double current_time = getTime();
    double dt = current_time - counter->last_time;
    if (dt > 0.0f) counter->current_fps = 1.0f / dt;
    else counter->current_fps = 0.0f;
    counter->last_time = current_time;
}

float fpsGetValue(const DODFPSCounter* counter) {
    return counter ? counter->current_fps : 0.0f;
}

DODFPSCounter* FPSCounterGetInstance(void) {
    return &FPSCounterInstance;
}

DODMemoryUsage* MemoryUsageGetInstance(void) {
    return &MemoryUsageInstance;
}

void addAllocation(DODMemoryUsage* mem, size_t size) {
    if (mem != NULL) {
        mem->memory += size;
    }
}

void removeAllocation(DODMemoryUsage* mem, size_t size) {
    if (mem != NULL) {
        mem->memory -= size;
    }
}

float memGetValue(const DODMemoryUsage* mem) {
    if (mem == NULL) return 0.0f;
    return (float)mem->memory / (1024.0f * 1024.0f);
}
