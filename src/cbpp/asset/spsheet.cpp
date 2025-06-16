#include "cbpp/asset/spsheet.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"
#include "cbpp/ttype/list.h"
#include "cbvs/texture.h"
#include "cbvs/render.h"

#include <string.h>

#include "cdf/cdf.h"
#include "cbpp/asset/cdf_classes.h"
#include "cbpp/asset/sdk_structs.h"

namespace cbpp {
    List<SpriteSheet*> g_aSheets; 

    void SpriteSheet::GetResolution(texres_t& iW, texres_t& iH) const {
        iW = m_iWidth;
        iH = m_iHeight;
    }

    SpriteSheet::~SpriteSheet() {
        glDeleteTextures(1, &m_hTexture);
    }

    bool LoadTextureSheet(const char* sPath) {
        File* hInput = OpenFile(PATH_TEXTURE, sPath, "rb");

        cdf_document* pDoc;
        cdf_verinfo Version;
        int16_t iClassID;

        cdf_retcode iCode = cdf_file_read((FILE*)(hInput->Handle()), &pDoc, &Version, &iClassID);

        hInput->Close();

        if(iCode != CDF_OK) {
            PushError(ERROR_CDF, cdf_get_error(iCode));
            return false;
        }

        if(iClassID != CDF_CLASS_SPRITESHEET) {
            PushError(ERROR_IO, "Not a spritesheet");
            cdf_document_destroy(pDoc);
            return false;
        }

        SpriteSheet* Out = new SpriteSheet();

        cdf_object Current;
        cdf_object* pRoot = cdf_document_root(pDoc);
        size_t iIter = 0;
        sdk_ImageInfo ImageData;

        Color* aImage = NULL;
        size_t iImageBytes;

        while( cdf_object_iterate(pRoot, &Current, &iIter) ) {
            const char* sName = cdf_object_name(pDoc, &Current);

            if( strcmp(sName, "cta_imginfo") == 0 ) {
                memcpy(&ImageData, cdf_object_data(&Current), sizeof(ImageData));
            }

            if( strcmp(sName, "cta_mapping") == 0 ) {
                cdf_uint iArrayLen = cdf_array_length(&Current);
                sdk_Sprite Buff;
                cdf_object Obj;
                SpriteMapping Map;

                for(size_t i = 0; i < iArrayLen; i++) {
                    cdf_array_index(&Current, &Obj, i);
                    memcpy(&Buff, cdf_object_data(&Obj), sizeof(Buff));

                    Map.X = (float_t)(Buff.iX) / (float_t)(ImageData.m_iWidth);
                    Map.Y = (float_t)(Buff.iY) / (float_t)(ImageData.m_iHeight);
                    Map.W = (float_t)(Buff.iW + Buff.iX) / (float_t)(ImageData.m_iWidth);
                    Map.H = (float_t)(Buff.iH + Buff.iY) / (float_t)(ImageData.m_iHeight);

                    const char* sSpriteName = pDoc->m_aNames[Buff.iNameID];
                    Out->m_mMapping[sSpriteName] = Map;
                }
            }

            if( strcmp(sName, "cta_raster") == 0 ) {
                iImageBytes = cdf_object_length(&Current);
                aImage = (Color*) cdf_object_data(&Current);
            }
        }

        if(aImage == NULL) {
            PushError(ERROR_IO, "Failed to get image data from the document");
            cdf_document_destroy(pDoc);
            return false;
        }

        Out->m_hTexture = cbvs::CreateTexture(aImage, ImageData.m_iWidth, ImageData.m_iHeight);
        Out->m_iWidth = ImageData.m_iWidth;
        Out->m_iHeight = ImageData.m_iHeight;

        g_aSheets.PushBack(Out);
        
        cdf_document_destroy(pDoc);
        return true;
    }

    void RemapTextureSheet(SpriteSheet& Target, texres_t iOldW, texres_t iOldH, texres_t iNewW, texres_t iNewH) {
        float_t fRatioX = (float_t)(iOldW) / (float_t)(iNewW);
        float_t fRatioY = (float_t)(iOldH) / (float_t)(iNewH);

        for(auto Iter = Target.m_mMapping.begin(); Iter != Target.m_mMapping.end(); Iter++) {
            SpriteMapping& Current = Iter->second;
            Current.H *= fRatioY;
            Current.W *= fRatioX;
            Current.X *= fRatioX;
            Current.Y *= fRatioY;
        }
    }

    void CompileTextureSheets() {
        cbvs::SpriteComposer* pComposer = new cbvs::SpriteComposer();
        pComposer->Reset();

        
    }

    SpriteInfo GetSpriteInfo(const char* sName) {
        SpriteInfo Out;
        memset(&Out, 0, sizeof(Out));

        for(size_t i = 0; i < g_aSheets.Length(); i++) {
            const mapping_t& Map = g_aSheets.At(i)->m_mMapping;
            if(Map.count(sName)) {
                Out.Mapping = Map.at(sName);
                Out.TextureID = g_aSheets.At(i)->m_hTexture;
                return Out;
            }
        }

        return Out;
    }
}
