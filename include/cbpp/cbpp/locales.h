#ifndef CBPP_LOCALES_H
#define CBPP_LOCALES_H

#include "cbpp/cbstring.h"
#include "cbpp/cb_hash.h"

#include <uchar.h>
#include <map>

namespace cbpp {
    class Locale {
        public:
            Locale();
            Locale(const char* fname);

            bool Load(const char* fname);
            bool Save(const char* fname);

            bool HasLabel(const char* label);
            bool GetIOResult();

            const String& GetString(const char* key);
            void PushString(const char* label, const String& text);

        private:
            bool ParseSource(const String& src);

            bool m_io_result = false;
            String m_default_string;
            std::map<hash_t, String> m_data;
    };

    class LocaleManager {
        public:
            LocaleManager() = delete;
            LocaleManager(const LocaleManager&) = delete;

            static bool MountLocale(const char* lname, const char* lfname);
            static Locale& GetCurrentLocale();
            static void SetLocale(const char* lname);

        private:
            static void SetCurrentLocaleName(const char* farg);

            constexpr static char* m_locale_active = NULL;
            static std::map<hash_t, Locale> m_locales;
    };
}

#endif
