#include "cbvs/render.h"
#include "cbpp/error.h"
#include "cbpp/asset/spsheet.h"
#include "cbvs/ddraw.h"
#include "cbvs/buffer.h"

#include <assert.h>

namespace cbvs {
    cbpp::float_t g_fScreenRatio = 1.0f;
    cbpp::Vec2 g_vScreenSize;

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
    void SpriteComposer::Reset() {
        memset(m_aMappingInfo, 0, sizeof(m_aMappingInfo));
    }

    GLuint SpriteComposer::GenerateTexture() const {
        return CreateTexture((const cbpp::Color*)(m_aImage), CBVS_SPRITEBATCH_SIZE, CBVS_SPRITEBATCH_SIZE);
    }

    void SpriteComposer::MarkRegionAs(int iX, int iY, int iW, int iH, bool bMark) {
        for(int Y0 = iY; Y0 < iY + iH; Y0++) {
            for(int X0 = iX; X0 < iX + iW; X0++) {
                m_aMappingInfo[Y0][X0] = bMark;
            }
        }
    }

    bool SpriteComposer::ScanRegion(int iX, int iY, int iW, int iH) const {
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

    bool SpriteComposer::LocateFreeRegion(int iW, int iH, int& iX, int& iY) const {
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

    void SpriteComposer::BlitImage(const cbvs::Image& Source, texres_t iX, texres_t iY) {
        const cbpp::Color* aPixels = Source.GetPixelData();

        for(size_t i = iY; i < Source.Height(); i++) {
            const cbpp::Color* pSource = &aPixels[Source.Width() * i];
            cbpp::Color* pTarget = m_aImage[i] + iX;
            memcpy(pTarget, pSource, Source.Width());
        }
    }

    void SpriteComposer::PrintMappingInfo() const {
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

    VertexBuffer<SpriteVertex>* GetSpriteRenderingBuffer() {
        static VertexBuffer<SpriteVertex>* s_hSpriteBuffer = new VertexBuffer<SpriteVertex>();
        return s_hSpriteBuffer;
    }

    bool InitRender() noexcept {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        VertexBuffer<SpriteVertex>* hSpriteBuff = GetSpriteRenderingBuffer();

        SpriteVertex aData[6];

        hSpriteBuff->Init(
            aData, 6,
            GL_DYNAMIC_DRAW
        );

        return true;
    }
    
    void CleanupRender() noexcept {
        delete GetSpriteRenderingBuffer();
    }

    void RenderSprite(cbpp::spriteid_t iIndex, cbpp::Vec2 vPos, cbpp::Vec2 vScale, cbpp::float_t fAngle, cbpp::Color iColor, cbpp::float_t fDepth) {
        static SpriteVertex s_aSpriteVtxBuff[6];
        static SpriteVertex vP1, vP2, vP3, vP4; //top-left, top-right, bottom-left, bottom-right

        const cbpp::SpriteInfo& Info = cbpp::GetSpriteInfo(iIndex);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        static Pipe* pSpritePipe = GetPipe("cbpp_sprite");

        const cbpp::float_t fRatio = (Info.Mapping.W - Info.Mapping.X) / (Info.Mapping.H - Info.Mapping.Y);
        const cbpp::Vec2 vScaleFinal = vScale * cbpp::Vec2(fRatio, 1.0f);

        vP1 = {  vPos - vScaleFinal*0.5f,                                              {Info.Mapping.X, Info.Mapping.H} };
        vP2 = { cbpp::Vec2(vPos.x + vScale.x*fRatio, vPos.y) - vScaleFinal*0.5f,       {Info.Mapping.W, Info.Mapping.H} };
        vP3 = { cbpp::Vec2(vPos.x, vPos.y + vScale.y) - vScaleFinal*0.5f,              {Info.Mapping.X, Info.Mapping.Y} };
        vP4 = {  vPos + vScale*cbpp::Vec2(fRatio, 1.0f) - vScaleFinal*0.5f,            {Info.Mapping.W, Info.Mapping.Y} };

        s_aSpriteVtxBuff[0] = vP3;
        s_aSpriteVtxBuff[1] = vP2;
        s_aSpriteVtxBuff[2] = vP1; // Generate a quad

        s_aSpriteVtxBuff[3] = vP4;
        s_aSpriteVtxBuff[4] = vP3;
        s_aSpriteVtxBuff[5] = vP2;

        VertexBuffer<SpriteVertex>* hSpriteBuff = GetSpriteRenderingBuffer();

        hSpriteBuff->Use();
        pSpritePipe->Use();

        glBindTexture(GL_TEXTURE_2D, Info.TextureID);

        pSpritePipe->PushUniform("cbpp_Depth", fDepth);
        pSpritePipe->PushUniform("cbpp_Ratio", g_fScreenRatio);
        pSpritePipe->PushUniform("cbpp_Color", iColor);
        pSpritePipe->PushUniform("cbpp_Angle", fAngle);
        pSpritePipe->PushUniform("cbpp_RotateOrigin", (vP1.vPos + vP4.vPos) / 2);

        hSpriteBuff->PushVertexData(s_aSpriteVtxBuff, 6);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glCheck();
    }
}
