#include "cbpp/asset/spsheet.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"

#include <string.h>

#include "cbpp/asset/cdf_classes.h"

namespace cbpp {
    bool SpriteSheet::LoadMapping(cdf_object* pSource) noexcept {
        if( !cdf_is_array(pSource) ) {
            return false;
        }

        cdf_uint iLength = cdf_array_length(pSource);

        m_aSprites = (Sprite*) malloc(sizeof(Sprite) * iLength);
        m_iSprites = iLength;

        cdf_object Current;
        cdf_retcode iCode;

        for( size_t i = 0; i < iLength; i++ ) {
            iCode = cdf_array_index(pSource, &Current, i);
            if(iCode != CDF_OK) { return false; }

            memcpy(&m_aSprites[i], cdf_object_data(&Current), sizeof(Sprite));
        }

        return true;
    }

    bool SpriteSheet::Load(const char* sPath) noexcept {
        File* hFile = OpenFile(PATH_TEXTURE, sPath, "rb");
        if(hFile == NULL) { return false; }

        cdf_document* pDoc;
        cdf_verinfo Version;
        int16_t iClassID;

        cdf_retcode iCode = cdf_file_read((FILE*)hFile->Handle(), &pDoc, &Version, &iClassID);

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
        size_t iIter;

        while( cdf_object_iterate(pRoot, &Current, &iIter) ) {
            const char* sName = cdf_object_name(pDoc, &Current);

            if( strcmp(sName, "mapping") == 0 ) {
                if(!this->LoadMapping(&Current)) { cdf_document_destroy(pDoc); return false; }
            }

            if( strcmp(sName, "image") == 0 ) {
                if(!this->LoadImage(&Current)) { cdf_document_destroy(pDoc); return false; }
            }
        }

        hFile->Close();

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
