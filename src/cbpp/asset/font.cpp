#include "cbpp/asset/font.h"

#include <stdint.h>

#include "cbpp/fileio.h"
#include "SOIL/stb_truetype.h"
#include "cbpp/error.h"

namespace cbpp {
    bool LoadFont(const char* sFile, const char* sAlias, bool bSDF) {
        File* hInput = OpenFile(PATH_FONT, sFile, "rb");

        List<uint8_t> aFileContent(hInput->Length());
        hInput->Read(aFileContent.Array(), hInput->Length());
        delete hInput;

        const int iFntAmount = stbtt_GetNumberOfFonts(aFileContent.Array());
        if( iFntAmount  == -1 ) {
            PushError(ERROR_IO, "Invalid font data");
            return false;
        }else if( iFntAmount > 1 ) {
            PushError(ERROR_IO, "TTC is not supported");
            return false;
        }

        stbtt_fontinfo FontInfo;
        stbtt_InitFont(&FontInfo, aFileContent.Array(), 0);

        for(uint32_t i = 0; i < 0xFFFF; i++) { //dumb, dumb, dumb!
            int iIndex = stbtt_FindGlyphIndex(&FontInfo, i);

            if(iIndex == 0) { continue; } // no glyth for this one

            int iWidth, iHeight, iXOff, iYOff;
            float fScale = stbtt_ScaleForPixelHeight(&FontInfo, 32);

            unsigned char* pSDF = stbtt_GetGlyphSDF(&FontInfo, 
                fScale,
                iIndex,
                5,
                180,
                36.0f,
                &iWidth, &iHeight,
                &iXOff, &iYOff
            );
        }

        return true;
    }
}
