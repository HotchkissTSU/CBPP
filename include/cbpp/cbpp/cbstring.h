#ifndef CBPP_STRING_H
#define CBPP_STRING_H

#include <cstdint>
#include <cstring>
#include <uchar.h>

#include "cbpp/ttype/list.h"

namespace cbpp {
    typedef char32_t Char;
    extern mbstate_t m_mbstate;

    class String {
        public:
            String() = default;
            String(const char* u8);
            String(const Char* u32);

            Char& At(size_t index);
            Char& operator[](size_t index);

            const Char* C32();
            const char* C8();

            //Get length of the null-terminated UTF-32 string
            static size_t str32len(const Char* u32, size_t maxln = -1);

            static const char* U32_U8(const Char* u32);
            static const Char* U8_U32(const char* u8);

        private:
            List<Char> m_array;
    };
}

#endif
