#ifndef CBVS_BUFFER_H
#define CBVS_BUFFER_H

#include "glad/glad.h"
#include "cbvs/error_check.h"

#include <stddef.h>
#include <stdint.h>

namespace cbvs {

    //  The thingie to represent a single vertex attribute.
    struct VtxAttribData {
        GLuint Size;
        GLuint ByteSize;
        GLenum Type;
    };

    template <typename vertex_t> class VertexBuffer {
        public:
            /*
                The summary size of all providen attributes must be equal to sizeof(vertex_t)
            */
            void Init(const vertex_t* pMemory, size_t iLength, GLenum iMode, const VtxAttribData* pAttribInfo = NULL, uint8_t iAttribAmount = 0) {
                printf("buffer init\n");

                glGenVertexArrays(1, &m_hVAO);
                glGenBuffers(1, &m_hVBO);

                size_t iOffset = 0;

                glBindVertexArray(m_hVAO);
                    glBindBuffer(GL_ARRAY_BUFFER, m_hVBO);
                    glBufferData(GL_ARRAY_BUFFER, iLength * sizeof(vertex_t), (const void*) pMemory, iMode);

                    for(size_t i = 0; i < iAttribAmount; i++) {
                        glEnableVertexAttribArray(i);

                        const VtxAttribData Data = pAttribInfo[i];
                        glVertexAttribPointer(i, Data.Size, Data.Type, GL_FALSE, sizeof(vertex_t), (GLvoid*)(iOffset + i*sizeof(vertex_t)));

                        //  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)0);
                        //  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)(sizeof(GLfloat)*2));

                        iOffset += Data.ByteSize;
                    }
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
