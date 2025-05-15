#include "cbpp/asset/spsheet.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"

#include <string.h>

#include "cbpp/asset/cdf_classes.h"

namespace cbpp {
    bool LoadMapping(cdf_object* pSource, sdk_Sprite*& pSpriteArray, uint32_t& iSprites) noexcept {
        if( !cdf_is_array(pSource) ) {
            return false;
        }

        cdf_uint iLength = cdf_array_length(pSource);

        pSpriteArray = (sdk_Sprite*) malloc(sizeof(sdk_Sprite) * iLength);
        iSprites = iLength;

        cdf_object Current;
        cdf_retcode iCode;

        for( size_t i = 0; i < iLength; i++ ) {
            iCode = cdf_array_index(pSource, &Current, i);
            if(iCode != CDF_OK) { return false; }

            if(cdf_object_length(&Current) != sizeof(sdk_Sprite)) {
                char sBuff[128];
                snprintf(sBuff, 128, "Invalid mapping info for the sprite '%d'", i);
                cbpp::PushError(ERROR_IO, sBuff);
                memset(&pSpriteArray[i], 0, sizeof(sdk_Sprite));
                continue;
            }

            memcpy(&pSpriteArray[i], cdf_object_data(&Current), sizeof(sdk_Sprite));
        }

        return true;
    }
    
    bool SpriteSheet::LoadImage(cdf_object* pSource, uint8_t*& pImageBytes, size_t& iImageLen) noexcept {
        pImageBytes = (uint8_t*)(cdf_object_data(pSource));
        iImageLen = cdf_object_length(pSource);
        return true;
    }

    bool SpriteSheet::LoadImgData(cdf_object* pSource, sdk_ImageInfo& ImgData) noexcept {
        if(cdf_object_length(pSource) != sizeof(sdk_ImageInfo)) {
            cbpp::PushError(ERROR_IO, "Invalid CTA image info");
            return false;
        }
        memcpy(&ImgData, cdf_object_data(pSource), sizeof(sdk_ImageInfo));
        return true;
    }

    bool SpriteSheet::Load(const char* sPath) noexcept {
        File* hFile = OpenFile(PATH_TEXTURE, sPath, "rb");
        if(hFile == NULL) { return false; }

        cdf_document* pDoc;
        cdf_verinfo Version;
        int16_t iClassID;

        cdf_retcode iCode = cdf_file_read((FILE*)hFile->Handle(), &pDoc, &Version, &iClassID);
        hFile->Close();

        if(iCode != CDF_OK) {
            cbpp::PushError(ERROR_CDF, cdf_get_error(iCode));
            return false;
        }

        if(iClassID != CDF_CLASS_SPRITESHEET) {
            cbpp::PushError(ERROR_IO, "The providen CDF file is not a spritesheet");
            cdf_document_destroy(pDoc);
            return false;
        }

        cdf_object* pRoot = cdf_document_root(pDoc);

        cdf_object Current;
        size_t iIter = 0;

        uint8_t bLoadInfo[3];
        memset(bLoadInfo, 0, 3);

        uint8_t* pImageBytes = NULL;
        size_t iImageLength = 0;
        sdk_ImageInfo ImgData;

        while( cdf_object_iterate(pRoot, &Current, &iIter) ) {
            const char* sName = cdf_object_name(pDoc, &Current);

            if( strcmp(sName, "cta_mapping") == 0 ) {
                bLoadInfo[0] = 1;
                if(!LoadMapping(&Current, m_aSprites, m_iSprites)) { cdf_document_destroy(pDoc); return false; }
            }

            if( strcmp(sName, "cta_raster") == 0 ) {
                bLoadInfo[1] = 1;
                if(!LoadImage(&Current, pImageBytes, iImageLength)) { cdf_document_destroy(pDoc); return false; }
            }

            if( strcmp(sName, "cta_imginfo") == 0 ) {
                bLoadInfo[2] = 1;
                if(!LoadImgData(&Current, ImgData)) { cdf_document_destroy(pDoc); return false; }
            }
        }

        for(int i = 0; i < sizeof(bLoadInfo); i++) {
            if(bLoadInfo[i] == 0) {
                cbpp::PushError(ERROR_IO, "Incomplete CTA file");
                cdf_document_destroy(pDoc);
                return false;
            }
        }

        GLenum iSourceChannels;
        switch(ImgData.m_iChannels) {
            case 1:
                iSourceChannels = GL_RED; break;

            case 2:
                iSourceChannels = GL_RG; break;
            
            case 3:
                iSourceChannels = GL_RGB; break;

            case 4:
                iSourceChannels = GL_RGBA; break;

            default:
                iSourceChannels = GL_RGBA;
        }

        glGenTextures(1, &m_hTexture);
        glBindTexture(GL_TEXTURE_2D, m_hTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImgData.m_iWidth, ImgData.m_iHeight, 0, iSourceChannels, GL_UNSIGNED_BYTE, pImageBytes);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        cdf_document_destroy(pDoc);
        return true;
    }
}

namespace cbpp {
    SpriteSheet* LoadSheet(const char* sPath) noexcept {
        SpriteSheet* pSheet = new SpriteSheet();
        if(!pSheet->Load(sPath)) {
            return NULL;
        }

        return pSheet;
    }
}
