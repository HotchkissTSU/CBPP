#include "cbpp/benchmark.h"

namespace cbpp {
    Profiler::Profiler(const char* sFuncName, FILE* hOutputStream) noexcept : m_sFuncName(sFuncName), m_hOutput(hOutputStream) {
        gettimeofday(&m_iBegin, NULL);
    }

    Profiler::~Profiler() noexcept {
        struct timeval iNow;
        gettimeofday(&iNow, NULL);

        double fSecs = (iNow.tv_sec - m_iBegin.tv_sec) + (iNow.tv_usec - m_iBegin.tv_usec) / 1000000.0;

        fprintf(m_hOutput, "Profile for %s: %.3lf ms\n", m_sFuncName, fSecs * 1000.0);
    }
}
