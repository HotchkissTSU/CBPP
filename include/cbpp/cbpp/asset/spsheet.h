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

    typedef std::unordered_map<CString, SpriteMapping> mapping_t;

    struct SpriteInfo {
        SpriteMapping Mapping;
        GLuint TextureID;
    };
    
    class SpriteSheet {
        friend bool LoadTextureSheet( const char* sPath );
        friend void CompileTextureSheets();
        friend SpriteInfo GetSpriteInfo( const char* sName );
        friend void RemapTextureSheet(SpriteSheet& Target, texres_t iOldW, texres_t iOldH, texres_t iNewW, texres_t iNewH);

        public:
            SpriteSheet() = default;
            ~SpriteSheet();

            const SpriteMapping& GetSpriteUV(const char* sName) const;
            GLenum GetTextureID() const;

            void GetResolution(texres_t& iW, texres_t& iH) const;

        private:
            GLuint m_hTexture;
            texres_t m_iWidth, m_iHeight;
            mapping_t m_mMapping;
    };
    
    SpriteInfo GetSpriteInfo( const char* sName );

    bool LoadTextureSheet( const char* sPath );
    void CompileTextureSheets();
}

#endif
