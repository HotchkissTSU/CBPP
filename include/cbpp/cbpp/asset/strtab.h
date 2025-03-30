#ifndef CBPP_ASSET_STRINGTABLE_H
#define CBPP_ASSET_STRINGTABLE_H

#include <stdint.h>

#include "cbpp/fileio.h"

namespace cbpp {
    struct StringTable {
        uint32_t m_iAmount = 0;
        size_t m_iLength = 0;
        char* m_aStrings = NULL;

        uint32_t PushString(const char* sString, bool* pResult = NULL) noexcept;
        bool RemoveString(uint32_t iIndex) noexcept;

        bool Read(File* hInput) noexcept;
        bool Write(File* hOutput) const noexcept;
    };
}

#endif
