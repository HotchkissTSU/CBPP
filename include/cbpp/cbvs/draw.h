#ifndef CBDRAW_DDRAW_H
#define CBDRAW_DDRAW_H

//debug-draw

#include <cstdint>

#include "glad/glad.h"

#include "cbpp/vec2.h"
#include "cbpp/color.h"
#include "cbvs/shader.h"

#define CBPP_DDRAW_BUFFER_SIZE 512 //calculated in floats

#define CBVS_DDRAW_CIRCLE_FIDELITY 32

namespace ddraw {
    extern GLuint vao, vbo;
    extern float buffer[];
    extern float scale;
    extern cbpp::NormColor color;
    extern bool is_init;

    extern cbvs::Shader *ddraw_vtx, *ddraw_frag, *ddraw_geom_circle, *ddraw_geom_circlef;
    extern cbvs::Pipe *ddraw_pipe_def, *ddraw_pipe_circle, *ddraw_pipe_circlef;

    bool Init();
    void Cleanup();

    void SetColor(cbpp::Color clr);

    void Line(cbpp::Vec2 p1, cbpp::Vec2 p2, float_t width = 1.0f);

    void CircleOutline(cbpp::Vec2 p, float_t radius, float_t width = 1.0f);
    void Circle(cbpp::Vec2 p, float_t radius);

    void RectOutline(cbpp::Vec2 p1, cbpp::Vec2 p2, float_t width = 1.0f);
    void Rect(cbpp::Vec2 p1, cbpp::Vec2 p2);
}

namespace cbvs {
    extern cbpp::Vec2 MousePosition, ScreenSize;
    extern float_t ScreenRatio;

    cbpp::Vec2 GetNormalizedMousePos();
}

#endif