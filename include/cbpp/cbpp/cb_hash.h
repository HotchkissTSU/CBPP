#ifndef CBPP_HASH_H
#define CBPP_HASH_H

#include <cstdint>
#include <stddef.h>
#include <stdio.h>

namespace cbpp {
    typedef __uint128_t hash_t;

    template <typename T = hash_t> constexpr T Hash ( const char * key ) {
		T h(3323198485ul);
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
