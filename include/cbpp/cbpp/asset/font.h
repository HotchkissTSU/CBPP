#ifndef CBPP_ASSET_FONT_H
#define CBPP_ASSET_FONT_H

#include "cbpp/ttype/list.h"
#include "glad/glad.h"

// Resolution of the single glyth`s SDF bitmap
#define CBPP_FONT_SDF_SIZE 64

namespace cbpp {

    /*
        We use these ranges to save on the actual memory usage, as loading and mapping
        every glyth in the font (this can potentially hit 65536 entries!) would be extremely dumb.

        Note that a particular font might lack some of these glyth ranges, so nothing will
        be loaded.
    */

    enum EFontLoadMask : uint32_t {
        LOAD_ASCII =        1 << 0,         // Any character between 32 and 127
        LOAD_CYRILLIC =     1 << 1,         // 0400 - 04FF
        LOAD_LATIN_AUX =    1 << 2,         // 00A0 - 00FF, some thingies for other europian languages
        LOAD_ALL =          1 << 3          // Scorch the earth, boil the seas
    };

    // This enum must correspond to the one above
    enum EUnicodeRangeIndex {
        UTF_RANGE_ASCII,
        UTF_RANGE_CYRILLIC,
        UTF_RANGE_LATIN_AUX,
        UTF_RANGE_FULL,

        UTF_RANGES_AMOUNT //This entry must be the last one
    };

    /*
        C++ has this moronic shit that you cant use enum type as a simple integer (as you can in the regular C).
        Performing a manual cast to uint32_t every time you want a bitmask is ugly and retarded, so this operator is implemented.
    */
    inline EFontLoadMask operator | (EFontLoadMask iLeft, EFontLoadMask iRight) {
        return (EFontLoadMask)( (uint32_t)(iLeft) | (uint32_t)(iRight) );
    }

    struct UnicodeRange {
        uint32_t iBegin, iEnd;
    };

    struct GlythInfo {
        float_t fWidth;       //Kinda self-explanatory
        float_t fBearY;       //Y bearing
        float_t fMaxBBoxY;    //bbox_ymax
    };

    struct GlythRange {
        uint32_t iOffset; // Codepoint offset of the first glyth in the range, since our array is indexed from zero
        List<GlythInfo> aData;
    };

    constexpr UnicodeRange GetUnicodeRange( EUnicodeRangeIndex iIndex );
    constexpr EUnicodeRangeIndex FindUnicodeRangeIndex(uint32_t iCodepoint);
    constexpr EUnicodeRangeIndex UnicodeRangeByLoadMask(EFontLoadMask iMask);

    class Font {
        friend bool LoadFont(const char*, const char*, bool);

        private:
            GlythRange m_aRanges[UTF_RANGES_AMOUNT];
            uint32_t m_iLoadedRanges = 0;
            bool m_bSDF = false;
    };

    uint32_t GetFontID(const char* sAlias);
    Font& GetFont(uint32_t iFontID);

    bool LoadFontBitmap(const char* sName, const char* sAlias, int iHeight, EFontLoadMask iMask);
    bool LoadFontSDF(const char* sname, const char* sAlias, EFontLoadMask iMask);
}

#endif
