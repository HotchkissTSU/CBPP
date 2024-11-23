#include "cbpp/locales.h"

#include "cbpp/fileio.h"
#include "cbpp/cb_alloc.h"

namespace cbpp {
    Locale::Locale(const char* fname) {
        Load(fname);
    }

    bool Locale::Load(const char* fname) {
        char fn_buffer[512];
        snprintf(fn_buffer, 512, "assets/locale/%s.txt", fname);

        File inp(fn_buffer, "rb");
        char* buffer = Allocate<char>(inp.Length()+1);
        if(buffer == NULL) { m_io_result = false; return m_io_result; }

        inp.Read(buffer, inp.Length(), 1);
        inp.Close();

        String file_text(buffer);
        free(buffer);

        ParseSource(file_text);

        m_io_result = true;
        return m_io_result;
    }

    bool Locale::ParseSource(const String& src) {
        return false;
    }

    const String& Locale::GetString(const char* key) {
        hash_t khash = Hash(key);

        if(m_data.count(khash) == 0) {
            return m_default_string;
        }

        return m_data[khash];
    }
}