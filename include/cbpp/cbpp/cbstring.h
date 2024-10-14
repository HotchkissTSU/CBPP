#ifndef CBPP_STRING_H
#define CBPP_STRING_H

#include <cstdint>
#include <cstring>

#include <uchar.h>

#include "cbpp/ttype/segarr.h"

namespace cbpp {
    typedef char32_t Char;

    uint64_t utf8_to_utf32(const char* utf8, Char** utf32);

    uint64_t u32_strlen(const Char* str);

    Char* u32_strcpy(Char* dest, const Char* src);
    Char* u32_strdup(const Char* src);

    class String {
        public:
            String() = default;
            String(const char* str);
            String(const Char* str);
            String(String& other);

            void FromUTF8(const char* utf8);
            void FromUTF32(const Char* utf32);

            Char& operator[]( uint64_t index );
            Char& At( uint64_t index );

            char* AsUTF8();
            const Char* AsUTF32();

            ~String();

        private:
            Char* buffer = NULL;
            uint64_t ln = 0;         //virtual length, always less then ln_actual
            uint64_t ln_actual = 0;  //actual allocated buffer size

            void Allocate(uint64_t ln);
    };
}

#endif
