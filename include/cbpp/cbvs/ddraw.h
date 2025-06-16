#ifndef CBVS_DDRAW_H
#define CBVS_DDRAW_H

/*
    Direct-Draw, aka "you call it, you see it".
    A rendering subsystem intended for debugging and testing purposes, easily accessible
    from any point in the engine.
*/

#include "cbpp/vec2.h"
#include "cbvs/shader.h"
#include "cbpp/color.h"
#include "cbvs/texture.h"

#define DDRAW_MESHBUFFER_LENGTH 128
#define DDRAW_CIRCLE_FIDELITY 32

namespace ddraw {
    extern GLfloat g_aMeshBuffer[];
    extern cbvs::Pipe *g_hGeneric, *g_hCircle, *g_hCirclef, *g_hTexture;
    extern GLuint g_hVAO, g_hVBO;

    extern GLuint g_hDefaultTexture;
    extern GLuint g_hTextureVAO, g_hTextureVBO;
    extern GLuint g_hFontTexture;

    extern GLuint g_hTextVAO, g_hTextVBO;

    extern cbpp::NormColor g_cActiveColor;

    bool Init() noexcept;

    void SetColor(const cbpp::Color& cNewColor) noexcept;
    void ApplyColor(const cbvs::Pipe* hTarget) noexcept;

    void Line(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2, cbpp::float_t fWidth = 1.0f) noexcept;
    void RectOutline(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2, cbpp::float_t fWidth = 1.0f) noexcept;
    void Rect(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2) noexcept;

    void CircleOutline(cbpp::Vec2 vPos, cbpp::float_t fRadius, cbpp::float_t fWidth = 1.0f) noexcept;
    void Circle(cbpp::Vec2 vPos, cbpp::float_t fRadius) noexcept;

    void Text(cbpp::Vec2 vPos, const char* sSource, cbpp::float_t fScale = 1.0f) noexcept;

    void Texture(cbpp::Vec2 vPos, cbpp::Vec2 vScale, GLuint hImage, bool bKeepRatio = true) noexcept;

    void MeshOutline(const cbpp::Vec2* aSource, size_t iSourceLength, cbpp::Vec2 vOffset) noexcept;
}

#endif
