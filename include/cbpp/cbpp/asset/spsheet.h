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
        float_t X,Y,W,H; // W and H here are not width and height (surprise!) - they are the coordinates of the top-right corner of the UV rectangle
    };

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

    //void CompileTextureSheets();

    // Deallocate all currently used sprites
    void CleanupSprites();
}

#endif
