#ifndef CBPP_SPSHEET_H
#define CBPP_SPSHEET_H

/*
    CBPP Texture Atlas (CTA) format
*/

#include "cbvs/texture.h"
#include "cbpp/cbdef.h"
#include "cbpp/ttype/list.h"

#include <stdint.h>
#include <stddef.h>

namespace cbpp {
    struct SpriteMapping {
        float_t X,Y; // Bottom-left corner of the UV
        float_t W,H; // Top-right corner of the UV
    };

    typedef uint32_t spriteid_t;

    struct SpriteInfo {
        const char* Name;
        SpriteMapping Mapping;
        GLuint TextureID;
    };
    
    spriteid_t GetSpriteID( const char* sName );
    const SpriteInfo& GetSpriteInfo( spriteid_t iSpriteID );

    class Font {
        public:
            struct Glyth {
                float_t fX, fY;       //Drawing origin
                float_t fWidth;       //Kinda self-explanatory
                float_t fBearY;       //Y bearing
                float_t fMaxBBoxY;    //bbox_ymax
            };

            struct Language {
                const char* Name;
                size_t Offset;
                List<Glyth> Mapping;
            };

        private:
            GLuint m_hTexture;
            List<Language> m_aLangs;
    };

    /*
        Load and mount a providen sheet-based file. Currently supported:
        .cta .cff
    */
    bool LoadSheet( const char* sPath, bool bAppendExt = true );

    //void CompileTextureSheets();

    // Deallocate all currently used sprites
    void CleanupSprites();
}

#endif
