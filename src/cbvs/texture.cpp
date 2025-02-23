#include "cbvs/texture.h"

#include "cbpp/fileio.h"
#include "cbpp/error.h"
#include "cbvs/error_check.h"

using namespace cbpp;

namespace cbvs {
    bool Texture::Load(const char* sPath, LOAD_FORMAT iLoadFormat, OGL_FLAG iGLflag) noexcept {
        File* hInput = OpenFile(PATH_TEXTURE, sPath, "rb");
        if(hInput == NULL) { return false; }

        size_t iFileLength = hInput->Length();
        uint8_t* aFileData = (uint8_t*) malloc(iFileLength);

        delete hInput;

        uint8_t* aImage = SOIL_load_image_from_memory(aFileData, iFileLength, (int*)&m_iWidth, (int*)&m_iHeight, NULL, iLoadFormat);    

        free(aFileData);

        m_hTexture = SOIL_create_OGL_texture(aImage, m_iWidth, m_iHeight, iLoadFormat, 0, iGLflag);  

        free(aImage);

        return glCheck();
    }

    GLuint Texture::GetHandle() const noexcept { return m_hTexture; }

    uint32_t Texture::Width() const noexcept { return m_iWidth; }
    uint32_t Texture::Height() const noexcept { return m_iHeight; }
}
