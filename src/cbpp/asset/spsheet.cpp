#include "cbpp/asset/spsheet.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"

#include "bson/bson.h"

#include <string.h>

namespace cbpp {
    
}

namespace cbpp {
    bool SpriteSheet::Load(const char* sSheetName) {
        File* hInput = OpenFile(PATH_TEXTURE, sSheetName);

        if(hInput == NULL) { return false; }

        size_t iFileLen = hInput.Length();
        uint8_t* aBuffer = (uint8_t*) malloc(iFileLen);

        hInput->Read(aBuffer, iFileLen, 1);

        bson_t* hRoot = bson_new_from_buffer(&aBuffer, iFileLen);
    }

    SpriteSheet::~SpriteSheet() {
    }
}
