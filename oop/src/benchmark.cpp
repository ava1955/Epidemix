#include "oop/include/benchmark.h"
void* operator new(std::size_t size) {
    std::size_t total_size = size + sizeof(std::size_t);
    void* raw_memory = std::malloc(total_size);
    if (!raw_memory) throw std::bad_alloc();
    *(static_cast<std::size_t*>(raw_memory)) = size;
    OOPMemoryUsage::getInstance().addAllocation(size);
    return static_cast<char*>(raw_memory) + sizeof(std::size_t);
}

void* operator new[](std::size_t size) {
    return ::operator new(size);
}

void operator delete(void* memory) noexcept {
    if (memory == nullptr) return;
    void* raw_memory = static_cast<char*>(memory) - sizeof(std::size_t);
    std::size_t size = *(static_cast<std::size_t*>(raw_memory));
    OOPMemoryUsage::getInstance().removeAllocation(size);
    std::free(raw_memory);
}

void operator delete[](void* memory) noexcept {
    ::operator delete(memory);
}

void operator delete(void* memory, std::size_t size) noexcept {
    (void)size;
    ::operator delete(memory);
}

void operator delete[](void* memory, std::size_t size) noexcept {
    (void)size;
    ::operator delete(memory);
}

float OOPFPSCounter::current_fps = 0.0f;
std::chrono::steady_clock::time_point OOPFPSCounter::last_time = std::chrono::steady_clock::now();

OOPFPSCounter &OOPFPSCounter::getInstance() {
    static OOPFPSCounter instance;
    return instance;
}

void OOPFPSCounter::update() {
    auto current_time = std::chrono::steady_clock::now();
    std::chrono::duration<float> dt = current_time - last_time;
    if (dt.count() > 0.0f) current_fps = 1.0f / dt.count();
    else current_fps = 0.0f;
    last_time = current_time;
}

float OOPFPSCounter::getValue() {
    return current_fps;
}

std::size_t OOPMemoryUsage::memory = 0;

OOPMemoryUsage& OOPMemoryUsage::getInstance() {
    static OOPMemoryUsage instance;
    return instance;
}

void OOPMemoryUsage::addAllocation(std::size_t size) {
    memory += size;
}

void OOPMemoryUsage::removeAllocation(std::size_t size) {
    memory -= size;
}

float OOPMemoryUsage::getValue() {
    return static_cast<float>(memory) / (1024.0f * 1024.0f);
}

void OOPMemoryUsage::update() {}
