#include "cbpp/cbstring.h"
#include "cbpp/cb_alloc.h"

namespace cbpp {
    Char* String::C32() const {
        Char* out = Allocate<Char>(Length()+1);
        memcpy(out, m_array.CArr(), sizeof(Char)*Length());

        return out;
    }

    char* String::C8() const {
        char* u8 = U32_U8(m_array.CArr());
        return u8;
    }

    void String::Print(FILE* target) const {
        for(size_t i = 0; i < Length(); i++) {
            fprintf(target, "%lc", m_array.ConstAt(i));
        }
    }

    String String::FromU8(const char* u8) {
        Char* u32 = U8_U32(u8);
        String out(u32);
        free(u32);

        return out;
    }

    String operator+(const String& A, const String& B) {
        Char* out_ptr = Allocate<Char>( A.Length() + B.Length() + 1 );

        for(size_t i = 0; i < A.Length(); i++) {
            out_ptr[i] = A.ConstAt(i);
        }

        for(size_t i = 0; i < B.Length(); i++) {
            out_ptr[i + A.Length()] = B.ConstAt(i);
        }

        String out(out_ptr);
        free(out_ptr);

        return out;
    }

    String operator*(const String& A, size_t mul) {
        size_t alen = A.Length();
        size_t flen = alen*mul;

        Char* out_ptr = Allocate<Char>(flen + 1);

        for(size_t i = 0; i < flen; i++) {
            out_ptr[i] = A.ConstAt( i % alen );
        }

        if(mul == 0) {
            free(out_ptr);
            Char buffer[] = {U' ',U'\0'};
            return String(buffer);
        }

        String out(out_ptr);
        free(out_ptr);

        return out;
    }
}

//The spooky section starts here
namespace cbpp {
    mbstate_t g_mbstate = {0};

    size_t String::str32len(const Char* u32, size_t maxln) {
        Char current = 'a';
        size_t ln = 0;
        while( (current = u32[ln] != '\0') ) {
            if(maxln != -1 && ln >= maxln) { break; }
            ln++;
        }
        return ln;
    }

    char* String::U32_U8(const Char* u32) {
        char buffer[MB_CUR_MAX]; //buffer to hold UTF-8 bytes for a single UTF-32 character

        size_t u32_ln = str32len(u32);

        char* out = Allocate<char>(MB_CUR_MAX * u32_ln);

        if(out == NULL) {
            return NULL;
        }

        memset(&g_mbstate, 0, sizeof(mbstate_t));

        char* p = out; //completely stolen from cppreference
        for (size_t n = 0; n < u32_ln; ++n) {
            size_t rc = c32rtomb(p, u32[n], &g_mbstate);
            if(rc == (size_t)-1) {
                PushError(ERROR_TCODE, "Invalid UTF-32 character code");
                return NULL;
            }
            p += rc;
        }
        
        size_t out_sz = p - out;

        //we allocated this buffer with a bit of overhead, so we need to free that leftover memory
        char* tmp = Reallocate<char>(out, -1, out_sz + 1);
        if(tmp == NULL) {
            return out;
        }

        tmp[out_sz] = '\0';
        return tmp;
    }

    Char* String::U8_U32(const char* u8) {
        size_t u8_ln = strlen(u8);
        Char* out = Allocate<Char>(u8_ln);
        if(out == NULL) {
            return NULL;
        }

        Char* p_out = out;
        char *p_in = const_cast<char*>(u8), *end = const_cast<char*>(u8) + u8_ln + 1;
        memset(&g_mbstate, 0, sizeof(mbstate_t));
        size_t rc = 0;

        //terrible! im scared of this
        while( rc = mbrtoc32(p_out, p_in, end - p_in, &g_mbstate) ) {
            if(rc == (size_t)-3) {
                PushError(ERROR_TCODE, "No surrogate pairs in UTF-32");
                return NULL;
            }

            if(rc == (size_t)-2) {
                PushError(ERROR_TCODE, "Truncated UTF-8 input");
                return NULL;
            }

            if(rc == (size_t)-1) {
                PushError(ERROR_TCODE, "Invalid UTF-8 input");
                return NULL;
            }

            p_in += rc;
            ++p_out;
        }

        //we allocated this buffer with a bit of overhead, so we need to free that leftover memory
        size_t out_size = p_out+1 - out;
        Char* tmp = Reallocate<Char>(out, u8_ln, out_size+1);

        if(tmp == NULL) {
            return out;
        }

        tmp[out_size] = '\0';
        return tmp;
    }
}
