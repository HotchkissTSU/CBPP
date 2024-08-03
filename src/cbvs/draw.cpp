#include "cbvs/draw.h"

#include "cbpp_gl.h"
#include "cbpp/error.h"
#include "cbvs/error_check.h"

#include <stdio.h>
#include <cstdint>
#include <cmath>

using cbpp::Vec2;
using cbpp::Color;

namespace cbdraw {
	int width = 0, height = 0;
	float width_inv = 0.0f, height_inv = 0.0f, screen_ratio = 1.0f, cam_scale = 0.05f;
	
	cbpp::NormColor main_color;
	
	GLuint def_vao, def_vbo;
	
	Shader draw_shader_program, draw_circle_shader_program;
	
	void Init() {
		static float default_buffer_data[CBDRAW_DEFAULT_BUFFER_SIZE];
		
		try {
			draw_shader_program.Load(CBDRAW_DRAW_VTX_SHADER, CBDRAW_DRAW_FRAG_SHADER, NULL);
		} catch (std::runtime_error& err) {
			printf("%s - %s\n", cbpp::GetErrorName(), cbpp::GetErrorInfo());
			return;
		}
		
		try {
			draw_circle_shader_program.Load(CBDRAW_DRAW_VTX_SHADER, CBDRAW_DRAW_FRAG_SHADER, CBDRAW_DRAW_CIRCLE_SHADER);
		} catch (std::runtime_error& err) {
			printf("%s - %s\n", cbpp::GetErrorName(), cbpp::GetErrorInfo());
			return;
		}
		
		glGenBuffers(1, &def_vbo);
		glGenVertexArrays(1, &def_vao);
		
		glBindVertexArray(def_vao);
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, def_vbo);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glBufferData(GL_ARRAY_BUFFER, sizeof(default_buffer_data), default_buffer_data, GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
	}
	
	void SetWH(int w, int h) {
		width = w;
		height = h;
		
		width_inv = 1.0f / (float)width;
		height_inv = 1.0f / (float)height;
		
		screen_ratio = (float)width / (float)height;
	}
	
	void SetColor(Color clr) {
		main_color = clr.Normalized();
	}
	
	void SetScale(float scale) {
		cam_scale = scale;
	}
	
	GLint cb_GetUniformLocation(Shader& shader, const char* unif_name) {
		GLint out = glGetUniformLocation(shader.GetProgramID(), unif_name);
		if(out < 0) {
			printf("Failed to get uniform '%s'\n", unif_name);
			RENDER_CHECK_ERROR();
		}
		return out;
	}
	
	void ApplyColor(Shader& shader) {
		GLint color_unif_id = cb_GetUniformLocation(shader, "cbdraw_color");
		glUniform4f(color_unif_id, main_color.r, main_color.g, main_color.b, main_color.a);
	}
	
	void ApplyScale(Shader& shader) {
		GLint scale_unif_id = cb_GetUniformLocation(shader, "cbdraw_cam_scale");
		glUniform1f(scale_unif_id, cam_scale);
	}
	
	void Line(Vec2 p1, Vec2 p2, float width) {
		float vtx_arr[4];
		vtx_arr[0] = p1.x; vtx_arr[1] = p1.y;
		vtx_arr[2] = p2.x; vtx_arr[3] = p2.y;
		
		draw_shader_program.Use();
		
		ApplyColor(draw_shader_program);
		ApplyScale(draw_shader_program);
		
		glLineWidth(width);
		
		glBindVertexArray(def_vao);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_arr), vtx_arr, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}
	
	void CircleOutline(Vec2 pos, float radius, float width) {		
		float vtx_arr[2] = { pos.x, pos.y };
		draw_circle_shader_program.Use();
		
		ApplyColor(draw_circle_shader_program);
		ApplyScale(draw_circle_shader_program);
		
		glLineWidth(width);
		
		GLint u_radius_id, u_ratio_id, u_fidelity_id;
		
		u_radius_id = cb_GetUniformLocation(draw_circle_shader_program, "cbdraw_circle_radius");
		u_fidelity_id = cb_GetUniformLocation(draw_circle_shader_program, "cbdraw_circle_fidelity");
		u_ratio_id = cb_GetUniformLocation(draw_circle_shader_program, "cbdraw_screen_ratio");
		
		glUniform1f(u_radius_id, radius * screen_ratio);
		glUniform1f(u_ratio_id, screen_ratio);
		glUniform1i(u_fidelity_id, CBDRAW_CIRCLE_FIDELITY);
		
		glBindVertexArray(def_vao);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_arr), vtx_arr, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}
	
	void TriangleOutline(cbpp::Vec2 a, cbpp::Vec2 b, cbpp::Vec2 c, float width) {
		float vtx_arr[6];
		
		vtx_arr[0] = a.x; vtx_arr[1] = a.y;
		vtx_arr[2] = b.x; vtx_arr[3] = b.y;
		vtx_arr[4] = c.x; vtx_arr[5] = c.y;
		
		draw_shader_program.Use();
		
		ApplyColor(draw_shader_program);
		ApplyScale(draw_shader_program);
		
		glBindVertexArray(def_vao);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_arr), vtx_arr, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_LINE_LOOP, 0, 3);
		glBindVertexArray(0);
	}
	
	void MeshOutline(cbpp::Mesh& msh, float width) {
		uint32_t mesh_len = msh.Size()*2;
		const float* vtx_arr = msh.GetArrayf();
		
		draw_shader_program.Use();
		
		ApplyColor(draw_shader_program);
		ApplyScale(draw_shader_program);
		
		glBindVertexArray(def_vao);
			glBufferData(GL_ARRAY_BUFFER, mesh_len*sizeof(float), vtx_arr, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_LINE_LOOP, 0, mesh_len);
		glBindVertexArray(0);
	}
}