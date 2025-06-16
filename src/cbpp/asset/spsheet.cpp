#include "cbpp/asset/spsheet.h"
#include "cbpp/error.h"
#include "cbpp/fileio.h"
#include "cbpp/ttype/list.h"
#include "cbvs/texture.h"
#include "cbvs/render.h"
#include "cbpp/color.h"

#include <string.h>

#include "cdf/cdf.h"
#include "cbpp/asset/cdf_classes.h"
#include "cbpp/asset/sdk_structs.h"

namespace cbpp {
    List<SpriteInfo> g_aSprites;

    /*
    void SpriteSheet::GetResolution(texres_t& iW, texres_t& iH) const {
        iW = m_iWidth;
        iH = m_iHeight;
    }

    SpriteSheet::~SpriteSheet() {
        glDeleteTextures(1, &m_hTexture);
    }
    */

    bool LoadTextureSheet( const char* sPath, bool bAppendExt ) {
        char sPathBuffer[128];

        const char* sExt;
        if(bAppendExt) { sExt = ".cta"; }
        else{ sExt = ""; }

        snprintf(sPathBuffer, 128, "%s%s", sPath, sExt);

        File* hInput = OpenFile(PATH_TEXTURE, sPathBuffer, "rb");

        cdf_document* pDoc;
        cdf_verinfo VersionData;
        int16_t iClassID;

        cdf_retcode iCode = cdf_file_read(hInput->Handle(), &pDoc, &VersionData, &iClassID);
        if(iCode != CDF_OK) {
            PushError(ERROR_CDF, cdf_get_error(iCode));
            delete hInput;
            return false;
        }

        delete hInput;

        cdf_object* pRoot = cdf_document_root(pDoc);
        cdf_object Current, Raster, Mapping, RasterData;
        size_t Iterator = 0;

        bool bHasRaster = false, bHasImgInfo = false, bHasMapping = false;

        while( cdf_object_iterate(pRoot, &Current, &Iterator) ) {
            const char* sObjName = cdf_object_name(pDoc, &Current);   // Parse the document and extract required fields

            if( strcmp("cta_imginfo", sObjName) == 0 ) {
                RasterData = Current;
                bHasImgInfo = true;
            }

            if( strcmp("cta_raster", sObjName) == 0 ) {
                Raster = Current;
                bHasRaster = true;
            }  

            if( strcmp("cta_mapping", sObjName) == 0 ) {
                Mapping = Current;
                bHasMapping = true;
            }
        }

        const char* sErrorMsg;

        if(!bHasImgInfo) { sErrorMsg = "The 'cta_imginfo' field is missing"; }
        if(!bHasMapping) { sErrorMsg = "The 'cta_mapping' field is missing"; }
        if(!bHasRaster)  { sErrorMsg = "The 'cta_raster' field is missing"; }

        if(!(bHasImgInfo && bHasMapping && bHasRaster)) { // Report a error if a particular field is missing
            PushError(ERROR_IO, sErrorMsg);
            cdf_document_destroy(pDoc);
            return false;
        }

        // Perform some safety checks
        if(cdf_object_type(&Mapping) != CDF_TYPE_ARRAY) {
            PushError(ERROR_IO, "Wrong 'cta_mapping' datatype: must be an array");
            cdf_document_destroy(pDoc);
            return false;
        }

        if(cdf_object_length(&RasterData) != sizeof(sdk_ImageInfo)) {
            PushError(ERROR_IO, "'cta_imginfo' size mismatch");
            cdf_document_destroy(pDoc);
            return false;
        }

        if(cdf_array_usize(&Mapping) != sizeof(sdk_Sprite)) {
            PushError(ERROR_IO, "Mapping unit size mismatch");
            cdf_document_destroy(pDoc);
            return false;
        }

        sdk_ImageInfo SourceImageInfo;
        memcpy( &SourceImageInfo, cdf_object_data(&RasterData), cdf_object_length(&RasterData) );

        /*
            Using cbvs::Image class to perform a convertion to RGBA if needed.

            A spritesheet file can store source image in any channels format it wants, but
            texture atlases only use RGBA, so convertion is required.
        */
        cbvs::Image RasterImage( (const uint8_t*)cdf_object_data(&Raster), 
                                SourceImageInfo.m_iWidth, 
                                SourceImageInfo.m_iHeight, 
                                SourceImageInfo.m_iChannels );

        GLuint hAtlasTexture = cbvs::CreateTexture(RasterImage);

        cdf_object CurrentSprite;
        cdf_uint iLength = cdf_array_length(&Mapping);

        SpriteInfo Sprite;
        sdk_Sprite CurrentMapping;

        for(size_t i = 0; i < iLength; i++) {
            cdf_array_index(&Mapping, &CurrentSprite, i);
            memcpy( &CurrentMapping, cdf_object_data(&CurrentSprite), cdf_object_length(&CurrentSprite) );

            Sprite.TextureID = hAtlasTexture;
            Sprite.Name = strdup( pDoc->m_aNames[ CurrentMapping.iNameID ] );

            Sprite.Mapping.X = (float_t)(CurrentMapping.iX) / (float_t)(SourceImageInfo.m_iWidth);
            Sprite.Mapping.Y = (float_t)(CurrentMapping.iY) / (float_t)(SourceImageInfo.m_iHeight);
            Sprite.Mapping.W = (float_t)(CurrentMapping.iX + CurrentMapping.iW) / (float_t)(SourceImageInfo.m_iWidth);
            Sprite.Mapping.H = (float_t)(CurrentMapping.iY + CurrentMapping.iH) / (float_t)(SourceImageInfo.m_iHeight);

            g_aSprites.PushBack(Sprite);
        }

        cdf_document_destroy(pDoc);
        return true;
    }
    
/*
    bool LoadTextureSheet(const char* sPath) {
        char sPathBuffer[512];
        snprintf(sPathBuffer, 512, "%s%s", sPath, ".cta");

        File* hInput = OpenFile(PATH_TEXTURE, sPathBuffer, "rb");

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

        cdf_object Current;
        cdf_object* pRoot = cdf_document_root(pDoc);
        size_t iIter = 0;
        sdk_ImageInfo ImageData;

        Color* aImage = NULL;
        size_t iImageBytes;

        GLuint hTex = 0;

        SpriteInfo& SpriteBuff = g_aSprites.At( g_aSprites.PushEmpty() );

        while( cdf_object_iterate(pRoot, &Current, &iIter) ) {
            const char* sName = cdf_object_name(pDoc, &Current);

            if( strcmp(sName, "cta_imginfo") == 0 ) {
                memcpy(&ImageData, cdf_object_data(&Current), sizeof(ImageData));
            }

            if( strcmp(sName, "cta_mapping") == 0 ) {
                cdf_uint iArrayLen = cdf_array_length(&Current);
                sdk_Sprite Buff;
                cdf_object Obj;
                SpriteMapping& Map = SpriteBuff.Mapping;

                for(size_t i = 0; i < iArrayLen; i++) {
                    cdf_array_index(&Current, &Obj, i);
                    memcpy(&Buff, cdf_object_data(&Obj), sizeof(Buff));

                    Map.X = (float_t)(Buff.iX) / (float_t)(ImageData.m_iWidth);
                    Map.Y = (float_t)(Buff.iY) / (float_t)(ImageData.m_iHeight);
                    Map.W = (float_t)(Buff.iW + Buff.iX) / (float_t)(ImageData.m_iWidth);
                    Map.H = (float_t)(Buff.iH + Buff.iY) / (float_t)(ImageData.m_iHeight);

                    SpriteBuff.Name = strdup( pDoc->m_aNames[Buff.iNameID] );
                    SpriteBuff.TextureID = hTex;

                    printf("%s\n", SpriteBuff.Name);
                }
            }

            if( strcmp(sName, "cta_raster") == 0 ) {
                iImageBytes = cdf_object_length(&Current);
                aImage = (Color*) cdf_object_data(&Current);

                if(aImage == NULL) {
                    PushError(ERROR_IO, "Failed to get image data from the document");
                    cdf_document_destroy(pDoc);
                    return false;
                }

                hTex = cbvs::CreateTexture(aImage, ImageData.m_iWidth, ImageData.m_iHeight);
            }
        }
        
        cdf_document_destroy(pDoc);
        return true;
    }*/

    void CompileTextureSheets() {
        cbvs::SpriteComposer* pComposer = new cbvs::SpriteComposer();
        pComposer->Reset();        
    }

    spriteid_t GetSpriteID( const char* sName ) {
        for(size_t i = 0; i < g_aSprites.Length(); i++) {
            if( strcmp(g_aSprites.At(i).Name, sName) == 0 ) {
                return i;
            }
        }

        return 0;
    }

    const SpriteInfo& GetSpriteInfo(spriteid_t iIndex) {
        if(iIndex >= g_aSprites.Length()) {
            iIndex = 0;
        }

        return g_aSprites.At( iIndex );
    }

    void CleanupSprites() {
        for(size_t i = 0; i < g_aSprites.Length(); i++) {
            free( (char*)(g_aSprites.At(i).Name) );
        }

        g_aSprites.Clear();
    }
}
