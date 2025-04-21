#ifndef CBPP_CDF_CLASSES_H
#define CBPP_CDF_CLASSES_H

namespace cbpp {
    /*
        All new entries MUST appear after the existing ones to maintain 
        compatibility with any older engine builds
    */
    enum : uint16_t {
        CDF_CLASS_SPRITESHEET,
        CDF_CLASS_MAP
    };
}

#endif
