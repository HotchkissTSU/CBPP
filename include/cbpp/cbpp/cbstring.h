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

#define CBPP_STRING_OVERHEAD 32

namespace cbpp {
    //4-byte character type used by the engine
    typedef char32_t Char;

    //this bro is required by the C unicode-converting functions and i have no idea what it does
    extern mbstate_t g_mbstate;

    extern char* g_string_buffer;
    
    // CBPP characted string type. Uses UTF-32 to store the data and can convert text from UTF-8 and back
    class String {
        public:
            String() = default;
            String(const Char* u32);
            String(const Char* u32, size_t ln);
            String(const String& other);
            String(const char* u8);

            const Char& ConstAt(size_t index) const;
            Char& At(size_t index);

            String& operator=(const String& other);

            Char& operator[](size_t index);
            const Char& operator[](size_t index) const;

            void Print(FILE* target = stdout) const;

            bool operator== (const String& other);
            String& operator+= (Char other);

            size_t Length() const;
            size_t PhysLength() const;

            void PushBack(Char value);
            void Clear();

            //Get string\`s content as a null-terminated, UTF-32 encoded char32_t* array
            Char* C32() const;

            //Get string\`s content as a null-terminated, UTF-8 encoded char* array
            char* C8() const;

            //Get length of the null-terminated UTF-32 string
            static size_t str32len(const Char* u32, size_t maxln = -1);

            /*Convert a null-terminated string from UTF32 to UTF8
            The resulting pointer is malloc`d and you need to free it by yourself*/
            static char* U32_U8(const Char* u32);
            static char* U32_U8(const Char* u32, size_t ln);

            /*Convert a null-terminated string from UTF8 to UTF32
            The resulting pointer is malloc`d and you need to free it by yourself*/
            static Char* U8_U32(const char* u8);

            /*Split a string by the delimeter.
            The resulting pointer is to be cleared with cbpp::Free()*/
            String* Split(size_t& size_ref, Char delim);

            /*Split a string by the delimeter, not including ones inside the quote pairs.
            The resulting pointer is to be cleared with cbpp::Free()*/
            String* SplitEx(size_t& size_ref, Char delim, Char quote = U'"');

            size_t Count(Char to_count) const;

            friend String operator+(const String&, const String&);
            friend String operator*(const String&, size_t);

        private:
            void Alloc(size_t ln);

            Char* m_array = NULL;
            size_t m_imag_length = 0, m_phys_length = 0;
    };

    String operator+(const String& A, const String& B);
    String operator*(const String& A, size_t mul);
}

#endif