#include "cbpp/asset/font.h"

#include <stdint.h>
#include <math.h>

#include "cbpp/fileio.h"
#include "SOIL/stb_truetype.h"
#include "cbpp/error.h"
#include "cbpp/geomath.h"
#include "SOIL/SOIL.h"
#include "cbpp/bit.h"
#include "cbvs/texture.h"

namespace cbpp {
    constexpr static const UnicodeRange g_aUnicodeRanges[] = {
        {0x0020, 0x007F},   // ASCII
        {0x0400, 0x04FF},   // Cyrillic
        {0x00A0, 0x00FF},   // Latin aux
        {0x0020, 0xFFFF},   // Let the VRAM erupt in flames
    };

    List<Font> g_aFonts;

    constexpr UnicodeRange GetUnicodeRange( EUnicodeRangeIndex iIndex ) {
        #ifdef CBPP_DEBUG
        if(iIndex >= UTF_RANGES_AMOUNT) { return g_aUnicodeRanges[UTF_RANGE_ASCII]; }
        #endif

        return g_aUnicodeRanges[(int)iIndex];
    }

    constexpr EUnicodeRangeIndex FindUnicodeRangeIndex(uint32_t iCodepoint) {
        for( int i = 0; i < UTF_RANGES_AMOUNT; i++ ) {
            const UnicodeRange Range = g_aUnicodeRanges[i];
            if( math::InRange(i, Range.iBegin, Range.iEnd) ) {
                return (EUnicodeRangeIndex)(i);
            }
        }

        return UTF_RANGE_FULL;
    }

    constexpr EUnicodeRangeIndex UnicodeRangeByLoadMask(EFontLoadMask iMask) {

        /*
            Find the first non-zero bit and output it`s index,
            converting a bitmask to an integer unicode range id
        */

        for( int k = 0; k < sizeof(EFontLoadMask)*8; k++ ) {
            if( ((int)(iMask) >> k) == 1 ) {
                return (EUnicodeRangeIndex)(k);
            }
        }

        return UTF_RANGE_ASCII;
    }

    bool LoadFontBitmap(const char* sName, const char* sAlias, int iHeight, EFontLoadMask iMask) {
        File* hInput = OpenFile(PATH_FONT, sName, "rb");

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
        if(stbtt_InitFont(&FontInfo, aFileContent.Array(), 0) < 0) {
            PushError(ERROR_IO, "Failed to parse font data (bad!)");
        }

        List<stbtt_pack_range> aPackRanges;
        List<stbtt_packedchar*> aPackedChars;

        uint32_t iUsedGlythsAmount = 0;

        if(TestBitmask(iMask, LOAD_ALL)) {
            iMask = LOAD_ALL;
        }

        for(int i = 0; i < UTF_RANGES_AMOUNT; i++) {
            if( TestBitmask((int)iMask, 1<<i) ) {
                const UnicodeRange Range = GetUnicodeRange( UnicodeRangeByLoadMask((EFontLoadMask)(1 << i)) );
                const uint32_t iRangeLength = Range.iEnd - Range.iBegin;

                stbtt_pack_range ttRange;
                
                stbtt_packedchar* CharData = Malloc<stbtt_packedchar>(iRangeLength);
                ttRange.first_unicode_codepoint_in_range = Range.iBegin;
                ttRange.num_chars = iRangeLength;
                ttRange.chardata_for_range = CharData;
                ttRange.font_size = (float)iHeight;
                ttRange.array_of_unicode_codepoints = NULL;

                aPackRanges.PushBack(ttRange);
                aPackedChars.PushBack(CharData);

                uint32_t iValidGlyths = 0;
                for(uint32_t k = Range.iBegin; k <= Range.iEnd; k++) {
                    iUsedGlythsAmount += (uint32_t)( stbtt_FindGlyphIndex(&FontInfo, k) != 0 );
                }
            }
        }

        // Find an approximate minimal bitmap size needed to fit this font
        const float fV = sqrt((float)(iUsedGlythsAmount)) * iHeight * 0.75f;
        const uint32_t iBitmapSide = math::CeilToPowerOf2((uint32_t)fV);

        uint8_t* pBitmap = Malloc<uint8_t>(iBitmapSide*iBitmapSide);

        stbtt_pack_context PackContext;
        PackContext.skip_missing = 1;

        stbtt_PackBegin(&PackContext, pBitmap, iBitmapSide, iBitmapSide, 0, 1, NULL);
            stbtt_PackFontRanges(&PackContext, aFileContent.Array(), 0, aPackRanges.Array(), aPackRanges.Length());
        stbtt_PackEnd(&PackContext);

        #ifdef CBPP_DEBUG
        char sbuff[64];
        snprintf(sbuff, 64, "%s.tga", sName);
        SOIL_save_image(sbuff, SOIL_SAVE_TYPE_TGA, iBitmapSide, iBitmapSide, 1, pBitmap);
        #endif

        GLuint hTexture = cbvs::CreateTexture(pBitmap, iBitmapSide, iBitmapSide, 1);
        Free(pBitmap);

        Font& Out = g_aFonts.At( g_aFonts.PushEmpty() );

        return true;
    }
}
