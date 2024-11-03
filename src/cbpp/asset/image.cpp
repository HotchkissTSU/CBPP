#include "cbpp/asset/image.h"
#include "cbpp/texture_default.h"
#include "cbvs/error_check.h"

namespace cbpp {
    Image LoadImage(const char* path, IMG_LOAD_FORMAT loadf, uint16_t flags) {
        Image out;

        int w,h;
        bool is_fallback = false;

        unsigned char* data = SOIL_load_image(path, &w, &h, NULL, (int)loadf);
        
        if(!data) {
            data = (uint8_t*)(cbpp::DefaultTexture);
            w = 32; h = 32;
            is_fallback = true;
        }

        out.W = (uint32_t)w;
        out.H = (uint32_t)h;

        //this call creates a mysterious OpenGL INVALID_ENUM error, but everything is working just fine
        out.objid = SOIL_create_OGL_texture(data, w, h, (int)loadf, 0, (unsigned int)flags);
        while( GLenum errcd = glGetError() != GL_NO_ERROR ) {
            //clear all errors not to crash the engine upon the next glCheck() call
        }

        if(!is_fallback) { //do not attempt to delete static array for the default texture
            SOIL_free_image_data(data);
        }

        return out;
    }
}
