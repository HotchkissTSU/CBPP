#include "cbvs/ddraw.h"

#include "cbpp/texture_default.h"

#include "cbvs/error_check.h"
#include "cbvs/render.h"

#include <assert.h>

#include "cbpp/bit.h"
#include "cbpp/error.h"

namespace ddraw {
    GLfloat g_aMeshBuffer[DDRAW_MESHBUFFER_LENGTH];
    cbvs::Pipe *g_hCircle = NULL, *g_hCirclef = NULL, *g_hGeneric = NULL, *g_hTexture = NULL;

    GLuint g_hVAO = 0, g_hVBO = 0;
    GLuint g_hDefaultTexture, g_hDefaultFontTexture, g_hTextureVAO, g_hTextureVBO, g_hTextVBO;

    GLuint g_hTextVAO = 0;

    GLuint g_hFontTexture;

    cbpp::NormColor g_cActiveColor;

    void __bitfield2array(uint8_t* pTarget, uint8_t iBits) noexcept {
        for(size_t i = 0; i < 8; i++) {
            pTarget[8-i-1] = cbpp::GetBit(iBits, i)*255;
        }
    }

    bool Init() noexcept {
        bool bOut = true;

        g_hGeneric = cbvs::GetPipe("ddraw_generic");
        bOut = bOut && g_hGeneric != NULL;
        
        g_hCircle = cbvs::GetPipe("ddraw_circle");
        bOut = bOut && g_hCircle  != NULL;

        g_hCirclef = cbvs::GetPipe("ddraw_circle_fill");
        bOut = bOut && g_hCirclef != NULL;

        g_hTexture = cbvs::GetPipe("ddraw_texture");
        bOut = bOut && g_hTexture != NULL;

        if(!bOut) {
            return false;
        }

        glGenVertexArrays(1, &g_hVAO);
        glGenBuffers(1, &g_hVBO);

        //Setup default buffer objects
        glBindVertexArray(g_hVAO);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, g_hVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_aMeshBuffer), g_aMeshBuffer, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBindVertexArray(0);

        bOut = bOut && glCheck() == GL_NO_ERROR;

        //Also setup them for texture rendering

        glGenVertexArrays(1, &g_hTextureVAO);
        glGenBuffers(1, &g_hTextureVBO);

        glBindVertexArray(g_hTextureVAO);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, g_hTextureVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_aMeshBuffer), g_aMeshBuffer, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid*)(sizeof(GLfloat)*2));
        glBindVertexArray(0);

        //assert(glIsBuffer(hTextureVBO) == GL_TRUE);
        //assert(g_hTextureVAO == 0);

        bOut = bOut && glCheck() == GL_NO_ERROR;
        
        //Load the default texture
        glGenTextures(1, &g_hDefaultTexture);
        glBindTexture(GL_TEXTURE_2D, g_hDefaultTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<const unsigned char*>(cbpp::g_aDefaultTexture));
        glBindTexture(GL_TEXTURE_2D, 0);

        bOut = bOut && glCheck() == GL_NO_ERROR;

        cbvs::Texture hFontTexture;
        hFontTexture.Load("ddraw/font.png", cbvs::IMG_L);
        g_hFontTexture = hFontTexture.GetHandle();
        
        /*
        bOut = bOut && glCheck() == GL_NO_ERROR;

        glGenVertexArrays(1, &g_hTextVAO);
        glGenBuffers(1, &g_hTextVBO);

        glBindVertexArray(1);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, g_hTextVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_aMeshBuffer), g_aMeshBuffer, GL_DYNAMIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2+1, (GLvoid*)0); //X,Y
            glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLfloat)*2+1, (GLvoid*)(sizeof(GLfloat)*2)); //character ID
        glBindVertexArray(0);
        
        bOut = bOut && glCheck() == GL_NO_ERROR;
        */
        return bOut;
    }

    void SetColor(const cbpp::Color& cNewColor) noexcept {
        g_cActiveColor = cNewColor.Normalized();
    }

    void ApplyColor(const cbvs::Pipe* hTarget) noexcept {
        GLint hColor = hTarget->GetUniform("cbpp_COLOR");
        GLuint hPipeID = hTarget->GetHandle();

        if(hColor != -1) {
            glUniform4f(hColor, g_cActiveColor.r,
                                g_cActiveColor.g,
                                g_cActiveColor.b,
                                g_cActiveColor.a);
        }
    }
}

