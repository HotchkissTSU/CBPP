#ifndef CBPP_ASSET_H
#define CBPP_ASSET_H

#include "cbpp/fileio.h"
#include "cbpp/ttype/segarr.h"

#include <cstdint>
#include <stddef.h>

namespace cbpp {
    typedef int32_t PathID;

    struct SearchPath_t {
        PathID pid = -1;
        int8_t priority = 0;
        char* path = NULL;
    };

    extern SegArray<SearchPath_t> assets_search_paths;
    extern size_t assets_sp_ln;

    PathID AddAssetSearchPath( const char* path, int8_t priority );
    void RemoveAssetSearchPath( PathID pid );

    const char* FindAsset( const char* name );
};

#endif
