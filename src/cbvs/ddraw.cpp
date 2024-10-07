#include "cbvs/draw.h"
#include "cbvs/error_check.h"
#include "cbvs/shader.h"

namespace ddraw {
    GLuint vao, vbo;
    float buffer[CBPP_DDRAW_BUFFER_SIZE];
    float scale = 1.0f;
    cbpp::NormColor color = { 0.0f, 0.0f, 0.0f, 1.0f };
    bool is_init = false;

    bool Init() {
        cbvs::Shader* ddraw_vtx_shader = new cbvs::Shader(GL_VERTEX_SHADER, "assets/shaders/default.vertex");
        cbvs::Shader* ddraw_frag_shader = new cbvs::Shader(GL_FRAGMENT_SHADER, "assets/shaders/default.fragment");
        cbvs::ShaderProgram* ddraw_def_prog = new cbvs::ShaderProgram(ddraw_vtx_shader, ddraw_frag_shader, NULL);
        
        GLenum errcd = glCheck();
        if(errcd != GL_NO_ERROR) { return errcd; }

        cbvs::RegisterShader(ddraw_vtx_shader, "ddraw_default.vertex");
        cbvs::RegisterShader(ddraw_frag_shader, "ddraw_default.fragment");
        cbvs::RegisterProgram(ddraw_def_prog, "ddraw_default");

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glCheck();
        glEnableVertexAttribArray(0);
        glCheck();

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glCheck();
            glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
            glCheck();
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
            glCheck();
            
        glBindVertexArray(0);
        glCheck();
        //glBindBuffer(GL_VERTEX_ARRAY, 0);

        printf("\n\nVBO is buffer: %d\n\n\n", glIsBuffer(vbo));

        return glCheck();
    }

    void SetColor(cbpp::Color clr) {
        color = clr.Normalized();
    }

    void Line(cbpp::Vec2 p1, cbpp::Vec2 p2, float width) {
        glLineWidth(width);
        glCheck();

        float buff[] = {
            p1.x, p1.y,
            p2.x, p2.y
        };

        cbvs::UseProgram("ddraw_default");

        glBindVertexArray(vao);
            //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            //glCheck();
            glDrawArrays(GL_LINES, 0, 2);
            glCheck();
        glBindVertexArray(0);

        glCheck();
    }
}