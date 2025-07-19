#ifndef CBVS_BUFFER_H
#define CBVS_BUFFER_H

#include "glad/glad.h"
#include "cbvs/error_check.h"

#include <stddef.h>
#include <stdint.h>

namespace cbvs {
    template <typename vertex_t> class IVertexBuffer {
        public:
            virtual void Init(const vertex_t* pMemory, GLuint iCount, GLenum iMode) = 0;

            void Use() const {
                glBindVertexArray(m_hVAO);
            }

            void PushVertexData(const vertex_t* pData, GLuint iCount) const {
                glBindVertexArray(m_hVAO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_t) * iCount, (const void*)(pData));
            }

            GLuint VAO() const {
                return m_hVAO;
            }

            GLuint VBO() const {
                return m_hVBO;
            }

            virtual ~IVertexBuffer() {
                glDeleteBuffers(1, &m_hVBO);
                glDeleteVertexArrays(1, &m_hVAO);
            }

        protected:
            GLuint m_hVAO, m_hVBO;
    };

    struct SpriteVertex {
        GLfloat X0, Y0;
        GLfloat X1, Y1;
        GLfloat U0, V0;
        GLfloat U1, V1;
        GLfloat Angle;
        GLfloat Depth;
    };

    class SpriteVertexBuffer : public IVertexBuffer <SpriteVertex> {
        public:
            virtual void Init(const SpriteVertex* pMemory, GLuint iCount, GLenum iMode) {
                glGenVertexArrays(1, &m_hVAO);
                glGenBuffers(1, &m_hVBO);

                glBindVertexArray(m_hVAO);
                    glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
                    glBufferData(GL_ARRAY_BUFFER, iCount * sizeof(SpriteVertex), (const void*) pMemory, iMode);

                    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)(0));
                    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)( sizeof(GLfloat)*2 ));
                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)( sizeof(GLfloat)*4 ));
                    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)( sizeof(GLfloat)*6 ));
                    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)( sizeof(GLfloat)*8 ));
                    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (GLvoid*)( sizeof(GLfloat)*9 ));
            }
    };
}

#endif
