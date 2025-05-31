/*
    Cuber Texture Atlas (CTA) packaging tool
    The format is based on CDF, so you can
    use /sdk/cdftree on any .cta file to check it`s tree structure.
*/

#include "cdf/cdf.h"
#include "cbpp/yyjson.h"
#include "SOIL/SOIL.h"

#include <unistd.h>
#include <stdlib.h>

#include "sdk_main.h"
#include "cbpp/asset/cdf_classes.h"
#include "cbpp/asset/sdk_structs.h"

const char* g_sArgs = "hlf:o:c:";

const char* g_sHelpMsg = "CBPP SDK\nshtool:\n'-h' - Display this message and exit\n'-f' - Input file path\n\
'-o' - Output file path\n'-c' - Specify the compression level (0-9), 6 by default\n\
'-l' - Treat output path as relative to the input .json file path, disabled by default\n";

int g_iCompressionLevel = 6;

const char* g_sFilePath;
char g_bHasFile = 0;
char g_bOutLocal = 0;

char* g_sOutputPath = NULL;

char IsPOT(int x) {
    return (x > 0) && ((x & (x - 1)) == 0);
}

void compile_sheet(yyjson_val* jRoot) {
    yyjson_val* jRaster = yyjson_obj_get(jRoot, "raster");
    if(!yyjson_is_str(jRaster)) { sdk_error("JSON file has no proper 'raster' string specified"); }

    yyjson_val* jMapping = yyjson_obj_get(jRoot, "mapping");
    if(!yyjson_is_obj(jMapping)) { sdk_error("JSON file has no proper 'mapping' dictionary specified"); }

    const char* sRasterPath = yyjson_get_str(jRaster);

    char* pSeparator = strrchr(g_sFilePath, '/');
    char* sDirectory = (char*) malloc(strlen(g_sFilePath) + strlen(sRasterPath) + 5);
    sDirectory[0] = '\0';

    if(pSeparator != NULL) {
        *pSeparator = '\0';
        strcat(sDirectory, g_sFilePath);
        strcat(sDirectory, "/");
        *pSeparator = '/';

        sDirectory = realloc(sDirectory, strlen(sDirectory)+1);
    }

    char* sPathBuffer = (char*) malloc( strlen(sDirectory) + strlen(sRasterPath) + 1 );
    sPathBuffer[0] = '\0';
    strcat(sPathBuffer, sDirectory);
    strcat(sPathBuffer, sRasterPath);

    FILE* pRasterFile = fopen(sPathBuffer, "rb");

    if(pRasterFile == NULL) {
        sdk_errorf("Unable to open raster image file '%s'", sPathBuffer);
    }

    fseek(pRasterFile, 0, SEEK_END);
    size_t iRasterLength = ftell(pRasterFile);
    fseek(pRasterFile, 0, SEEK_SET);

    char* aImageData = (char*) malloc(iRasterLength);
    fread(aImageData, 1, iRasterLength, pRasterFile);

    fclose(pRasterFile);

    int iWidth, iHeight, iChannels;

    unsigned char* pRawImage = SOIL_load_image_from_memory(aImageData, iRasterLength, &iWidth, &iHeight, &iChannels, SOIL_LOAD_RGBA);
    free(aImageData);

    if(iChannels != SOIL_LOAD_RGBA) {
        printf("Warning! The source image channels are not RGBA, forcibly loading as RGBA\n");
    }

    if(pRawImage == NULL) {
        sdk_errorf("SOIL: %s", SOIL_last_result());
    }

    if(iWidth != iHeight) {
        sdk_error("Non-square source images are not supported yet");
    }

    if(!IsPOT(iWidth)) {
        sdk_error("NPOT resolution source images are not supported");
    }
    
    cdf_document* pDoc = cdf_document_create();
    cdf_object* pRoot = cdf_document_root(pDoc);
    cdf_object* pMappingObj = cdf_object_create(pDoc, "cta_mapping", CDF_TYPE_ARRAY);

    yyjson_val *jKey, *jVal;
    yyjson_obj_iter Iter = yyjson_obj_iter_with(jMapping);

    sdk_Sprite SpriteBuffer;

    while ((jKey = yyjson_obj_iter_next(&Iter))) {
        jVal = yyjson_obj_iter_get_val(jKey);
        
        const char* sName = yyjson_get_str(jKey);

        if(!yyjson_is_arr(jVal)) {
            sdk_errorf("Invalid mapping info for the sprite '%s'", sName);
        }

        if(yyjson_arr_size(jVal) < 4) {
            sdk_errorf("Too short mapping (must be at least 4 numbers) for the sprite '%s'", sName);
        }
        
        SpriteBuffer.iX = yyjson_get_int(yyjson_arr_get(jVal, 0));
        SpriteBuffer.iY = yyjson_get_int(yyjson_arr_get(jVal, 1));
        SpriteBuffer.iW = yyjson_get_int(yyjson_arr_get(jVal, 2));
        SpriteBuffer.iH = yyjson_get_int(yyjson_arr_get(jVal, 3));
        SpriteBuffer.iNameID = cdf_nametable_push(pDoc, sName);

        sdk_cdf_validate( cdf_array_data_push_ex(pMappingObj, &SpriteBuffer, sizeof(sdk_Sprite), CDF_TYPE_BINARY) )
    }

    sdk_ImageInfo ImgData;
    ImgData.m_iResolution = iWidth;

    sdk_cdf_validate( cdf_push_binary(pDoc, pRoot, "cta_imginfo", (uint8_t*)&ImgData, sizeof(ImgData)) )
    sdk_cdf_validate( cdf_object_push(pRoot, pMappingObj) )
    sdk_cdf_validate( cdf_push_binary(pDoc, pRoot, "cta_raster", pRawImage, iWidth*iHeight*iChannels) )

    FILE* hFile = NULL;
    
    if(g_bOutLocal == 0) {
        hFile = fopen(g_sOutputPath, "wb");
    }else{
        sPathBuffer = (char*) realloc(sPathBuffer, strlen(sDirectory) + strlen(g_sOutputPath) + 1);

        sPathBuffer[0] = '\0';
        strcat(sPathBuffer, sDirectory);
        strcat(sPathBuffer, g_sOutputPath);

        hFile = fopen(sPathBuffer, "wb");
    }

    if(hFile == NULL) {
        sdk_errorf("Unable to open output file '%s'", g_sOutputPath);
    }
    
    sdk_cdf_validate( cdf_file_write(hFile, pDoc, CDF_CLASS_SPRITESHEET, g_iCompressionLevel) )
    fclose(hFile);
    cdf_document_destroy(pDoc);
    cdf_object_destroy(NULL, pMappingObj);

    SOIL_free_image_data(pRawImage);

    free(sDirectory);
    free(sPathBuffer);
}

