#include "cbpp/cbstring.h"
#include "cbpp/error.h"

#include <stdlib.h>

namespace cbpp {
    uint64_t utf8_to_utf32(const char* utf8, Char** utf32) {
        uint64_t len = strlen(utf8);
        uint64_t utf32_len = 0;
        
        // Предполагаем, что результат будет достаточно большим (по крайней мере, равным длине входной строки)
        *utf32 = (Char*)realloc(utf32, (len + 1) * sizeof(Char));
        if (*utf32 == NULL) {
            return 0;  // ошибка выделения памяти
        }

        Char* ptr = *utf32;
        const char* end = utf8 + len;

        while (utf8 < end) {
            uint32_t codepoint = 0;
            unsigned char byte = *utf8;

            if (byte < 0x80) {
                // 1 байт (ASCII)
                codepoint = byte;
                utf8++;
            } else if ((byte & 0xE0) == 0xC0) {
                // 2 байта
                codepoint = (byte & 0x1F) << 6;
                utf8++;
                codepoint |= (*utf8 & 0x3F);
                utf8++;
            } else if ((byte & 0xF0) == 0xE0) {
                // 3 байта
                codepoint = (byte & 0x0F) << 12;
                utf8++;
                codepoint |= (*utf8 & 0x3F) << 6;
                utf8++;
                codepoint |= (*utf8 & 0x3F);
                utf8++;
            } else if ((byte & 0xF8) == 0xF0) {
                // 4 байта
                codepoint = (byte & 0x07) << 18;
                utf8++;
                codepoint |= (*utf8 & 0x3F) << 12;
                utf8++;
                codepoint |= (*utf8 & 0x3F) << 6;
                utf8++;
                codepoint |= (*utf8 & 0x3F);
                utf8++;
            } else {
                // Неверный байт, пропустим
                utf8++;
                continue;
            }

            *ptr++ = (Char)codepoint;
            utf32_len++;
        }
        
        *ptr = 0;  // Нуль-терминатор для UTF-32 строки
        return utf32_len;
    }

    char* utf32_to_utf8(const Char* str32) {
        char* str8 = NULL;
        
        // Determine the length of the UTF-8 string
        uint64_t len = 0;
        const char32_t* ptr = str32;
        while (*ptr != U'\0') {
            if (*ptr < 0x80) {
                len += 1;
            } else if (*ptr < 0x800) {
                len += 2;
            } else if (*ptr < 0x10000) {
                len += 3;
            } else {
                len += 4;
            }
            ptr++;
        }
        
        // Allocate memory for the UTF-8 string
        str8 = (char*)malloc(len + 1);
        
        // Convert the UTF-32 string to UTF-8
        char* ptr8 = str8;
        ptr = str32;
        while (*ptr != U'\0') {
            if (*ptr < 0x80) {
                *ptr8++ = (char)(*ptr & 0x7F);
            } else if (*ptr < 0x800) {
                *ptr8++ = (char)(((*ptr >> 6) & 0x1F) | 0xC0);
                *ptr8++ = (char)((*ptr & 0x3F) | 0x80);
            } else if (*ptr < 0x10000) {
                *ptr8++ = (char)(((*ptr >> 12) & 0x0F) | 0xE0);
                *ptr8++ = (char)(((*ptr >> 6) & 0x3F) | 0x80);
                *ptr8++ = (char)((*ptr & 0x3F) | 0x80);
            } else {
                *ptr8++ = (char)(((*ptr >> 18) & 0x07) | 0xF0);
                *ptr8++ = (char)(((*ptr >> 12) & 0x3F) | 0x80);
                *ptr8++ = (char)(((*ptr >> 6) & 0x3F) | 0x80);
                *ptr8++ = (char)((*ptr & 0x3F) | 0x80);
            }
            ptr++;
        }
        
        // Null-terminate the UTF-8 string
        *ptr8 = '\0';
        
        return str8;
    }

    uint64_t u32_strlen(const Char* str) {
        Char c = U'a';
        uint64_t l = 0;
        while(c != U'\0'){
            c = str[l];
            l++;
        }

        return l;
    }

    Char* u32_strcpy(Char* dest, const Char* src) {
        uint64_t src_ln = u32_strlen(src);
        memcpy( dest, src, (src_ln + 1)*sizeof(Char) );
        return dest;
    }

    Char* u32_strdup(const Char* src) {
        if(src == NULL) { return NULL; }

        uint64_t ln = u32_strlen(src);
        Char* ns = new Char[ln+1];
        memset(ns, 0, ln+1);

        u32_strcpy(ns, src);

        return ns;
    }

    String::String(const char* utf8) {
        this->FromUTF8(utf8);
    }

    String::String(const Char* utf32) {
        this->FromUTF32(utf32);
    }

    String::String(String& other) {
        this->FromUTF32( other.AsUTF32() );
    }

    void String::FromUTF8(const char* utf8) {
        uint64_t u32_ln = utf8_to_utf32(utf8, &buffer);
        ln = u32_ln;
        ln_actual = u32_ln;
    }

    void String::FromUTF32(const Char* utf32) {
        ln = u32_strlen( utf32 );
        Allocate(ln);
        memcpy(buffer, utf32, ln);
    }

    const Char* String::AsUTF32() {
        return const_cast<const Char*>(buffer);
    }

    char* String::AsUTF8() {
        return utf32_to_utf8(buffer);
    }

    void String::Allocate(uint64_t len) {
        ln = 0;

        if(len > ln_actual || ln_actual*2 > len) {
            buffer = (Char*)realloc( buffer, sizeof(Char)*len );
            memset(buffer, 0, len*sizeof(Char));
            ln_actual = len;
        }
    }

    String::~String() {
        free( buffer );
    }
}
