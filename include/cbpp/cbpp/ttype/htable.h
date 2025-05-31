#ifndef CBPP_HASH_TABLE_H
#define CBPP_HASH_TABLE_H

#include <stdint.h>

#include "cbpp/cb_hash.h"

namespace cbpp {
    template <typename key_t, typename value_t, typename mask_t = uint32_t> class Table {
        public:
            Table() = default;
            Table(hash_t (*pHasher)(const key_t&));

            T& operator[] (key_t& Key) noexcept;

            ~Table();

        private:
            struct Chunk {
                value_t m_aData[sizeof(mask_t)*8];
                mask_t m_iMask = (mask_t)(-1);
                Chunk* m_pNextChunk = NULL;
            } *m_pHead = NULL;

            hash_t (*m_pHasher)(const key_t&) = NULL;
    };
}

#endif
