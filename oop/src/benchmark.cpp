#include "oop/include/benchmark.h"


void* operator new(std::size_t size) {
    MemoryUsage::getInstance().addAllocation(size);
    return std::malloc(size);
}

void* operator new[](std::size_t size) {
    MemoryUsage::getInstance().addAllocation(size);
    return std::malloc(size);
}

void operator delete(void* memory) noexcept {
    std::free(memory);
}

void operator delete(void* memory, std::size_t size) noexcept {
    MemoryUsage::getInstance().removeAllocation(size);
    std::free(memory);
}

void operator delete[](void* memory, std::size_t size) noexcept {
    MemoryUsage::getInstance().removeAllocation(size);
    std::free(memory);
}


FPSCounter::FPSCounter() : last_time(std::chrono::steady_clock::now()), current_fps(0.0f) {}

void FPSCounter::update() {
    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> dt = current_time - last_time;

    if (dt.count() > 0.0f) current_fps = 1.0f / dt.count();
    else current_fps = 0.0f;
    last_time = current_time;
}

float FPSCounter::getValue() {
    return current_fps;
}

std::size_t MemoryUsage::memory = 0;

MemoryUsage& MemoryUsage::getInstance() {
    static MemoryUsage instance;
    return instance;
}

void MemoryUsage::addAllocation(std::size_t size) {
    memory += size;
}

void MemoryUsage::removeAllocation(std::size_t size) {
    memory -= size;
}

float MemoryUsage::getValue() {
    return static_cast<float>(memory) / (1024.0f * 1024.0f);
}
