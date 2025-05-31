#ifndef CBPP_SPSHEET_H
#define CBPP_SPSHEET_H

/*
    CBPP Texture Atlas (CTA) format
*/

#include "cbpp/vec2.h"
#include "cbvs/texture.h"
#include "cbpp/fileio.h"
#include "cbpp/cbdef.h"
#include "cbpp/cbstring.h"
#include "cbpp/ttype/list.h"

#include <stdint.h>
#include <stddef.h>
#include <unordered_map>

namespace cbpp {
    struct SpriteMapping {
        float_t X,Y,W,H;
    };

    extern std::unordered_map<CString, SpriteMapping> g_mSpriteMapping;

    /*
        Load and map a new spritesheet
    */
    bool AppendTextureSheet( const char sPath );
}

#endif
