#ifndef CBPP_SPSHEET_H
#define CBPP_SPSHEET_H

/*
    CBPP Texture Atlas (CTA) format.
    Stores sprites mapping info and one big texture
*/

#include "cbpp/vec2.h"
#include "cbvs/texture.h"
#include "cbpp/fileio.h"

#include <stdint.h>
#include <stddef.h>

#define CBPP_CTA_VERSION_MAJOR 1
#define CBPP_CTA_VERSION_MINOR 0

namespace cbpp {
    class SpriteSheet {
        public:
            struct Sprite {
                char* m_sName;
                float m_fX, m_fY, m_fW, m_fH;
            };
            
            SpriteSheet() = default;

            SpriteSheet(const SpriteSheet& hOther) = delete;
            SpriteSheet& operator=(const SpriteSheet& hOther) = delete;

            uint32_t SpriteID(const char* sSpriteName) const noexcept;
            const char* SpriteName(uint32_t iSpriteID) const noexcept;

            bool Load( const char* sSheetName ) noexcept;
            bool Save( const char* sSheetName ) const noexcept;

            ~SpriteSheet();

        private:
            GLuint* m_aSprites = NULL;
    };
}

#endif