void compile_font() {

}

int main(int argc, char** argv) {
    const char* sMode;

    char c;
    while( (c = getopt(argc, argv, g_sArgs)) != -1 ) {
        const char* sOpt = optarg;
        switch ( c ) {
            case 'h': printf("%s\n", g_sHelpMsg); exit(EXIT_SUCCESS);
            case 'f': g_sFilePath = sOpt; g_bHasFile = 1; break;
            case 'o': g_sOutputPath = optarg; break;
            case 'c': g_iCompressionLevel = atoi(optarg); break;
            case 'l': g_bOutLocal = 1; break;
        }
    }

    if(!g_bHasFile) { sdk_error("No input file providen"); }

    FILE* pFile = fopen(g_sFilePath, "rb");
    if(pFile == NULL) { sdk_errorf("Can`t open file '%s'", g_sFilePath); }

    fseek(pFile, 0, SEEK_END);
    size_t iFileLength = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    char* aFileBuffer = (char*) malloc(iFileLength+1);
    aFileBuffer[iFileLength] = '\0';
    fread(aFileBuffer, 1, iFileLength, pFile);
    fclose(pFile);

    yyjson_doc* jDoc = yyjson_read_opts(aFileBuffer, iFileLength, CBPP_JSONREAD_OPTS, NULL, NULL);
    if(jDoc == NULL) { sdk_error("Failed to parse JSON file"); }

    free(aFileBuffer);

    yyjson_val* jRoot = yyjson_doc_get_root(jDoc);
    if(jRoot == NULL) { sdk_error("JSON has no root value"); }

    yyjson_val* jType = yyjson_obj_get(jRoot, "type");
    if(jType == NULL) { sdk_error("JSON file has no proper 'type' string specified"); }
    if(yyjson_is_str(jType)){
        sMode = yyjson_get_str( jType );
    }else{
        sMode = "atlas";
    }

    if( strcmp("atlas", sMode) == 0 ) {
        compile_sheet(jRoot);
    }else if( strcmp("font", sMode) == 0 ) {

    }else{
        sdk_errorf("Unknown type: '%s'", sMode);
    }

    return EXIT_SUCCESS;
}
