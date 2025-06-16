#ifndef CBVS_TEXTURE_H
#define CBVS_TEXTURE_H

#include <stdint.h>

#include "glad/glad.h"
#include "SOIL/SOIL.h"

#include "cbpp/cbdef.h"
#include "cbpp/color.h"

namespace cbvs {
    typedef cbpp::texres_t texres_t;

    /*
        A CPU-side image data holder. Stores color data in the RGBA format,
        so this is better to create your textures using it to avoid costly converting process.
    */
    class Image {
        public:
            Image() = default;
            Image(const uint8_t* aPixels, texres_t iW, texres_t iH, int iSrcChannels) noexcept;
            Image(texres_t iW, texres_t iH) noexcept;

            texres_t Width() const noexcept { return m_iW; }
            texres_t Height() const noexcept { return m_iH; }

            // Check whether this image can be used as a spritesheet (both width and height are POT)
            bool IsSheetCompatible() const noexcept;

            GLuint GenerateTexture() const noexcept;

            const cbpp::Color* GetPixelData() const noexcept { return const_cast<const cbpp::Color*>(m_aImageData); }
            cbpp::Color* GetPixelData() noexcept { return m_aImageData; }

            ~Image();

        private:
            texres_t m_iW = 0, m_iH = 0;
            cbpp::Color* m_aImageData = NULL;
    };

    GLuint CreateTexture(const cbpp::Color* pImage, texres_t iW, texres_t iH, bool bFlipY = false);
    GLuint CreateTexture(const Image& SourceImage, bool bFlipY = false);

    Image* GetImageFromTexture(GLuint hTexID, texres_t iWidth, texres_t iHeight);
}

#endif
