#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    double last_time;
    float current_fps;
} DODFPSCounter;

typedef struct {
    size_t memory;
} DODMemoryUsage;

void fpsUpdate(DODFPSCounter* counter);
float fpsGetValue(const DODFPSCounter* counter);

void addAllocation(DODMemoryUsage* mem, size_t size);
void removeAllocation(DODMemoryUsage* mem, size_t size);
float memGetValue(const DODMemoryUsage* mem);
DODFPSCounter* FPSCounterGetInstance(void);
DODMemoryUsage* MemoryUsageGetInstance(void);
void* memalloc(size_t size);
void memfree(void* memory, size_t size);