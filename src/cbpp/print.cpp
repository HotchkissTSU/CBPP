#include "cbpp/print.h"

namespace cbpp {
    template<> void Print<float >(const float& refValue, FILE* hTarget) { fprintf(hTarget, "%f", refValue); }
    template<> void Print<double>(const double& refValue, FILE* hTarget) { fprintf(hTarget, "%lf", refValue); }

    template<> void Print<int8_t >(const int8_t& refValue, FILE* hTarget) { fprintf(hTarget, "%i", refValue); }
    template<> void Print<int16_t>(const int16_t& refValue, FILE* hTarget) { fprintf(hTarget, "%i", refValue); }
    template<> void Print<int32_t>(const int32_t& refValue, FILE* hTarget) { fprintf(hTarget, "%i", refValue); }
    template<> void Print<int64_t>(const int64_t& refValue, FILE* hTarget) { fprintf(hTarget, "%li", refValue); }

    template<> void Print<uint8_t >(const uint8_t& refValue, FILE* hTarget) { fprintf(hTarget, "%u", refValue); }
    template<> void Print<uint16_t>(const uint16_t& refValue, FILE* hTarget) { fprintf(hTarget, "%u", refValue); }
    template<> void Print<uint32_t>(const uint32_t& refValue, FILE* hTarget) { fprintf(hTarget, "%u", refValue); }
    template<> void Print<uint64_t>(const uint64_t& refValue, FILE* hTarget) { fprintf(hTarget, "%lu", refValue); }

    template<> void SPrint<float >(const float& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%f", refValue); }
    template<> void SPrint<double>(const double& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%lf", refValue); }

    template<> void SPrint<int8_t >(const int8_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%d", refValue); }
    template<> void SPrint<int16_t>(const int16_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%d", refValue); }
    template<> void SPrint<int32_t>(const int32_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%d", refValue); }
    template<> void SPrint<int64_t>(const int64_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%ld", refValue); }

    template<> void SPrint<uint8_t >(const uint8_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%u", refValue); }
    template<> void SPrint<uint16_t>(const uint16_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%u", refValue); }
    template<> void SPrint<uint32_t>(const uint32_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%u", refValue); }
    template<> void SPrint<uint64_t>(const uint64_t& refValue, char* sTarget, size_t iMaxWrite) { snprintf(sTarget, iMaxWrite, "%lu", refValue); }
}
