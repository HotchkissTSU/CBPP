#ifndef CBPP_SPSHEET_H
#define CBPP_SPSHEET_H

/*
    CBPP Texture Atlas (CTA) format
*/

#include "cbvs/texture.h"
#include "cbpp/cbdef.h"
#include "cbpp/cbstring.h"

#include <stdint.h>
#include <stddef.h>
#include <unordered_map>

namespace cbpp {
    struct SpriteMapping {
        float_t X,Y,W,H;
    };

    //typedef std::unordered_map<CString, SpriteMapping> mapping_t;
    typedef uint32_t spriteid_t;

    struct SpriteInfo {
        const char* Name;
        SpriteMapping Mapping;
        GLuint TextureID;
    };
    
    spriteid_t GetSpriteID( const char* sName );
    const SpriteInfo& GetSpriteInfo( spriteid_t iSpriteID );

    /*
        Load and mount a providen spritesheet
    */
    bool LoadTextureSheet( const char* sPath, bool bAppendExt = true );

    void CompileTextureSheets();

    void CleanupSprites();
}

#endif
