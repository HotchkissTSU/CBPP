#include "cbvs/draw.h"
#include "cbvs/error_check.h"

namespace ddraw {
    GLuint vao, vbo;
    float buffer[CBPP_DDRAW_BUFFER_SIZE];
    float scale = 1.0f;
    cbpp::NormColor color = { 0.0f, 0.0f, 0.0f, 1.0f };
    bool is_init = false;

    bool Init() {

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
            
        glBindVertexArray(0);

        int glchk = glCheck();

        return (glchk == 0);
    }

    void SetColor(cbpp::Color clr) {
        color = clr.Normalized();

        //GLuint shid = ddraw_shprog->GetObjectID();
    }

    void Line(cbpp::Vec2 p1, cbpp::Vec2 p2, float width) {
        glLineWidth(width);
        glCheck();

        float buff[] = {
            p1.x, p1.y,
            p2.x, p2.y
        };

        //ddraw_shprog->Use();

        glBindVertexArray(vao);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(buff), buff);
            glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        glCheck();
    }
}
