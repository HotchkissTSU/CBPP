#ifndef CBVS_BUFFER_H
#define CBVS_BUFFER_H

#include "glad/glad.h"
#include "cbvs/error_check.h"

#include <stddef.h>
#include <stdint.h>

/*
    About vertex attributes: 
    0 - 2 GL_FLOATs - Vertex position
    1 - 2 GL_FLOATs - Vertex UV coords
*/

namespace cbvs {
    template <typename vertex_t> class VertexBuffer {
        public:
            /*
                The summary size of all providen attributes must be equal to sizeof(vertex_t)
            */
            void Init(const vertex_t* pMemory, size_t iLength, GLenum iMode) {
                glGenVertexArrays(1, &m_hVAO);
                glGenBuffers(1, &m_hVBO);

                glBindVertexArray(m_hVAO);
                    glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
                    glBufferData(GL_ARRAY_BUFFER, iLength * sizeof(vertex_t), (const void*) pMemory, iMode);

                    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)(0));
                    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)( sizeof(GLfloat)*2 ));
            }

            VertexBuffer() = default;
            VertexBuffer(const VertexBuffer& Other) = delete;

            void Use() const {
                glBindVertexArray(m_hVAO);
            }

            void PushVertexData(vertex_t* aVertices, size_t iVtxAmount) {
                glBindVertexArray(m_hVAO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_t) * iVtxAmount, (const void*)(aVertices));
            }

            GLuint VAO() const { return m_hVAO; }
            GLuint VBO() const { return m_hVBO; }

            ~VertexBuffer() {
                glDeleteVertexArrays(1, &m_hVAO);
                glDeleteBuffers(1, &m_hVBO);
            }

        private:
            GLuint m_hVAO, m_hVBO;
    };
}

#endif
