#include "cbpp/cbstring.h"
#include "cbpp/cb_alloc.h"

namespace cbpp {
    char* g_string_buffer = NULL;

    void String::Alloc(size_t ln) {
        m_imag_length = ln;

        if(m_imag_length > m_phys_length) {
            m_phys_length = m_imag_length + CBPP_STRING_OVERHEAD;
        }

        if(m_imag_length*2 < m_phys_length) {
            m_phys_length = m_imag_length + CBPP_STRING_OVERHEAD;
        }

        Char* tmp_ptr = Realloc<Char>(m_array, m_phys_length*sizeof(Char));
        if(tmp_ptr == NULL) {
            free(m_array);
            CbThrowErrorf("String memory allocation failure (Img:%lu/Phys:%lu)", m_imag_length, m_phys_length);
        }

        m_array = tmp_ptr;
    }

    String& String::operator=(const String& other) {
        m_imag_length = other.Length();
        Alloc(m_imag_length);

        for(size_t i = 0; i < m_imag_length; i++) {
            m_array[i] = other.ConstAt(i);
        }

        return *this;
    }

    String::String(const Char* u32) {
        size_t ln = str32len(u32);
        Alloc(ln);
        memcpy(m_array, u32, sizeof(Char)*ln);
    }

    String::String(const Char* u32, size_t ln) {
        Alloc(ln);
        memcpy(m_array, u32, sizeof(Char)*ln);
    }

    String::String(const String& other) {
        
    }

    String::String(const char* u8) {
        Char* buffer = U8_U32(u8);
        size_t u32_ln = str32len(buffer);
        Alloc(u32_ln);
        memcpy(m_array, buffer, sizeof(Char)*u32_ln);
        free(buffer);
    }

    Char& String::At(size_t index) {
        if(index >= m_phys_length) {
            CbThrowErrorf("Invalid string index %lu of maximum %lu", index, m_phys_length);
        }

        if(index >= m_imag_length) {
            CbThrowWarningf("Index %lu is inside the allocated chunk, but outside of virtual string bounds (%lu)", index, m_imag_length);
        }

        return m_array[index];
    }

    const Char& String::ConstAt(size_t index) const {
        if(index >= m_phys_length) {
            CbThrowErrorf("Invalid string index %lu of maximum %lu", index, m_phys_length);
        }

        if(index >= m_imag_length) {
            CbThrowWarningf("Index %lu is inside the allocated chunk, but outside of virtual string bounds (%lu)", index, m_imag_length);
        }

        return m_array[index];
    }

    Char& String::operator[](size_t index) {
        return At(index);
    }

    const Char& String::operator[](size_t index) const {
        return ConstAt(index);
    }

    void String::PushBack(Char value) {
        m_imag_length++;
        Alloc(m_imag_length);
        m_array[m_imag_length-1] = value;
    }

    void String::Clear() {
        m_imag_length = 0;
        m_phys_length = 0;
        free(m_array);
        m_array = NULL;
    }

    size_t String::Length() const {
        return m_imag_length;
    }

    size_t String::PhysLength() const {
        return m_phys_length;
    }

    void String::Print(FILE* target) const {
        for(size_t i = 0; i < m_imag_length; i++) {
            fprintf(target, "%lc", m_array[i]);
        }
    }

    Char* String::C32() const {
        g_string_buffer = Realloc<char>(g_string_buffer, (m_imag_length+1)*sizeof(Char));
        memset(g_string_buffer, 0, m_imag_length+1);
        memcpy(g_string_buffer, m_array, m_imag_length);

        return (Char*)g_string_buffer;
    }

    char* String::C8() const {
        free(g_string_buffer);
        g_string_buffer = U32_U8(m_array, m_imag_length);

        return g_string_buffer;
    }

