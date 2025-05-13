#ifndef CBPP_SPSHEET_H
#define CBPP_SPSHEET_H

/*
    CBPP Texture Atlas (CTA) format.
    Stores sprites mapping info and one big texture
*/

#include "cbpp/vec2.h"
#include "cbvs/texture.h"
#include "cbpp/fileio.h"
#include "cbpp/asset/resource.h"

#include "cdf/cdf.h"
#include "sdk_structs.h"

#include <stdint.h>
#include <stddef.h>

#define CBPP_CTA_VERSION_MAJOR 1
#define CBPP_CTA_VERSION_MINOR 0

namespace cbpp {
    class SpriteSheet {
        public:
            SpriteSheet() = default;
            ~SpriteSheet() noexcept;

            bool Load(const char* sPath) noexcept;

            friend SpriteSheet* LoadSheet(const char* sPath) noexcept;

        private:
            bool LoadMapping(cdf_object* pSource) noexcept;
            bool LoadImage(cdf_object* pSource) noexcept;

            GLuint m_hTexture = 0;
            sdk_Sprite* m_aSprites = NULL;
            uint32_t m_iSprites = 0;
    };

    SpriteSheet* LoadSheet(const char* sPath) noexcept;
}

#endif
