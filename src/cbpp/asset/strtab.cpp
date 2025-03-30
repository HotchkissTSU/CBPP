#include "cbpp/asset/strtab.h"

#include <string.h>
#include <stdlib.h>

namespace cbpp {
    bool StringTable::PushString(const char* sSource, bool* pResult) noexcept {
        size_t iLength = strlen(sSource);

        char* pTemp = (char*) realloc(m_aStrings, m_iLength + iLength + 1);
        if(pTemp == NULL) {
            if(pResult != NULL) {
                *pResult = false;
                return 0;
            }
        }

        m_aStrings = pTemp;
        memcpy(m_aStrings + m_iLength + 1, sSource, iLength+1);
        m_iLength += iLength+1;

        if(pResult != NULL) {
            *pResult = true;
        }

        m_iAmount++;

        return m_iAmount-1;
    }
}
