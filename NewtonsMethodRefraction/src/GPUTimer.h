#pragma once
#include "external/glad.h"

// The GPU and CPU are not synchronized
// To time GPU operations, we must use special timers
class GPUTimer
{
public:
    // Ideally, these timers should only be initialized once when the application is launched
    GPUTimer();
    ~GPUTimer();

    // The Begin() operation is fast
    void Begin();
    // The End() operation forces the CPU and GPU to synchronize and so is very slow
    // Returns the number of nanoseconds that the GPU operations have taken since Begin() was called
    GLint64 End();
private:
    GLuint64 startTime, endTime;
    unsigned int queryID[2];
};

// Non-blocking variant. Begin() reads the oldest pending result if available (without
// stalling), then issues a new timestamp query. End() closes the current measurement.
// GetMs() returns an EMA-smoothed millisecond value lagging by up to NUM_SLOTS frames.
class AsyncGPUTimer
{
public:
    AsyncGPUTimer();
    ~AsyncGPUTimer();
    void Begin();
    void End();
    double GetMs() const { return m_ms; }
private:
    static const int NUM_SLOTS = 3;
    unsigned int m_queryIds[NUM_SLOTS][2];
    bool m_pending[NUM_SLOTS];
    int m_writeIdx;
    double m_ms;
};
