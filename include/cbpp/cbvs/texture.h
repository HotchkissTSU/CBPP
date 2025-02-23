#ifndef CBVS_TEXTURE_H
#define CBVS_TEXTURE_H

#include <stdint.h>

#include "glad/glad.h"
#include "SOIL/SOIL.h"

namespace cbvs {
    //A CPU-side handle for an OpenGL texture
    class Texture {
        public:
            enum LOAD_FORMAT : uint8_t {
                LOAD_AUTO = SOIL_LOAD_AUTO,
                LOAD_RGBA = SOIL_LOAD_RGBA,
                LOAD_RGB  = SOIL_LOAD_RGB,
                LOAD_L    = SOIL_LOAD_L,
                LOAD_LA   = SOIL_LOAD_LA
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

            bool Load(const char* sPath, LOAD_FORMAT iLoadFormat = LOAD_RGBA, OGL_FLAG iGLflag = FLAG_POWER_OF_2) noexcept;

            GLuint GetHandle() const noexcept;
            uint32_t Width() const noexcept;
            uint32_t Height() const noexcept;

        private:
            GLuint m_hTexture;
            uint32_t m_iWidth, m_iHeight;
    };
}

#endif
