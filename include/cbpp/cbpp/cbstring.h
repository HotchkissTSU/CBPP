#ifndef CBPP_STRING_H
#define CBPP_STRING_H

/*
    CBPP uses UTF32 for an internal strings storage and UTF8 for a file I/O.
    Forget about locales. They does not exist. It`s a myth.
*/

#include <cstdint>
#include <cstring>
#include <uchar.h>

#include <typeinfo>

#include "cbpp/cbdef.h"
#include "cbpp/error.h"

namespace cbpp {
    //4-byte character type used by the engine
    typedef char32_t Char;

    //This bro is required by C unicode-converting functions and i have no idea what he does
    extern mbstate_t g_mbstate;

    //This buffer holds the results of String::C8 and String::C32 methods
    extern char* g_string_buffer;
    
    // CBPP character string type. Uses UTF-32 to store the data and can convert text from UTF-8 and back.
    // Use only in situations where multiple languages are required.
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

            //Get string\`s content as a null-terminated, UTF-32 encoded char32_t* array.
            //The return pointer is buffered and you must never free() it by yourself.
            Char* C32() const;

            //Get string\`s content as a null-terminated, UTF-8 encoded char* array.
            //The return pointer is buffered and you must never free() it by yourself.
            char* C8() const;

            //Get length of the null-terminated UTF-32 string
            static size_t str32len(const Char* u32, size_t maxln = -1);

            //Create a copy of the UTF-32 string
            static Char* str32dup(const Char* src);

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

            /*Split a string by the delimeter, not including ones between the quote pairs.
            The resulting pointer is to be cleared with cbpp::Free()*/
            String* SplitEx(size_t& size_ref, Char delim, Char quote = U'"');

            size_t Count(Char to_count) const;

            friend String operator+(const String&, const String&);

            ~String();

        private:
            void Alloc(size_t ln);

            Char* m_array = NULL;
            size_t m_imag_length = 0, m_phys_length = 0;
    };

    String operator+(const String& A, const String& B);

    String operator ""_CB (const char* u8, size_t);
    String operator ""_CB (const Char* u32, size_t);
    
    /*
        Character stream object for the more convenient text parsing
        NOTE: char-templated CharStream is NOT encoded in UTF-8!
    */
    template<typename CharT> class CharStream {
        public:
            CharStream() = default;

            CharStream(const CharT* c_string) {
                Set(c_string);
            }

            //Get the current symbol
            CharT Current() {
                if(!IsValid()) {
                    CbThrowError("Character stream is not allocated");
                }

                return m_stream[m_pointer];
            }

            //Get the symbol with the offset from the current one
            CharT Offset(int64_t ln) {
                if(m_pointer+ln < m_length && m_pointer+ln >= 0) {
                    return m_stream[m_pointer+ln];
                }else{
                    return (CharT)(0);
                }
            }

            //Move the pointer forward
            bool Advance(size_t dist) {
                m_pointer += dist;

                if(m_pointer >= m_length) {
                    m_pointer = m_length-1;
                    return false;
                }

                return true;
            }

            //Set the pointer to the default position
            void Reset() {
                m_pointer = 0;
            }

            bool IsValid() {
                return (m_stream != NULL) && (m_pointer < m_length);
            }

            size_t GetIndex() {
                return m_pointer;
            }

            ~CharStream() {
                free(m_stream);
            }

        private:
            size_t _cb_cs_strlen(const char* ch) {
                return strlen(ch);
            }

            size_t _cb_cs_strlen(const Char* ch) {
                return String::str32len(ch);
            }

            void Set(const CharT* c_str) {
                size_t s_ln = _cb_cs_strlen(c_str);

                CharT* tmp_ptr = (CharT*)realloc(m_stream, s_ln);

                if(tmp_ptr == NULL) {
                    CbThrowErrorf("CharStream reallocation (%lu->%lu) failed", m_length, s_ln);
                }

                m_stream = tmp_ptr;
                m_length = s_ln;

                memcpy(m_stream, c_str, sizeof(CharT)*s_ln);
            }

            CharT* m_stream = NULL;
            size_t m_length = 0;
            size_t m_pointer = 0;
    };

    // A structure for safe c-strings storage in any containers
    class CString {
        public:
            CString();
            CString(const char* sSource);
            CString(const CString& refOther);

            CString& operator=(const CString& refOther);
            bool operator==(const char* sOther) const noexcept;
            bool operator==(const CString& refOther) const noexcept;

            bool operator<(const CString& refOther) const noexcept;

            char& operator[](size_t iIndex) noexcept;
            char operator[](size_t iIndex) const noexcept;

            operator const char * () const noexcept;
            operator char*() noexcept;

            ~CString();

        private:
            char* m_sData = NULL;
    };
}

#endif
