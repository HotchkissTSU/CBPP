#include "cbvs/render.h"
#include "cbpp/error.h"

namespace cbvs {
    cbpp::float_t g_fScreenRatio = 1.0f;

    IRenderable::IRenderable(const char* sPipeName) {
        m_pPipe = cbvs::GetPipe(sPipeName);

        if(m_pPipe == NULL) {
            CbThrowErrorf("Unable to find the '%s' rendering pipeline", sPipeName);
        }
    }

    void IRenderable::CallPipe() const noexcept {
        if(m_pPipe != NULL) { m_pPipe->Use(); }
    }
}

namespace cbvs {
    GLuint SpriteBatcher::GenerateTexture() const {
        GLuint hOut;
        glGenTextures(1, &hOut);

        glBindTexture(GL_TEXTURE_2D, hOut);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CBVS_SPRITEBATCH_SIZE, CBVS_SPRITEBATCH_SIZE,
                        0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)(m_aImage));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return hOut;
    }

    void SpriteBatcher::MarkRegionAs(int iX, int iY, int iW, int iH, bool bMark) {
        for(int Y0 = iY; Y0 < iY + iH; Y0++) {
            for(int X0 = iX; X0 < iX + iW; X0++) {
                m_aMappingInfo[Y0][X0] = bMark;
            }
        }
    }

    bool SpriteBatcher::ScanRegion(int iX, int iY, int iW, int iH) const {
        const int iLen = CBVS_SPRITEBATCH_SIZE / CBVS_SPRITEMAPPER_MINSIZE;
        for(int X0 = iX; X0 < iX + iW; X0++) {
            for(int Y0 = iY; Y0 < iY + iH; Y0++) {
                if(X0 >= iLen || Y0 >= iLen) {
                    return false;
                }

                if(m_aMappingInfo[iY][iX] != 0) {
                    return false;
                }
            }
        }

        return true;
    }

    bool SpriteBatcher::LocateFreeRegion(int iW, int iH, int& iX, int& iY) const {
        const int iLen = CBVS_SPRITEBATCH_SIZE / CBVS_SPRITEMAPPER_MINSIZE;
        for(int X0 = 0; X0 < iLen; X0++) {
            for(int Y0 = 0; Y0 < iLen; Y0++) {
                if( ScanRegion(X0, Y0, iW, iH) ) {
                    iX = X0; iY = Y0;
                    return true;
                }else {
                    continue;
                }
            }
        }

        return false;
    }

    void SpriteBatcher::PrintMappingInfo() const {
        const int iLen = CBVS_SPRITEBATCH_SIZE / CBVS_SPRITEMAPPER_MINSIZE;

        printf("   ");
        for(int iX = 0; iX < iLen; iX++) {
            printf("%.2d ", iX);
        }
        printf("\n");

        char chr;
        for(int iX = 0; iX < iLen; iX++) {
            printf("%.2d ", iX);
            for(int iY = 0; iY < iLen; iY++) {
                if(m_aMappingInfo[iX][iY] != 0) {
                    chr = '#';
                }else{
                    chr = '_';
                }
                printf("%c  ", chr);
            }
            printf("\n");
        }
    }
}
