#ifndef CBPP_BITMASK_H
#define CBPP_BITMASK_H

namespace cbpp {
    template <int BITS = 16> struct BitMask {
        unsigned char m_aBytes[ BITS / 8 ];
    };
}

#endif