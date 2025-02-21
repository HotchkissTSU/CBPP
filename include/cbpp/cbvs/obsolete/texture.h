#ifndef CBVS_GLTEXTURE_H
#define CBVS_GLTEXTURE_H

#include "glad/glad.h"
#include "cbpp/asset/image.h"
#include "cbvs/error_check.h"
#include <cstdint>

namespace cbvs {
    class Texture {
        public:
            Texture() = default;
            Texture(const cbpp::Image& img, GLenum wrap_x = GL_REPEAT, GLenum wrap_y = GL_REPEAT, GLenum format = GL_RGBA, GLenum filter = GL_LINEAR);
            Texture(const cbpp::Image& img, GLenum wrap = GL_REPEAT, GLenum format = GL_RGBA, GLenum filter = GL_LINEAR);

            bool Create(const cbpp::Image& img, GLenum wrap_x = GL_REPEAT, GLenum wrap_y = GL_REPEAT, GLenum format = GL_RGBA, GLenum filter = GL_LINEAR);

        private:
            GLuint objid;
    };
}

#endif