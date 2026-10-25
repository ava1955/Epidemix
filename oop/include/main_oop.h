#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <raylib.h>
#include "oop/include/entity.h"
#include "oop/include/benchmark.h"

struct OOPState {
    std::vector<std::unique_ptr<Entity>> entities;
    float deleteTimer = 0.0f;
    benchmark* fps = &OOPFPSCounter::getInstance();
    benchmark* memory = &OOPMemoryUsage::getInstance();
    bool isInitialized = false;
};

void oop(OOPState& state, float dt);
