#include "GPUTimer.h"

GPUTimer::GPUTimer()
{
    glGenQueries(2, queryID);
}

GPUTimer::~GPUTimer()
{
    glDeleteQueries(2, queryID);
}

void GPUTimer::Begin()
{
    glQueryCounter(queryID[0], GL_TIMESTAMP);
}

GLint64 GPUTimer::End()
{
    glQueryCounter(queryID[1], GL_TIMESTAMP);
    GLint stopTimerAvailable = 0;

    // This stalls the CPU until the GPU is done
    while (!stopTimerAvailable)
    {
        glGetQueryObjectiv(queryID[1], GL_QUERY_RESULT_AVAILABLE, &stopTimerAvailable);
    }

    glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
    glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &endTime);

    return endTime - startTime;
}

AsyncGPUTimer::AsyncGPUTimer() : m_writeIdx(0), m_ms(0.0)
{
    for (int i = 0; i < NUM_SLOTS; ++i) {
        glGenQueries(2, m_queryIds[i]);
        m_pending[i] = false;
    }
}

AsyncGPUTimer::~AsyncGPUTimer()
{
    for (int i = 0; i < NUM_SLOTS; ++i) {
        glDeleteQueries(2, m_queryIds[i]);
    }
}

void AsyncGPUTimer::Begin()
{
    // Read oldest pending result non-blockingly before overwriting the slot
    if (m_pending[m_writeIdx]) {
        GLint available = 0;
        glGetQueryObjectiv(m_queryIds[m_writeIdx][1], GL_QUERY_RESULT_AVAILABLE, &available);
        if (available) {
            GLuint64 startNs = 0, endNs = 0;
            glGetQueryObjectui64v(m_queryIds[m_writeIdx][0], GL_QUERY_RESULT, &startNs);
            glGetQueryObjectui64v(m_queryIds[m_writeIdx][1], GL_QUERY_RESULT, &endNs);
            double ms = (double)(endNs - startNs) / 1e6;
            m_ms = (m_ms == 0.0) ? ms : (0.9 * m_ms + 0.1 * ms);
            m_pending[m_writeIdx] = false;
        }
    }
    glQueryCounter(m_queryIds[m_writeIdx][0], GL_TIMESTAMP);
}

void AsyncGPUTimer::End()
{
    glQueryCounter(m_queryIds[m_writeIdx][1], GL_TIMESTAMP);
    m_pending[m_writeIdx] = true;
    m_writeIdx = (m_writeIdx + 1) % NUM_SLOTS;
}
