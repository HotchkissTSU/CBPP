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

    bool LoadSheet( const char* sPath, bool bAppendExt ) {
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

        for(int i = 0; i < pDoc->m_iNames; i++) {
            printf("cdf: %s\n", pDoc->m_aNames[i]);
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

        if(cdf_object_length(&Raster) != SourceImageInfo.m_iHeight*SourceImageInfo.m_iWidth*SourceImageInfo.m_iChannels) {
            PushError(ERROR_IO, "Raster length does not correspond to cta_imginfo");
            cdf_document_destroy(pDoc);
            return false;
        }

        /*
            Using cbvs::Image class to perform a convertion to RGBA if needed.

            A spritesheet file can store source image in any channels format it wants, but
            texture atlases only use RGBA, so possible convertion is required.
        */
        cbvs::Image RasterImage( (const uint8_t*)cdf_object_data(&Raster), 
                                SourceImageInfo.m_iWidth, 
                                SourceImageInfo.m_iHeight, 
                                SourceImageInfo.m_iChannels );

        GLuint hAtlasTexture = cbvs::CreateTexture(RasterImage, false);

        cdf_object CurrentSprite;
        cdf_uint iLength = cdf_array_length(&Mapping);

        SpriteInfo Sprite;
        sdk_Sprite CurrentMapping;

        // Iterate each mapping and push it into the global registry
        for(size_t i = 0; i < iLength; i++) {
            cdf_array_index(&Mapping, &CurrentSprite, i);
            memcpy( &CurrentMapping, cdf_object_data(&CurrentSprite), cdf_object_length(&CurrentSprite) );

            Sprite.TextureID = hAtlasTexture;
            Sprite.Name = strdup( pDoc->m_aNames[ CurrentMapping.iNameID ] );

            printf("mapper: %s\n", Sprite.Name);

            Sprite.Mapping.X = (float_t)(CurrentMapping.iX) / (float_t)(SourceImageInfo.m_iWidth);
            Sprite.Mapping.Y = (float_t)(CurrentMapping.iY) / (float_t)(SourceImageInfo.m_iHeight);
            Sprite.Mapping.W = (float_t)(CurrentMapping.iX + CurrentMapping.iW) / (float_t)(SourceImageInfo.m_iWidth);
            Sprite.Mapping.H = (float_t)(CurrentMapping.iY + CurrentMapping.iH) / (float_t)(SourceImageInfo.m_iHeight);

            g_aSprites.PushBack(Sprite);
        }

        cdf_document_destroy(pDoc);
        return true;
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
            glDeleteTextures(1, &g_aSprites.At(i).TextureID);
        }

        g_aSprites.Clear();
    }
}
