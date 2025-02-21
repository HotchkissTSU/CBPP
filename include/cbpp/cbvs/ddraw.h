#ifndef CBVS_DDRAW_H
#define CBVS_DDRAW_H

/*
    Direct-Draw system, aka "you call it, you see it".
    A rendering subsystem intended for debugging and testing purposes, easily accessible
    from any point in the engine.
*/

#include "cbpp/vec2.h"
#include "cbvs/shader.h"
#include "cbpp/color.h"

#define DDRAW_MESHBUFFER_LENGTH 128
#define DDRAW_CIRCLE_FIDELITY 32

namespace ddraw {
    extern GLfloat g_aMeshBuffer[];
    extern cbvs::Pipe *g_hGeneric, *g_hCircle, *g_hCirclef;
    extern GLuint g_hVAO, g_hVBO;

    extern GLuint g_hDefaultTexture, g_hDefaultFontTexture;
    extern GLuint g_hTextureVAO;

    extern cbpp::NormColor g_cActiveColor;

    void RegisterShaders() noexcept;
    bool Init() noexcept;

    void SetColor(const cbpp::Color& cNewColor) noexcept;
    void ApplyColor(const cbvs::Pipe* hTarget) noexcept;

    void Line(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2, cbpp::float_t fWidth = 1.0f) noexcept;
    void RectOutline(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2, cbpp::float_t fWidth = 1.0f) noexcept;
    void Rect(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2) noexcept;

    void CircleOutline(cbpp::Vec2 vPos, cbpp::float_t fRadius, cbpp::float_t fWidth = 1.0f) noexcept;
    void Circle(cbpp::Vec2 vPos, cbpp::float_t fRadius) noexcept;

    void Text(cbpp::Vec2 vPos, const char* sSource, cbpp::float_t fScale = 1.0f) noexcept;

    void MeshOutline(const cbpp::Vec2* aSource, size_t iSourceLength, cbpp::Vec2 vOffset) noexcept;
}

#endif
