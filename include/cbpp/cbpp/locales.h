#ifndef CBPP_LOCALES_H
#define CBPP_LOCALES_H

#include "cbpp/cbstring.h"

#include <map>

namespace cbpp {
    String GetLocaleText(std::string text_id);

    class LocaleManager {
        public:
            std::string GetCurrentLocale();
            void SetLocale(std::string new_locale_name);

            String GetString(std::string text_id);

            bool MountLocale(std::string locale_name, std::string file_path);
            bool MountLocales(std::string locales_def_path);

        private:
            std::string cur_locale;
            std::map< std::string, std::map<std::string, String> > localemap;
    };

    LocaleManager* Locale();
}

#endif
