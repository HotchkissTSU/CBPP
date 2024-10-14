#include "cbpp/locales.h"

#include "cbpp/error.h"
#include "json/json.h"

namespace cbpp {
    LocaleManager* Locale() {
        static LocaleManager locale_man;
        return &locale_man;
    }

    std::string LocaleManager::GetCurrentLocale() {
        return cur_locale;
    }

    void LocaleManager::SetLocale(std::string new_locale) {
        if(localemap.count(new_locale) <= 0) {
            char err_log[256];
            snprintf(err_log, 256, "Attempt to set unknown locale '%s'", new_locale.c_str());
            CbThrowError(err_log);
            return;
        }

        cur_locale = new_locale;
    }

    String LocaleManager::GetString(std::string text_id) {
        if(localemap.at(cur_locale).count(text_id) <= 0) {
            char err_log[256];
            snprintf(err_log, 256, "Locale '%s' does not have any string labeled as '%s'", cur_locale.c_str(), text_id.c_str());
            CbThrowError(err_log);

            return String(U"[UNDEFINED STRING LABEL]");
        }

        return localemap.at(cur_locale).at(text_id);
    }

    String GetLocaleString(std::string text_id) {
        return Locale()->GetString(text_id);
    }
}