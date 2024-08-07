#ifndef CBVS_DRAW_H
#define CBVS_DRAW_H

#include "cbpp/vec2.h"
#include "cbpp/mesh.h"
#include "cbpp/color.h"

#include "cbpp_gl.h"
#include "cbvs/shader.h"

#include <cstdint>

#define CBDRAW_DEFAULT_BUFFER_SIZE 64
#define CBDRAW_DRAW_VTX_SHADER "assets/shaders/cbdraw_default.vertex"
#define CBDRAW_DRAW_FRAG_SHADER "assets/shaders/cbdraw_default.fragment"

#define CBDRAW_DRAW_CIRCLE_SHADER "assets/shaders/cbdraw_circle.geometry"
#define CBDRAW_CIRCLE_FIDELITY 32

namespace cbdraw {
	extern GLuint def_vao, def_vbo;
	
	extern int width, height;
	extern float width_inv, height_inv, screen_ratio, cam_scale;
	
	extern cbpp::NormColor main_color;
	
	extern Shader draw_shader_program, draw_circle_shader_program;
	
	void Init();
	
	void SetWH(int width, int height);
	
	void SetColor(cbpp::Color clr);
	void SetScale(float scale);
	
	void Line(cbpp::Vec2 pos1, cbpp::Vec2 pos2, float width = 1.0f);
	void CircleOutline(cbpp::Vec2 pos, float radius, float width = 1.0f);
	void TriangleOutline(cbpp::Vec2 a, cbpp::Vec2 b, cbpp::Vec2 c, float width = 1.0f);
	void MeshOutline(cbpp::Mesh& msh, float width = 1.0f);
}

#endif