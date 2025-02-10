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
            virtual void Print(FILE* hStream = stdout) const = 0;
            virtual size_t SPrint(char* sTarget, size_t iMaxWrite) const = 0;

        protected:
            virtual ~BasePrintable() = default;
    };

    template <typename T> void Print(const T& refValue, FILE* hTarget = stdout) {
        if(std::is_base_of<BasePrintable, T>::value) {
            refValue.Print(hTarget);
        }else{
            fprintf(hTarget, CBPP_TYPEIO_UNDEFINED);
        }
    }

    template <typename T> size_t SPrint(const T& refValue, char* sTarget, size_t iMaxWrite) {
        if(std::is_base_of<BasePrintable, T>::value) {
            return refValue.SPrint(sTarget, iMaxWrite);
        }else{
            return snprintf(sTarget, iMaxWrite, CBPP_TYPEIO_UNDEFINED);
        }
    }

    /*
        As i can not inherit a base type from the trait class listed above, i have to do this 
        purgatory to implement the Print for him.
        If you want Print to support other primitive types, you should add them to these functions by yourself.
    */

    template<> void Print<float >(const float& refValue, FILE* hTarget);
    template<> void Print<double>(const double& refValue, FILE* hTarget);

    template<> void Print<int8_t >(const int8_t& refValue, FILE* hTarget);
    template<> void Print<int16_t>(const int16_t& refValue, FILE* hTarget);
    template<> void Print<int32_t>(const int32_t& refValue, FILE* hTarget);
    template<> void Print<int64_t>(const int64_t& refValue, FILE* hTarget);

    template<> void Print<uint8_t >(const uint8_t& refValue, FILE* hTarget);
    template<> void Print<uint16_t>(const uint16_t& refValue, FILE* hTarget);
    template<> void Print<uint32_t>(const uint32_t& refValue, FILE* hTarget);
    template<> void Print<uint64_t>(const uint64_t& refValue, FILE* hTarget);

    template<> size_t SPrint<float >(const float& refValue, char* sTarget, size_t iMaxWrite);
    template<> size_t SPrint<double>(const double& refValue, char* sTarget, size_t iMaxWrite);

    template<> size_t SPrint<int8_t >(const int8_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> size_t SPrint<int16_t>(const int16_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> size_t SPrint<int32_t>(const int32_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> size_t SPrint<int64_t>(const int64_t& refValue, char* sTarget, size_t iMaxWrite);

    template<> size_t SPrint<uint8_t >(const uint8_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> size_t SPrint<uint16_t>(const uint16_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> size_t SPrint<uint32_t>(const uint32_t& refValue, char* sTarget, size_t iMaxWrite);
    template<> size_t SPrint<uint64_t>(const uint64_t& refValue, char* sTarget, size_t iMaxWrite);
}

#endif
