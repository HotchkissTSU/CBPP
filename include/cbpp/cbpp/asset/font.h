#ifndef CBPP_ASSET_FONT_H
#define CBPP_ASSET_FONT_H

#include "glad.h"

#include "cbpp/ttype/list.h"

namespace cbpp {
    struct GlythInfo {
        float_t fX, fY;       //Drawing origin
        float_t fWidth;       //Kinda self-explanatory
        float_t fBearY;       //Y bearing
        float_t fMaxBBoxY;    //bbox_ymax
    };

    class Font {
        friend bool LoadFont(const char*, bool);
            
        private:
            List<GlythInfo> m_aGlyths;
            GLuint m_hTexture;
            const char* m_sName;
    };

    bool LoadFont(const char* sPath, bool bAppendEx = true);
}

#endif
