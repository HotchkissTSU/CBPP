#ifndef CBPP_POOL_H
#define CBPP_POOL_H

#include <stdint.h>
#include <stddef.h>

namespace cbpp {
    template <typename data_t, size_t iChunkSize> struct PoolChunk {
        data_t m_aChunk[iChunkSize];
    };

    template <typename data_t, typename mask_t = uint32_t> class Pool {
        public:
            Pool() = default;

        private:
            PoolChunk<data_t, mask_t>* m_aChunks = NULL;
    };
}

#endif
