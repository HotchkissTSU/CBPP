#ifndef CBPP_ASSET_H
#define CBPP_ASSET_H

#include "cbpp/fileio.h"

namespace cbpp {
    extern const char** AssetPaths;

    std::string ResolveAssetPath(std::string& local_path);
}

#endif