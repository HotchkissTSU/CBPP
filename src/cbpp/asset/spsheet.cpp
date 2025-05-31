#include "cbpp/asset/spsheet.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"

#include <string.h>

#include "cbpp/asset/cdf_classes.h"

namespace cbpp {
    List<SpriteSheet*> g_aSheets; 

    SpriteSheet::SpriteSheet() noexcept {
        size_t iFreeIndex = g_aSheets.Find(NULL);
        if(iFreeIndex == (size_t)(-1)) {
            iFreeIndex = g_aSheets.PushBack(this); //Register ourselves in the global spritesheets list
        }else {
            g_aSheets[iFreeIndex] = this;
        }

        m_ID = iFreeIndex;
    }

    bool SpriteSheet::LoadMapping(cdf_object* pSource, sdk_Sprite*& pSpriteArray, uint32_t& iSprites) noexcept {
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
                char sBuffer[512];
                const char* sErrorMsg;

                switch (i) {
                    case 0:
                        sErrorMsg = "cta_mapping";
                        break;

                    case 1:
                        sErrorMsg = "cta_raster";
                        break;

                    case 2:
                        sErrorMsg = "cta_imginfo";
                        break;
                }

                snprintf(sBuffer, 512, "Imcomplete CTA file : field '%s' is missing!", sErrorMsg);
                cbpp::PushError(ERROR_IO, sBuffer);
                return false;
            }
        }

        m_Atlas.SetData(pImageBytes, ImgData.m_iResolution, ImgData.m_iResolution, (cbvs::IMG_FORMAT)(ImgData.m_iChannels), true);

        cdf_document_destroy(pDoc);
        return true;
    }

    cbvs::Image& SpriteSheet::GetAtlas() noexcept {
        return m_Atlas;
    }

    texres_t SpriteSheet::Resolution() noexcept {
        return m_Atlas.Width();
    }

    SpriteSheet::~SpriteSheet() noexcept {
        g_aSheets[m_ID] = NULL;
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
