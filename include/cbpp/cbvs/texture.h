#ifndef CBVS_TEXTURE_H
#define CBVS_TEXTURE_H

#include <stdint.h>

#include "glad/glad.h"
#include "SOIL/SOIL.h"

#include "cbpp/geomath.h"

namespace cbvs {
    enum IMG_FORMAT : uint8_t {
        IMG_AUTO = SOIL_LOAD_AUTO,
        IMG_RGBA = SOIL_LOAD_RGBA,
        IMG_RGB  = SOIL_LOAD_RGB,
        IMG_L    = SOIL_LOAD_L,
        IMG_LA   = SOIL_LOAD_LA
    };

    enum OGL_FLAG : uint16_t {
        FLAG_POWER_OF_2 = SOIL_FLAG_POWER_OF_TWO,
        FLAG_MIPMAPS = SOIL_FLAG_MIPMAPS,
        FLAG_REPEATS = SOIL_FLAG_TEXTURE_REPEATS,
        FLAG_MUL_ALPHA = SOIL_FLAG_MULTIPLY_ALPHA,
        FLAG_INV_Y = SOIL_FLAG_INVERT_Y,
        FLAG_DXT_COMPRESS = SOIL_FLAG_COMPRESS_TO_DXT,
        FLAG_DDS_LOAD_DIRECT = SOIL_FLAG_DDS_LOAD_DIRECT,
        FLAG_NTSC_SAFE_RGB = SOIL_FLAG_NTSC_SAFE_RGB,
        FLAG_CoCg_Y = SOIL_FLAG_CoCg_Y,
        FLAG_RECTANGLE = SOIL_FLAG_TEXTURE_RECTANGLE
    };

    //A CPU-side mutable image datatype
    class Image {
        public:
            static uint8_t* Convert(const Image& refImage, IMG_FORMAT iTargetFormat) noexcept;

            Image(const uint8_t* aPixels, uint16_t iX, uint16_t iY, IMG_FORMAT iChannels) noexcept;

            uint8_t* At(uint16_t iX, uint16_t iY) noexcept;

            //Write another image at the specified coordinates
            bool Render(const Image& refOther, uint16_t iX, uint16_t iY, bool bClip = false) noexcept;

            uint16_t Width() const noexcept { return m_iW; }
            uint16_t Height() const noexcept { return m_iH; }
            IMG_FORMAT Channels() const noexcept { return m_iChannels; }
            const uint8_t* Array() const noexcept { return const_cast<const uint8_t*>(m_aPixels); }
            uint8_t* Array() noexcept { return m_aPixels; }

            ~Image();

        private:
            uint16_t m_iW = 0, m_iH = 0;
            IMG_FORMAT m_iChannels;
            uint8_t* m_aPixels = NULL;
    };

    //A CPU-side handle for an OpenGL texture
    class Texture {
        public:
            Texture() {}

            bool Load(const char* sPath, IMG_FORMAT iLoadFormat = IMG_RGBA, OGL_FLAG iGLflag = (OGL_FLAG)(FLAG_POWER_OF_2 | FLAG_INV_Y)) noexcept;
            bool Load(const Image& refImage) noexcept;

            GLuint GetHandle() const noexcept;
            uint32_t Width() const noexcept;
            uint32_t Height() const noexcept;

            void Free() const noexcept;

        private:
            GLuint m_hTexture;
            uint32_t m_iWidth, m_iHeight;
    };

    Image* LoadImage(const char* sPath, IMG_FORMAT iLoadFormat = IMG_RGBA) noexcept;
    Texture* LoadTexture();
}

#endif
