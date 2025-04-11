#ifndef CBPP_SPSHEET_H
#define CBPP_SPSHEET_H

/*
    CBPP Texture Atlas (CTA) format.
    Stores sprites mapping info and one big texture
*/

#include "cbpp/vec2.h"
#include "cbvs/texture.h"
#include "cbpp/fileio.h"

#include "cdf/cdf.h"

#include <stdint.h>
#include <stddef.h>

#define CBPP_CTA_VERSION_MAJOR 1
#define CBPP_CTA_VERSION_MINOR 0

namespace cbpp {
    class SpriteSheet {
        public:
            struct Sprite {
                size_t iNameID;
                float fX, fY;
                float fW, fH;
            };

            SpriteSheet() = default;
            ~SpriteSheet() noexcept;

            bool Load(const char* sPath) noexcept;

            friend SpriteSheet* LoadSheet(const char* sPath) noexcept;

        private:
            GLuint m_hTexture = 0;
            cdf_document* m_pDoc = NULL;
    };

    SpriteSheet* LoadSheet(const char* sPath) noexcept;
}

#endif
