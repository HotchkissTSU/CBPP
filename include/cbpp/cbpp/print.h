#ifndef CBPP_PRINTF_H
#define CBPP_PRINTF_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "cbpp/cbdef.h"

#define CBPP_TYPEIO_UNDEFINED "Type unsupported"

namespace cbpp {
    class BasePrintable {
        public:
            virtual void Print(FILE* hStream = stdout) = 0;
            virtual void SPrint(char* sTarget, size_t iMaxWrite) = 0;

            virtual const char* ToString() = 0;
            virtual bool FromString(const char* sSource) = 0;
    };

    template <typename T> void Print(const T& refValue, FILE* hTarget = stdout) {
        if(std::is_base_of<BasePrintable, T>::value) {
            refValue.Print(hTarget);
        }else{
            fprintf(hTarget, CBPP_TYPEIO_UNDEFINED);
        }
    }

    template <typename T> void SPrint(const T& refValue, char* sTarget, size_t iMaxWrite) {
        if(std::is_base_of<BasePrintable, T>::value) {
            refValue.SPrint(sTarget, iMaxWrite);
        }else{
            snprintf(sTarget, iMaxWrite, CBPP_TYPEIO_UNDEFINED);
        }
    }

    /*
        As i can not inherit a base type from the trait class listed above, i have to do this 
        purgatory to implement the Print for them.
        If you want Print to support other primitive types, you should add them to these functions by yourself.
    */

    template<> void Print<float >(const float& refValue, FILE* hTarget = stdout);
    template<> void Print<double>(const double& refValue, FILE* hTarget = stdout);

    template<> void Print<int8_t >(const int8_t& refValue, FILE* hTarget = stdout);
    template<> void Print<int16_t>(const int16_t& refValue, FILE* hTarget = stdout);
    template<> void Print<int32_t>(const int32_t& refValue, FILE* hTarget = stdout);
    template<> void Print<int64_t>(const int64_t& refValue, FILE* hTarget = stdout);

    template<> void Print<uint8_t >(const uint8_t& refValue, FILE* hTarget = stdout);
    template<> void Print<uint16_t>(const uint16_t& refValue, FILE* hTarget = stdout);
    template<> void Print<uint32_t>(const uint32_t& refValue, FILE* hTarget = stdout);
    template<> void Print<uint64_t>(const uint64_t& refValue, FILE* hTarget = stdout);

    template<> void SPrint<float >(const float& refValue, char* sTarget, size_t iMaxWrite);
    template<> void SPrint<double>(const double& refValue, char* sTarget, size_t iMaxWrite);

    template<> void SPrint<int8_t >(const int8_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> void SPrint<int16_t>(const int16_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> void SPrint<int32_t>(const int32_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> void SPrint<int64_t>(const int64_t& refValue, char* sTarget, size_t iMaxWrite);

    template<> void SPrint<uint8_t >(const uint8_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> void SPrint<uint16_t>(const uint16_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> void SPrint<uint32_t>(const uint32_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> void SPrint<uint64_t>(const uint64_t& refValue, char* sTarget, size_t iMaxWrite);
}

#endif
