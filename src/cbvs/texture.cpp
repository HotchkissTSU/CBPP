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

        if(!bIsFallback) { //We really should not free the default texture buffer
            SOIL_free_image_data(aImage);
        }

        return !bIsFallback;
    }
    
    bool Texture::Load(const Image& refImage) noexcept {
        m_hTexture = SOIL_create_OGL_texture(refImage.Array(), refImage.Width(), refImage.Height(), refImage.Channels(), NULL, FLAG_POWER_OF_2);
        m_iWidth = refImage.Width();
        m_iHeight = refImage.Height();

        return true;
    }

    GLuint Texture::GetHandle() const noexcept { return m_hTexture; }

    uint32_t Texture::Width() const noexcept { return m_iWidth; }
    uint32_t Texture::Height() const noexcept { return m_iHeight; }

    void Texture::Free() const noexcept {
        glDeleteTextures(1, &m_hTexture);
    }
}

namespace cbvs {
    uint8_t* Image::Convert(const Image& refImage, IMG_FORMAT iTargetFormat) noexcept {
        size_t iNewLength = refImage.Width() * refImage.Height() * iTargetFormat;
        uint8_t* pNewImage = (uint8_t*)malloc(iNewLength);
    }

    Image::Image(const uint8_t* aPixels, uint16_t iX, uint16_t iY, IMG_FORMAT iChannels) noexcept : m_iH(iY), m_iW(iX), m_iChannels(iChannels) {
        size_t iArraySize = (uint8_t)iChannels * iX * iY;
        m_aPixels = (uint8_t*)malloc(iArraySize);
        memcpy(m_aPixels, aPixels, (uint8_t)iChannels * iX * iY);
    }

    bool Image::Render(const Image& refOther, uint16_t iX, uint16_t iY, bool bClip) noexcept {
        if(iX > m_iW || iY > m_iH) {
            return false;
        }

        if(refOther.Channels() != m_iChannels) {
            cbpp::PushError(cbpp::ERROR_MEM, "Images` formats mismatch");
            return false;
        }

        uint16_t iWidth = refOther.Width();
        uint16_t iHeight = refOther.Height();

        if((iX + iWidth > m_iW) || (iY + iHeight > m_iH)) {
            if(bClip) {
                iWidth = cbpp::math::Clamp<uint16_t>(iWidth, 0, m_iW - iX);
                iHeight = cbpp::math::Clamp<uint16_t>(iHeight, 0, m_iH - iY);
            }else{
                cbpp::PushError(cbpp::ERROR_MEM, "Image sticks out of the target canvas with clipping disabled");
                return false;
            }
        }

        uint8_t* pRow;
        for(uint16_t iRow = 0; iRow < iHeight; iRow++) {
            pRow = m_aPixels + (iRow + iY)*m_iChannels*m_iW;
            memcpy(pRow + iX, refOther.Array() + iRow*m_iChannels, iWidth*m_iChannels);
        }
    }

    Image::~Image() {
        if(m_aPixels != NULL) {
            free(m_aPixels);
        }
    }
}