namespace ddraw {
    void Line(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2, cbpp::float_t fWidth) noexcept {
        g_hGeneric->Use();

        ApplyColor(g_hGeneric);

        GLfloat aBuffer[] = {
            vPos1.x, vPos1.y,
            vPos2.x, vPos2.y
        };

        glLineWidth(fWidth);

        glBindVertexArray(g_hVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_hVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(aBuffer), aBuffer);
            glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        glCheck();
    }

    void RectOutline(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2, cbpp::float_t fWidth) noexcept {
        g_hGeneric->Use();
        ApplyColor(g_hGeneric);

        GLfloat aBuffer[] = {
            vPos1.x, vPos1.y,
            vPos2.x, vPos1.y,
            vPos2.x, vPos2.y,
            vPos1.x, vPos2.y
        };

        glLineWidth(fWidth);

        glBindVertexArray(g_hVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_hVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(aBuffer), aBuffer);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);

        glCheck();
    }

    void Rect(cbpp::Vec2 vPos1, cbpp::Vec2 vPos2) noexcept {
        g_hGeneric->Use();
        ApplyColor(g_hGeneric);

        GLfloat aBuffer[] = {
            vPos1.x, vPos1.y,
            vPos2.x, vPos1.y,
            vPos2.x, vPos2.y,

            vPos1.x, vPos1.y,
            vPos2.x, vPos2.y,
            vPos1.x, vPos2.y
        };

        glBindVertexArray(g_hVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_hVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(aBuffer), aBuffer);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glCheck();
    }

    void CircleOutline(cbpp::Vec2 vPos, cbpp::float_t fRadius, cbpp::float_t fWidth) noexcept {
        g_hCircle->Use();
        ApplyColor(g_hCircle);

        g_hCircle->PushUniform("cbpp_RATIO", cbvs::g_fScreenRatio);
        g_hCircle->PushUniform("cbpp_CIRCLE_RADIUS", fRadius);
        g_hCircle->PushUniform("cbpp_CIRCLE_FIDELITY", DDRAW_CIRCLE_FIDELITY);

        glLineWidth(fWidth);

        g_aMeshBuffer[0] = vPos.x;
        g_aMeshBuffer[1] = vPos.y;

        glBindVertexArray(g_hVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_hVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*2, g_aMeshBuffer);
            glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        glCheck();
    }

    void Circle(cbpp::Vec2 vPos, cbpp::float_t fRadius) noexcept {
        g_hCirclef->Use();
        ApplyColor(g_hCirclef);

        g_hCirclef->PushUniform("cbpp_RATIO", cbvs::g_fScreenRatio);
        g_hCirclef->PushUniform("cbpp_CIRCLE_RADIUS", fRadius);
        g_hCirclef->PushUniform("cbpp_CIRCLE_FIDELITY", DDRAW_CIRCLE_FIDELITY);

        g_aMeshBuffer[0] = vPos.x;
        g_aMeshBuffer[1] = vPos.y;

        glBindVertexArray(g_hVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_hVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*2, g_aMeshBuffer);
            glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        glCheck();
    }

    void Texture(cbpp::Vec2 vPos, cbpp::Vec2 vScale, cbvs::Texture& hImage, bool bKeepRatio) noexcept {
        g_hTexture->Use();

        if(bKeepRatio) {
            g_hTexture->PushUniform("cbpp_RATIO", cbvs::g_fScreenRatio);
        }else{
            g_hTexture->PushUniform("cbpp_RATIO", 1.0f);
        }

        cbpp::Vec2 vPos1 = vPos, vPos2 = vPos + vScale;
        
        GLfloat aBuffer[] = {
            vPos1.x, vPos1.y, 0.0f, 0.0f,
            vPos2.x, vPos1.y, 1.0f, 0.0f,
            vPos2.x, vPos2.y, 1.0f, 1.0f,

            vPos1.x, vPos1.y, 0.0f, 0.0f,
            vPos2.x, vPos2.y, 1.0f, 1.0f,
            vPos1.x, vPos2.y, 0.0f, 1.0f
        };

        glBindTexture(GL_TEXTURE_2D, hImage.GetHandle());
        glBindVertexArray(g_hTextureVAO);
            glBindBuffer(GL_ARRAY_BUFFER, g_hTextureVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(aBuffer), aBuffer);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glCheck();
    }

    void Text(cbpp::Vec2 vPos, const char* sSource, cbpp::float_t fScale) noexcept {

    }
}
