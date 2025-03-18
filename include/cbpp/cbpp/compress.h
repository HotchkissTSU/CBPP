#ifndef CBPP_COMPRESS_H
#define CBPP_COMPRESS_H

#include "zlib/zlib.h"

#include <stdint.h>
#include <stddef.h>

namespace cbpp {
    bool Deflate(uint8_t* pChunkIn, size_t pInputLength, uint8_t*& refChunkOut, size_t& refOutLength) noexcept;
}

#endif
