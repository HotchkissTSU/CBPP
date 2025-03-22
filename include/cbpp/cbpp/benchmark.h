#ifndef CBPP_BENCHMARK_H
#define CBPP_BENCHMARK_H

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#include "cbpp/error.h"

#ifdef CBPP_DEBUG
    #define CbBenchmark(hStream) cbpp::Profiler __cbprofiler_current(__PRETTY_FUNCTION__, hStream)
#else
    #define CbBenchmark(hStream)
#endif

namespace cbpp {
    class Profiler {
        public:
            Profiler(const char* sFuncName, FILE* hOutputStream) noexcept;
            ~Profiler() noexcept;

        private:
            const char* m_sFuncName;
            FILE* m_hOutput;
            timeval m_iBegin;
    };
}

#endif
