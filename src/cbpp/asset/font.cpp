#include "cbpp/asset/font.h"

#include "cbpp/asset/sdk_structs.h"
#include "cbpp/fileio.h"

namespace cbpp {
    bool LoadFont(const char* sPath, bool bAppendEx = true) {
        const char* sExt;

        if(bAppendEx) { sExt = ".cff"; }
        else{ sExt = ""; }

        char sPathBuffer[128];
        snprintf(sPathBuffer, 128, "%s%s", sPath, sExt);

        File* hInput = OpenFile();
    }
}
