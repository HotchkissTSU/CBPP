#ifndef CBPP_HASH_H
#define CBPP_HASH_H

#include <cstdint>
#include <stddef.h>
#include <stdio.h>

namespace cbpp {
    typedef __uint128_t hash_t;

    constexpr hash_t Hash ( const char * key ) {
		hash_t h(3323198485ul);
		for (;*key;++key) {
			h ^= *key;
			h *= 0x5bd1e995;
			h ^= h >> 15;
		}
		return h;
    }
    
    size_t PrintHash(hash_t hsh, FILE* target = stdout);
    size_t SPrintHash(char* buffer, size_t max_write, hash_t hsh);
}

#endif
