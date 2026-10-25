#pragma once
#include <chrono>
#include <cstddef>

class benchmark {
public:
    virtual ~benchmark() = default;
    virtual float getValue() = 0;
    virtual void update() = 0;
};

class OOPFPSCounter : public benchmark {
    static std::chrono::steady_clock::time_point last_time;
    static float current_fps;
    OOPFPSCounter() = default;

public:
    static OOPFPSCounter& getInstance();
    OOPFPSCounter(const OOPFPSCounter&) = delete;
    OOPFPSCounter& operator=(const OOPFPSCounter&) = delete;
    OOPFPSCounter(OOPFPSCounter&&) = delete;
    OOPFPSCounter& operator=(OOPFPSCounter&&) = delete;
    void update() override;
    float getValue() override;
};
class OOPMemoryUsage : public benchmark {
    static std::size_t memory;
    OOPMemoryUsage() = default;
public:
    static OOPMemoryUsage& getInstance();
    OOPMemoryUsage(const OOPMemoryUsage&) = delete;
    OOPMemoryUsage& operator=(const OOPMemoryUsage&) = delete;
    OOPMemoryUsage(OOPMemoryUsage&&) = delete;
    OOPMemoryUsage& operator=(OOPMemoryUsage&&) = delete;
    void addAllocation(std::size_t size);
    void removeAllocation(std::size_t size);
    void update() override;
    float getValue() override;
};
