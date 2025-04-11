#include "cbpp/asset/spsheet.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"

#include "bson/bson.h"

#include <string.h>

namespace cbpp {
    bool SpriteSheet::Load(const char* sPath) noexcept {
        File* hFile = OpenFile(PATH_TEXTURE, sPath);
        if(hFile == NULL) { return false; }

        m_pDoc = cdf_document_read(hFile->Handle());
        if(m_pDoc == NULL) { return false; }
    }
}

namespace cbpp {
    SpriteSheet* LoadSheet(const char* sPath) noexcept {
        SpriteSheet* pSheet = new SpriteSheet();
        if(!pSheet->Load()) {
            return NULL;
        }

        return pSheet;
    }
}
