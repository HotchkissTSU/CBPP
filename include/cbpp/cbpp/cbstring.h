#ifndef CBPP_STRING_H
#define CBPP_STRING_H

/*
    CBPP uses UTF32 for an internal strings storage and UTF8 for a file I/O.
    Forget about locales. They does not exist. It`s a myth.
*/

#include <cstdint>
#include <cstring>
#include <uchar.h>

#include "cbpp/ttype/list.h"

namespace cbpp {
    //4-byte character type used by the engine
    typedef char32_t Char;

    //this bro is required by the C unicode-converting functions and i have no idea what it does
    extern mbstate_t g_mbstate;
    
    /*
        CBPP characted string type. Uses UTF-32 to store the data and can convert text from UTF-8 and back
    */
    class String : public List<Char> {
        public:
            String() = default;
            String(const Char* u32) : List<Char>(u32, str32len(u32)) {};
            String(const Char* u32, size_t ln) : List<Char>(u32,ln) {};
            String(const List<Char>& list) : List<Char>(list) {};
            String(size_t ln) : List<Char>(ln) {};

            static String FromU8(const char* u8);

            void Print(FILE* target = stdout) const;

            bool operator== (const String& other);
            String& operator+= (Char other);

            /*Get string\`s content as a C char32_t* array
            This pointer is malloc`d and you need to free it by yourself*/
            Char* C32() const;

            /*Get string\`s content as a C, UTF-8 encoded char* array
            This pointer is malloc`d and you need to free it by yourself*/
            char* C8() const;

            //Get length of the null-terminated UTF-32 string
            static size_t str32len(const Char* u32, size_t maxln = -1);

            /*Convert a null-terminated string from UTF32 to UTF8
            The resulting pointer is malloc`d and you need to free it by yourself*/
            static char* U32_U8(const Char* u32);

            /*Convert a null-terminated string from UTF8 to UTF32
            The resulting pointer is malloc`d and you need to free it by yourself*/
            static Char* U8_U32(const char* u8);

            /*Split a string by the delimeter.
            The resulting pointer is to be cleared with cbpp::Free()*/
            static String* Split(size_t& size_ref, const String& str, Char delim);

            /*Split a string by the delimeter, not including ones inside the quote pairs.
            The resulting pointer is to be cleared with cbpp::Free()*/
            static String* SplitEx(size_t& size_ref, const String& str, Char delim, Char quote = U'"');

            friend String operator+(const String&, const String&);
            friend String operator*(const String&, size_t);
    }

    String operator+(const String& A, const String& B);
    String operator*(const String& A, size_t mul);
}

#endif