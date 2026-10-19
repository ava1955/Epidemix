#pragma once
#include <chrono>
#include <cstddef>

class benchmark {
public:
    virtual ~benchmark() = default;
    virtual float getValue() = 0;
};

class FPSCounter : public benchmark {
    std::chrono::steady_clock::time_point last_time;
    float current_fps;

public:
    FPSCounter();
    void update();
    float getValue() override;
};

class MemoryUsage : public benchmark {
    static std::size_t memory;
    MemoryUsage() = default;

public:
    static MemoryUsage& getInstance();
    MemoryUsage(const MemoryUsage&) = delete;
    MemoryUsage& operator=(const MemoryUsage&) = delete;

    void addAllocation(std::size_t size);
    void removeAllocation(std::size_t size);
    float getValue() override;
};
