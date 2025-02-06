#ifndef CBDRAW_FONT_H
#define CBDRAW_FONT_H

#include <stdint.h>
#include <stddef.h>

#include "cbpp/vec2.h"
#include "cbvs/shader.h"
#include "cbpp/color.h"

namespace cbvs {
    extern const uint64_t* g_aFontDefault;
}

namespace ddraw {
    //This uses regular floats as OpenGL uses them too.
    //Primitive glyths are all square and have same size.
    struct GlythPrimitive {
        float fX,fY;
        float fSize;
        int32_t iChar;
    };

    extern GlythPrimitive* g_aPGlythBuffer;
    extern size_t g_iPGlythBufferLength;

    extern cbvs::Shader *g_pPTextVTX, *g_pPTextFRAG;
    extern cbvs::Pipe *g_pPTextPipe;

    void UpdatePGlythBuffer(size_t iBufferSize);

    void TextSimple(cbpp::Vec2 vTextPos, float_t fTextSize, const char* sText);
}

#endif
