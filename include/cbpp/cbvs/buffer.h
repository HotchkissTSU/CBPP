#ifndef CBVS_BUFFER_H
#define CBVS_BUFFER_H

#include "glad/glad.h"

namespace cbvs {
    class IBuffer {
        public:
            virtual GLuint VertexBuffer() const noexcept = 0;
            virtual GLuint VertexArray() const noexcept = 0;
        private:
            GLuint m_hVAO, m_hVBO;
    };
}

#endif
