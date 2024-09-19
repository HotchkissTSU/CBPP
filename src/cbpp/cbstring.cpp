#include "cbpp/cbstring.h"
#include <codecvt>

namespace cbpp {
    std::wstring StringToWString(std::string src) {
        std::wstring out;
        for(int i = 0; i < src.length(); i++) {
            out[i] = (Char)src[i];
        }

        return out;
    }

    std::string WStringToString(std::wstring src) {
        std::string out;
        for(int i = 0; i < src.length(); i++) {
            out[i] = (char)src[i];
        }

        return out;
    }
}