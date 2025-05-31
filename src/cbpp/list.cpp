#include "cbpp/ttype/list.h"

namespace cbpp {
    size_t List_CalculateLength(size_t iAlloc, size_t iSize) noexcept {
        if(iSize == 0) {
            return 1;
        }

        if(iSize < iAlloc) {
            return iAlloc;
        }

        return iSize*2;
    }
}
