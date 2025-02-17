#include "cbvs/draw.h"
#include "cbpp/texture_default.h"

namespace ddraw {
    GLuint vao, vbo, default_texture;
    GLuint vao_tex, vbo_tex;

    float buffer[CBPP_DDRAW_BUFFER_SIZE];
    float scale = 1.0f;
    cbpp::NormColor color = { 0.0f, 0.0f, 0.0f, 1.0f };
    bool is_init = false;

    cbvs::Shader *ddraw_vtx = nullptr, *ddraw_frag = nullptr, *ddraw_geom_circle = nullptr;
    cbvs::Shader *ddraw_geom_circlef = nullptr, *ddraw_vtx_tex = nullptr, *ddraw_frag_tex = nullptr;

    cbvs::Pipe *ddraw_pipe_def = nullptr, *ddraw_pipe_circle = nullptr;
    cbvs::Pipe *ddraw_pipe_circlef = nullptr, *ddraw_pipe_tex = nullptr;

    bool Init() {
        ddraw_vtx = cbvs::CreateShader(GL_VERTEX_SHADER, cbvs::default_vtx);
        ddraw_frag = cbvs::CreateShader(GL_FRAGMENT_SHADER, cbvs::default_frag);

        ddraw_pipe_def = cbvs::CreatePipe(ddraw_vtx, ddraw_frag);
        //---
        const char* sbuffer = cbvs::LoadShader("ddraw_circle", GL_GEOMETRY_SHADER);
        if(sbuffer == NULL) {
            return false;
        }
        ddraw_geom_circle = cbvs::CreateShader(GL_GEOMETRY_SHADER, sbuffer);
        delete[] sbuffer;
        //---
        sbuffer = cbvs::LoadShader("ddraw_circlef", GL_GEOMETRY_SHADER);
        if(sbuffer == NULL) {
            return false;
        }
        ddraw_geom_circlef = cbvs::CreateShader(GL_GEOMETRY_SHADER, sbuffer);
        delete[] sbuffer;
        //---
        sbuffer = cbvs::LoadShader("ddraw_tex", GL_VERTEX_SHADER);
        if(sbuffer == NULL) {
            return false;
        }
        ddraw_vtx_tex = cbvs::CreateShader(GL_VERTEX_SHADER, sbuffer);
        delete[] sbuffer;
        //---
        sbuffer = cbvs::LoadShader("ddraw_tex", GL_FRAGMENT_SHADER);
        if(sbuffer == NULL) {
            return false;
        }
        ddraw_frag_tex = cbvs::CreateShader(GL_FRAGMENT_SHADER, sbuffer);
        delete[] sbuffer;

        ddraw_pipe_circle = cbvs::CreatePipe(ddraw_vtx, ddraw_frag, ddraw_geom_circle);
        ddraw_pipe_circlef = cbvs::CreatePipe(ddraw_vtx, ddraw_frag, ddraw_geom_circlef);
        ddraw_pipe_tex = cbvs::CreatePipe(ddraw_vtx_tex, ddraw_frag_tex);

        glCheck();

        glGenTextures(1, &default_texture);
        glBindTexture(GL_TEXTURE_2D, default_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<const unsigned char*>(cbpp::DefaultTexture));
        glBindTexture(GL_TEXTURE_2D, 0);

        glCheck();

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
            
        glBindVertexArray(0);

        glGenVertexArrays(1, &vao_tex);

        glBindVertexArray(vao_tex);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)(sizeof(GLfloat)*2));

        glBindVertexArray(0);

        glCheck();

        int glchk = glCheck();

        is_init = (glchk == 0);
        return (glchk == 0);
    }

    void Cleanup() {
        if(!is_init) { return; }

        delete ddraw_vtx, ddraw_frag, ddraw_geom_circle, ddraw_geom_circlef;
        delete ddraw_pipe_circle, ddraw_pipe_circlef, ddraw_pipe_def;
    }

    void SetColor(cbpp::Color clr) {
        color = clr.Normalized();
    }

    void Line(cbpp::Vec2 p1, cbpp::Vec2 p2, float_t width) {
        glLineWidth((GLfloat)width);

        GLfloat buff[] = {
            (GLfloat)p1.x, (GLfloat)p1.y,
            (GLfloat)p2.x, (GLfloat)p2.y
        };

        ddraw_pipe_def->Use();
        ddraw_pipe_def->PushUniform("cbpp_COLOR", color.r, color.g, color.b, color.a);
        //ddraw_pipe_def->PushUniform("cbpp_RATIO", cbvs::ScreenRatio);

        glBindVertexArray(vao);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        glCheck();
    }

    void CircleOutline(cbpp::Vec2 p, float_t radius, float_t width) {
        glLineWidth((GLfloat)width);

        GLfloat buff[] = { (GLfloat)p.x, (GLfloat)p.y };

        ddraw_pipe_circle->Use();
        ddraw_pipe_circle->PushUniform("cbpp_COLOR", color.r, color.g, color.b, color.a);
        ddraw_pipe_circle->PushUniform("cbpp_RATIO", cbvs::ScreenRatio);
        ddraw_pipe_circle->PushUniform("cbpp_CIRCLE_FIDELITY", CBVS_DDRAW_CIRCLE_FIDELITY);
        ddraw_pipe_circle->PushUniform("cbpp_CIRCLE_RADIUS", radius);

        glBindVertexArray(vao);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        glCheck();
    }

    void Circle(cbpp::Vec2 p, float_t radius) {
        GLfloat buff[] = { (GLfloat)p.x, (GLfloat)p.y };

        ddraw_pipe_circlef->Use();
        ddraw_pipe_circlef->PushUniform("cbpp_COLOR", color.r, color.g, color.b, color.a);
        ddraw_pipe_circlef->PushUniform("cbpp_RATIO", cbvs::ScreenRatio);
        ddraw_pipe_circlef->PushUniform("cbpp_CIRCLE_FIDELITY", CBVS_DDRAW_CIRCLE_FIDELITY);
        ddraw_pipe_circlef->PushUniform("cbpp_CIRCLE_RADIUS", radius);

        glBindVertexArray(vao);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        glCheck();
    }

    void RectOutline(cbpp::Vec2 p1, cbpp::Vec2 p2, float_t width) {
        glLineWidth((GLfloat)width);

        GLfloat buff[] = {
            (GLfloat)p1.x, (GLfloat)p1.y,
            (GLfloat)p2.x, (GLfloat)p1.y,
            (GLfloat)p2.x, (GLfloat)p2.y,
            (GLfloat)p1.x, (GLfloat)p2.y
        };

        ddraw_pipe_def->Use();
        ddraw_pipe_def->PushUniform("cbpp_COLOR", color.r, color.g, color.b, color.a);

        glBindVertexArray(vao);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);

        glCheck();
    }

    void Rect(cbpp::Vec2 p1, cbpp::Vec2 p2) {
        GLfloat buff[] = {
            (GLfloat)p1.x, (GLfloat)p1.y,
            (GLfloat)p2.x, (GLfloat)p1.y,
            (GLfloat)p1.x, (GLfloat)p2.y,

            (GLfloat)p2.x, (GLfloat)p2.y,
            (GLfloat)p1.x, (GLfloat)p2.y,
            (GLfloat)p2.x, (GLfloat)p1.y
        };

        ddraw_pipe_def->Use();
        ddraw_pipe_def->PushUniform("cbpp_COLOR", color.r, color.g, color.b, color.a);

        glBindVertexArray(vao);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        glCheck();
    }

    void Image(cbpp::Vec2 pos, cbpp::Vec2 scale, cbpp::Image& img) {
        cbpp::Vec2 p1 = pos, p2 = pos + scale;

        GLfloat buff[] = {
            p1.x, p1.y, 0.0f, 0.0f,
            p2.x, p1.y, 1.0f, 0.0f,
            p1.x, p2.y, 0.0f, 1.0f,

            p2.x, p2.y, 1.0f, 1.0f,
            p1.x, p2.y, 0.0f, 1.0f,
            p2.x, p1.y, 1.0f, 0.0f
        };

        glCheck();

        ddraw_pipe_tex->Use();
        ddraw_pipe_tex->PushUniform("cbpp_RATIO", cbvs::ScreenRatio);

        glBindTexture(GL_TEXTURE_2D, img.objid);
        glBindVertexArray(vao_tex);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glCheck();
    }
}

namespace cbvs {
    cbpp::Vec2 MousePosition(0), ScreenSize(0);
    float_t ScreenRatio = (float_t)1.0f;

    cbpp::Vec2 GetNormalizedMousePos() {
        return (cbvs::MousePosition / cbvs::ScreenSize)*cbpp::Vec2(2,-2) + cbpp::Vec2(-1, 1);
    }
}
