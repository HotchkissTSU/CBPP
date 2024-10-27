#include "asset.h"

namespace cbpp {
    SegArray<SearchPath_t> assets_search_paths;

    size_t assets_sp_ln = 1;
    PathID last_pid = 1;

    PathID AddAssetSearchPath( const char* path, int8_t priority ) {
        
    }
}