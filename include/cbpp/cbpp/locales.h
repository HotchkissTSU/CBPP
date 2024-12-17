#ifndef CBPP_LOCALES_H
#define CBPP_LOCALES_H

#include "cbpp/cbstring.h"
#include "cbpp/cb_hash.h"

#include <uchar.h>
#include <map>

//The maximal length of the label name
#define CBPP_LOCALE_MAXNAME 64

//The maximal length of the labeled text
#define CBPP_LOCALE_MAXTEXT 512

namespace cbpp {
    class Locale {
        public:
            Locale(){};

            Char* Load(const char* fname);
            bool Save(const char* fname);

            bool HasLabel(const char* label);
            bool GetIOResult();

            const String& GetString(const char* key);
            void PushString(const char* label, const String& text);

            bool ParseSource(const Char* src);
            
            void SetName(const String& nm);
            const String& GetName();

            String m_sDefaultEntry;

            void operator=(Locale& other);

        private:
            /*
                Removes commentaries and newlines
            */
            Char* Preprocess(const Char* src);
            bool ProcessPair(const Char* name, const Char* text);

            bool m_bRWresult = false, m_bValid = false;
            String m_sPrettyName;
            std::map<hash_t, String> m_mData;
    };

    /*
        Load and store a new locale from a file by it`s short name,
        like "ru" instead of "assets/locales/ru.json" etc.
    */
    void MountLocale(const char* lfname);
    Locale* GetCurrentLocale() noexcept;

    /*
        Attempt to change the current locale.
        A new one must be mounted before this call.
    */
    bool SetLocale(const char* lname) noexcept;

    extern Locale* g_pCurrentLocale;
    extern std::map<hash_t, Locale> g_mLocales;
}

#endif
