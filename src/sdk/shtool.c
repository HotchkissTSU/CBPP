/*
    Cuber Texture Atlas (CTA) packaging/mapping tool
    The format is based on the CDF, so you can
    use sdk/cdftree on any .cta file to check it`s tree structure.
*/

#include "cdf/cdf.h"
#include "cbpp/yyjson.h"
#include "SOIL/SOIL.h"

#include <unistd.h>
#include <stdlib.h>

#include "sdk_main.h"
#include "cbpp/asset/cdf_classes.h"
#include "cbpp/asset/sdk_structs.h"

const char* g_sArgs = "hm:f:o:";

const char* g_aModes[] = {
    "comp",
    "amap"
};

const char* g_sHelpMsg = "CBPP SDK\nshtool:\n'-h' - display this message\n'-m' - select the tool`s mode\n\'-o' - General output path\n\n\
Mode 'comp' - compile a sheet:\n\t'-f' - Path to the sheet .json file\n\nMode 'amap' - perform automatic mapping:\n\t'--offset X,Y' - Set the X and Y offset\
of the entire map\n\t'--spacing X,Y' - Set the spacing between the tiles\n\t'--size X,Y' - Set the size of the single tile";

int g_iX = 0, g_iY = 0;
int g_iW = 16, g_iH = 16;
int g_iRow = 8, g_iNumRows = 8;

const char* g_sFilePath;
char g_bHasFile = 0;

char* g_sOutputPath = NULL;

void compile_sheet() {
    printf("file = %s\n", g_sFilePath);

    FILE* pFile = fopen(g_sFilePath, "rb");
    if(pFile == NULL) { sdk_errorf("Cant open file '%s'", g_sFilePath); }

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

    yyjson_val* jRaster = yyjson_obj_get(jRoot, "raster");
    if(!yyjson_is_str(jRaster)) { sdk_error("JSON file has no proper 'raster' string specified"); }

    yyjson_val* jMapping = yyjson_obj_get(jRoot, "mapping");
    if(!yyjson_is_obj(jMapping)) { sdk_error("JSON file has no proper 'mapping' dictionary specified"); }

    const char* sRasterPath = yyjson_get_str(jRaster);
    size_t iRasterNameLength = strlen(sRasterPath);

    size_t iPathBuffLen = strlen(g_sFilePath)+1;
    char* sPathBuffer = (char*) malloc(iPathBuffLen);
    memcpy(sPathBuffer, g_sFilePath, iPathBuffLen);

    FILE* pRasterFile = NULL;
    char* pDot = strrchr(sPathBuffer, '/');
    if(pDot != NULL) {
        *pDot = '\0';
        iPathBuffLen += iRasterNameLength + 2;
        sPathBuffer = (char*) realloc(sPathBuffer, iPathBuffLen);
        strcat(sPathBuffer, "/");
        strcat(sPathBuffer, sRasterPath);

        pRasterFile = fopen(sPathBuffer, "rb");
    }else{
        pRasterFile = fopen(sRasterPath, "rb");
    }

    if(pRasterFile == NULL) {
        sdk_errorf("Unable to open raster image file '%s'", sPathBuffer);
    }
    
    free(sPathBuffer);

    fseek(pRasterFile, 0, SEEK_END);
    size_t iRasterLength = ftell(pRasterFile);
    fseek(pRasterFile, 0, SEEK_SET);

    char* aImageData = (char*) malloc(iRasterLength);
    fread(aImageData, 1, iRasterLength, pRasterFile);

    fclose(pRasterFile);

    int iWidth, iHeight, iChannels;

    unsigned char* pRawImage = SOIL_load_image_from_memory(aImageData, iRasterLength, &iWidth, &iHeight, &iChannels, SOIL_LOAD_AUTO);
    free(aImageData);

    if(pRawImage == NULL) {
        sdk_errorf("SOIL: %s", SOIL_last_result());
    }

    printf("w=%d h=%d d=%d i=%x\n", iWidth, iHeight, iChannels, pRawImage);

    cdf_document* pDoc = cdf_document_create();
    cdf_object* pRoot = cdf_document_root(pDoc);
    cdf_object* pMappingObj = cdf_object_create(pDoc, "mapping", CDF_TYPE_ARRAY);

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

        SpriteBuffer.fX = (float)(yyjson_get_int(yyjson_arr_get(jVal, 0)) / (float)iWidth);
        SpriteBuffer.fY = (float)(yyjson_get_int(yyjson_arr_get(jVal, 1)) / (float)iHeight);
        SpriteBuffer.fW = (float)(yyjson_get_int(yyjson_arr_get(jVal, 2)) / (float)iWidth);
        SpriteBuffer.fH = (float)(yyjson_get_int(yyjson_arr_get(jVal, 3)) / (float)iHeight);
        SpriteBuffer.iNameID = cdf_nametable_push(pDoc, sName);

        sdk_cdf_validate( cdf_array_data_push_ex(pMappingObj, &SpriteBuffer, sizeof(sdk_Sprite), CDF_TYPE_BINARY) )
    }

    sdk_cdf_validate( cdf_object_push(pRoot, pMappingObj) )
    sdk_cdf_validate( cdf_push_binary(pDoc, pRoot, "raster", pRawImage, iWidth*iHeight*iChannels) )

    FILE* hFile = fopen(g_sOutputPath, "wb");
    if(hFile == NULL) {
        sdk_errorf("Unable to open output file '%s'", g_sOutputPath);
    }
    
    sdk_cdf_validate( cdf_file_write(hFile, pDoc, CDF_CLASS_SPRITESHEET, 1) )
    fclose(hFile);
    cdf_document_destroy(pDoc);
    cdf_object_destroy(NULL, pMappingObj);

    SOIL_free_image_data(pRawImage);
}

void do_automap() {
}

int main(int argc, char** argv) {
    char* sMode = NULL;

    char c;
    while( (c = getopt(argc, argv, g_sArgs)) != -1 ) {
        const char* sOpt = optarg;
        switch ( c ) {
            case 'm': sMode = optarg; break;
            case 'h': printf("%s\n", g_sHelpMsg); exit(EXIT_SUCCESS);
            case 'f': g_sFilePath = sOpt; g_bHasFile = 1; break;
            case 'x': g_iX = atoi(sOpt); break;
            case 'y': g_iY = atoi(sOpt); break;
            case 'X': g_iW = atoi(sOpt); break;
            case 'Y': g_iH = atoi(sOpt); break;
            case 'r': g_iRow = atoi(sOpt); break;
            case 'n': g_iNumRows = atoi(sOpt); break;
            case 'o': g_sOutputPath = optarg; break;
        }
    }

    if(sMode == NULL) {
        printf("WARNING: No mode selected, pretending to be in the compiling mode\n");
        sMode = "comp";
    }

    if( strcmp("comp", sMode) == 0 ) {
        if(!g_bHasFile) { sdk_error("No input file providen"); }
        compile_sheet();
    }else if( strcmp("amap", sMode) == 0 ) {

    }else{
        sdk_errorf("Unknown mode: '%s'", sMode);
    }

    return EXIT_SUCCESS;
}
