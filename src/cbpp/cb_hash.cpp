#include "cbpp/cb_hash.h"

namespace cbpp {
    size_t PrintHash(hash_t hsh, FILE* target) {
        union {
            uint64_t first8, last8;
            hash_t whole16;
        } _u;

        _u.whole16 = hsh;

        return fprintf(target, "%x%x", _u.first8, _u.last8);
    }

    size_t SPrintHash(char* buffer, size_t max_write, hash_t hsh) {
        union {
            uint64_t first8, last8;
            hash_t whole16;
        } _u;

        _u.whole16 = hsh;

        return snprintf(buffer, max_write, "%x%x", _u.first8, _u.last8);
    }
}
