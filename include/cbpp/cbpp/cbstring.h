#ifndef CBPP_STRING_H
#define CBPP_STRING_H

/*
    CBPP uses UTF32 for an internal strings storage and UTF8 for a file I/O
*/

#include <cstdint>
#include <cstring>
#include <uchar.h>

#include "cbpp/ttype/list.h"

namespace cbpp {
    //4-byte character type used by the engine
    typedef char32_t Char;

    //this bro is required by the C unicode converting functions and i have no idea what it does
    extern mbstate_t m_mbstate;

    class String : public List<Char> {
        public:
            String() = default;
            String(const char* u8);
            String(const Char* u32) : List<Char>(u32, str32len(u32)) {};

            /*Get string\`s content as a C char32_t* array
            This pointer is malloc`d and you need to free it by yourself*/
            Char* C32();

            /*Get string\`s content as a C char* array
            This pointer is malloc`d and you need to free it by yourself*/
            char* C8();

            //Get length of the null-terminated UTF-32 string
            static size_t str32len(const Char* u32, size_t maxln = -1);

            /*Convert a null-terminated string from UTF32 to UTF8
            The resulting pointer is malloc`d and you need to free it by yourself*/
            static char* U32_U8(const Char* u32);

            /*Convert a null-terminated string from UTF8 to UTF32
            The resulting pointer is malloc`d and you need to free it by yourself*/
            static Char* U8_U32(const char* u8);
    };
}

#endif