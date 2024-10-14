#ifndef CBDRAW_DDRAW_H
#define CBDRAW_DDRAW_H

//debug-draw

#include <cstdint>

#include "glad/glad.h"

#include "cbpp/vec2.h"
#include "cbpp/color.h"
#include "cbvs/shader.h"

#define CBPP_DDRAW_BUFFER_SIZE 512 //calculated in floats

namespace ddraw {
    extern GLuint vao, vbo;
    extern float buffer[];
    extern float scale;
    extern cbpp::NormColor color;
    extern bool is_init;

    bool Init();

    void SetColor(cbpp::Color clr);

    void Line(cbpp::Vec2 p1, cbpp::Vec2 p2, float width = 1.0f);
    void CircleOutline(cbpp::Vec2 p, float radius, float width = 1.0f);
    void RectOutline(cbpp::Vec2 p1, cbpp::Vec2 p2, float width = 1.0f);
}

#endif