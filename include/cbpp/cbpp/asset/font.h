#ifndef CBPP_ASSET_FONT_H
#define CBPP_ASSET_FONT_H

#include "cbpp/ttype/list.h"
#include "glad/glad.h"

#include <unordered_map>

#define CBPP_FONT_SDF_SIZE 64

namespace cbpp {
    struct GlythInfo {
        float_t fWidth;       //Kinda self-explanatory
        float_t fBearY;       //Y bearing
        float_t fMaxBBoxY;    //bbox_ymax

        GLuint hSDF;
    };

    class Font {
        friend bool LoadFont(const char*, const char*, bool);

        private:
            std::unordered_map<const char*, GlythInfo> m_mData;
    };

    bool LoadFont(const char* sName, const char* sAlias, bool bUseSDF = false);
}

#endif
