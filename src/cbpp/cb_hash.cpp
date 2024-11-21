#include "cbpp/cb_hash.h"

namespace cbpp {
    size_t PrintHash(const hash_t& hsh, FILE* target) {
        uint64_t begin, end; //first and last 8 bytes
        begin = hsh >> 64;
        end = hsh & ((hash_t)(-1) >> 64);

        return fprintf(target, "%x%x", begin, end);
    }

    size_t SPrintHash(char* buffer, size_t max_write, const hash_t& hsh) {
        uint64_t begin, end; //first and last 8 bytes
        begin = hsh >> 64;
        end = hsh & ((hash_t)(-1) >> 64);

        return snprintf(buffer, max_write, "%x%x", begin, end);
    }
}