    String operator+(const String& A, const String& B) {
        Char* out_ptr = Malloc<Char>( A.Length() + B.Length() + 1 );

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

    String& String::operator+=(Char other) {
        PushBack(other);

        return *this;
    }

    String* String::Split(size_t& size_ref, Char delim) {
        size_ref = 0;
        for(size_t i = 0; i < Length(); i++) {
            if(At(i) == delim) {
                size_ref++;
            }
        }
        size_ref += 1;

        String* out = Malloc<String>(size_ref);
        if(out == NULL) {
            size_ref = 0;
            return NULL;
        }

        String tmp;
        size_t str_counter = 0;
        for(size_t i = 0; i < Length()+1; i++) {
            if(i == Length()) {
                out[str_counter] = tmp;
                break;
            }

            if(At(i) == delim) {
                out[str_counter] = tmp;
                str_counter++;
                tmp.Clear();
            }else{
                tmp += At(i);
            }
        }

        return out;
    }

    String* String::SplitEx(size_t& size_ref, Char delim, Char quote) {
        size_ref = 0;
        bool is_quo = false;
        for(size_t i = 0; i < Length(); i++) {
            if(At(i) == quote) {
                is_quo = !is_quo;
            }else if(!is_quo && At(i) == delim) {
                size_ref++;
            }
        }
        size_ref += 1;

        is_quo = false;

        String* out = Malloc<String>(size_ref);
        if(out == NULL) {
            size_ref = 0;
            return NULL;
        }

        String tmp;
        size_t str_counter = 0;
        for(size_t i = 0; i < Length()+1; i++) {
            if(i == Length()) {
                out[str_counter] = tmp;
                break;
            }

            if(At(i) == quote) {
                is_quo = !is_quo;
            }else if(At(i) == delim && !is_quo) {
                out[str_counter] = tmp;
                str_counter++;
                tmp.Clear();
            }else {
                tmp += At(i);
            }
        }

        return out;
    }

    size_t String::Count(Char to_count) const {
        size_t out = 0;
        for(size_t i = 0; i < Length(); i++) {
            out = out + (size_t)(ConstAt(i) == to_count);
        }

        return out;
    }

    String operator ""_CB (const char* u8, size_t) {
        String out(u8);
        return out;
    }

    String operator ""_CB (const Char* u32, size_t) {
        String out(u32);
        return out;
    }
    
    String::~String() {
        free(m_array);
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

    Char* String::str32dup(const Char* u32) {
        size_t iStrLen = str32len(u32);
        Char* sOut = Malloc<Char>( sizeof(Char)*(iStrLen+1) );
        sOut[iStrLen] = U'\0';
        memcpy(sOut, u32, sizeof(Char)*iStrLen);

        return sOut;
    }

    char* String::U32_U8(const Char* u32) {
        return U32_U8(u32, str32len(u32));
    }

    char* String::U32_U8(const Char* u32, size_t u32_ln) {
        char buffer[MB_CUR_MAX]; //buffer to hold UTF-8 bytes for a single UTF-32 character

        char* out = Malloc<char>(MB_CUR_MAX * u32_ln);

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

        //we have allocated this buffer with a bit of overhead, so need to free that leftover memory
        char* tmp = Realloc<char>(out, out_sz + 1);
        if(tmp == NULL) {
            return out;
        }

        tmp[out_sz] = '\0';
        return tmp;
    }

    Char* String::U8_U32(const char* u8) {
        size_t u8_ln = strlen(u8);
        Char* out = Malloc<Char>(u8_ln*MB_CUR_MAX);
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

        //we have allocated this buffer with a bit of overhead, so need to free that leftover memory
        size_t out_size = p_out+1 - out;
        Char* tmp = Realloc<Char>(out, out_size+1);

        if(tmp == NULL) {
            return out;
        }

        tmp[out_size] = '\0';
        return tmp;
    }

    
    CString::CString() : m_sData(NULL) { }
    CString::CString(const char* sTarget) {
        size_t iLength = strlen(sTarget);
        m_sData = Malloc<char>(iLength+1);
        memcpy(m_sData, sTarget, iLength);

        m_sData[iLength] = '\0';
    }

    CString::operator char * () noexcept { return m_sData; }

    CString::operator const char * () const noexcept {
        return const_cast<const char*>(m_sData);
    }

    CString::CString(const CString& refOther) {
        if(m_sData != NULL) {
            free(m_sData);
        }

        size_t iLength = strlen(refOther.m_sData);
        m_sData = Malloc<char>(iLength+1);
        memcpy(m_sData, refOther.m_sData, iLength);

        m_sData[iLength] = '\0';
    }

    CString& CString::operator=(const CString& refOther) {
        if(m_sData != NULL) {
            free(m_sData);
        }

        m_sData = strdup(refOther.m_sData);
        return *this;
    }

    bool CString::operator==(const char* sOther) const noexcept {
        return strcmp(m_sData, sOther) == 0;
    }

    bool CString::operator==(const CString& refOther) const noexcept {
        return strcmp(m_sData, refOther.m_sData) == 0;
    }

    bool CString::operator<(const CString& refOther) const noexcept {
        return strcmp(m_sData, refOther.m_sData) < 0;
    }

    char& CString::operator[](size_t iIndex) noexcept {
        return m_sData[iIndex];
    }

    char CString::operator[](size_t iIndex) const noexcept {
        return m_sData[iIndex];
    }

    CString::~CString() {
        if(m_sData != NULL) { Free(m_sData); }
    }
}

namespace cbpp {
    ConstString::ConstString(const ConstString& sOther) : m_sData(sOther.m_sData) {}
    ConstString::ConstString(const char* sSource) : m_sData(sSource) {}

    ConstString::operator const char* () const noexcept {
        return m_sData;
    }

    bool ConstString::operator==(const ConstString& sOther) const noexcept {
        return strcmp(m_sData, sOther.m_sData) == 0;
    }

    bool ConstString::operator<(const ConstString& sOther) const noexcept {
        return strcmp(m_sData, sOther.m_sData) < 0;
    }
}
