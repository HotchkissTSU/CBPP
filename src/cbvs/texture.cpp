#include "cbvs/texture.h"

#include "cbpp/fileio.h"
#include "cbpp/error.h"
#include "cbvs/error_check.h"
#include "cbpp/texture_default.h"

using namespace cbpp;

namespace cbvs {
    bool Texture::Load(const char* sPath, IMG_FORMAT iLoadFormat, OGL_FLAG iGLflag) noexcept {
        File* hInput = OpenFile(PATH_TEXTURE, sPath, "rb");
        if(hInput == NULL) { return false; }

        size_t iFileLength = hInput->Length();
        uint8_t* aFileData = (uint8_t*) malloc(iFileLength);

        hInput->Read(aFileData, iFileLength);

        delete hInput;

        uint8_t* aImage = SOIL_load_image_from_memory(aFileData, iFileLength, (int*)&m_iWidth, (int*)&m_iHeight, NULL, iLoadFormat);    

        bool bIsFallback = false;
        if(aImage == NULL) {
            aImage = (uint8_t*)cbpp::g_aDefaultTexture;
            m_iHeight = 32;
            m_iWidth = 32;
            bIsFallback = true;
        }

        free(aFileData);

        m_hTexture = SOIL_create_OGL_texture(aImage, m_iWidth, m_iHeight, iLoadFormat, 0, iGLflag);  

        if(!bIsFallback) {
            SOIL_free_image_data(aImage);
        }

        return glCheck();
    }

    GLuint Texture::GetHandle() const noexcept { return m_hTexture; }

    uint32_t Texture::Width() const noexcept { return m_iWidth; }
    uint32_t Texture::Height() const noexcept { return m_iHeight; }

    Texture::~Texture() {
        if(glIsTexture(m_hTexture)) {
            glDeleteTextures(1, &m_hTexture);
        }
    }
}
