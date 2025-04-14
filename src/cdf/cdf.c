#include "cdf/cdf.h"

#include <string.h>
#include <stdlib.h>

const char* cdf_get_error(cdf_retcode iCode) {
    switch( iCode ) {
        CDF_ENAME(CDF_OK)
        CDF_ENAME(CDF_ALLOC_FAILURE)
        CDF_ENAME(CDF_NON_ITERABLE)
        CDF_ENAME(CDF_TYPE_UNSUPPORTED)
        CDF_ENAME(CDF_UNEXPECTED_EOF)
        CDF_ENAME(CDF_OUT_OF_BOUNDS)
        CDF_ENAME(CDF_NOT_AN_ARRAY)
        CDF_ENAME(CDF_TYPE_MISMATCH)
        CDF_ENAME(CDF_NO_COMPRESSION)
        CDF_ENAME(CDF_ZLIB_ERROR)

        default: return "UNKNOWN";
    }
}

int cdf_type_primitive(cdf_type iType) {
    return iType < CDF_TYPE_STRING;
}

int cdf_type_iterable(cdf_type iType) {
    return iType == CDF_TYPE_OBJECT;
}

size_t cdf_sizeof(cdf_type iType) {
    switch (iType) {
        case CDF_TYPE_INT:
            return sizeof(int32_t);
            break;
        
        case CDF_TYPE_UINT:
            return sizeof(uint32_t);
            break;

        case CDF_TYPE_FLOAT:
            return sizeof(float);
            break;

        case CDF_TYPE_DOUBLE:
            return sizeof(double);

        case CDF_TYPE_VECTOR:
            return sizeof(float)*2;

        default:
            return 0;
    }
}

size_t cdf_object_sizeof(cdf_object* pObj) {
    CDF_CHECK( if(pObj == NULL) { return 0; } )
    return CDF_HEADER_SIZE + pObj->m_iLength;
}

cdf_document* cdf_document_create() {
    cdf_document* pDoc = (cdf_document*) malloc( sizeof(cdf_document) );
    pDoc->m_iNames = 0;
    pDoc->m_aNames = NULL;

    cdf_retcode iCode = cdf_object_init_ex(pDoc, &pDoc->m_Root, CDF_NAME_ROOT, 0, CDF_TYPE_OBJECT);
    if(iCode != CDF_OK) { return NULL; }

    return pDoc;
}

cdf_uint cdf_nametable_push(cdf_document* pDoc, const char* sName) {
    CDF_CHECK( if(pDoc == NULL) { return 0; } )

    if(sName == NULL) { return 0; }
    
    cdf_uint iIndex = cdf_nametable_find(pDoc, sName);

    if(iIndex != 0) {
        return iIndex;
    }

    iIndex = cdf_nametable_find(pDoc, NULL);
    if(iIndex != 0) {
        pDoc->m_aNames[iIndex] = CDF_COPY(sName);
        return iIndex;
    }

    pDoc->m_iNames++;
    char** pTemp = (char**) realloc(pDoc->m_aNames, sizeof(char*) * (pDoc->m_iNames));
    if(pTemp == NULL) { return 0; }
    pDoc->m_aNames = pTemp;

    pDoc->m_aNames[pDoc->m_iNames-1] = CDF_COPY(sName);

    return pDoc->m_iNames-1;
}

cdf_uint cdf_nametable_find(cdf_document* pDoc, const char* sName) {
    CDF_CHECK( if(pDoc == NULL) { return 0; } )

    for(size_t i = 0; i < pDoc->m_iNames; i++) {
        if(sName == NULL && sName == pDoc->m_aNames[i]) {
            return i;
        }

        if( sName != NULL && strcmp(sName, pDoc->m_aNames[i]) == 0 ) {
            return i;
        }
    }

    return 0;
}

cdf_retcode cdf_nametable_remove(cdf_document* pDoc, cdf_uint iNameIndex) {
    CDF_CHECK( 
        if(pDoc == NULL) { return CDF_INV_INPUT; }
        if(iNameIndex > pDoc->m_iNames || iNameIndex == 0) { return CDF_INV_INPUT; }
    )

    if( pDoc->m_aNames[iNameIndex] == NULL ) { return CDF_OK; }

    free(pDoc->m_aNames[iNameIndex]);
    pDoc->m_aNames[iNameIndex] = NULL;

    return CDF_OK;
}

cdf_retcode cdf_object_init_ex(cdf_document* pDoc, cdf_object* pObj, const char* sName, size_t iSize, cdf_type iType) {
    CDF_CHECK( if(pDoc == NULL) { return CDF_INV_INPUT; } )

    pObj->m_iLength = iSize;
    pObj->m_iNameID = cdf_nametable_push(pDoc, sName);
    pObj->m_iType = iType;

    if(iSize != 0) {
        pObj->m_pData = malloc(iSize);
        if(pObj->m_pData == NULL) { return CDF_ALLOC_FAILURE; }
    }else{
        pObj->m_pData = NULL;
    }

    return CDF_OK;
}

cdf_object* cdf_object_create_ex(cdf_document* pDoc, const char* sName, cdf_uint iSize, cdf_type iType) {
    CDF_CHECK( if(pDoc == NULL) { return NULL; } )

    cdf_object* pObj = (cdf_object*) malloc( sizeof(cdf_object) );
    cdf_object_init_ex(pDoc, pObj, sName, iSize, iType);

    return pObj;
}

void cdf_object_destroy_ex(cdf_document* pDoc, cdf_object* pObj, int bStack) {
    CDF_CHECK( if(pDoc == NULL || pObj == NULL) { return; } )

    if(pObj->m_iNameID != 0 || pDoc != NULL) {
        cdf_nametable_remove(pDoc, pObj->m_iNameID);
    }

    if(pObj->m_iLength != 0 && pObj->m_pData != NULL) {
        free(pObj->m_pData);
    }

    if(bStack == 0) { free(pObj); }
}

cdf_retcode cdf_object_unwrap(cdf_object* pObj, void* pTarget) {
    CDF_CHECK( if(pObj == NULL || pTarget == NULL) { return CDF_INV_INPUT; } )

    memcpy(pTarget, pObj, CDF_HEADER_SIZE);
    memcpy(pTarget+CDF_HEADER_SIZE, pObj->m_pData, pObj->m_iLength);

    return CDF_OK;
}

cdf_retcode cdf_object_push(cdf_object* pParent, cdf_object* pChild) {
    CDF_CHECK( if(pParent == NULL || pChild == NULL) { return CDF_INV_INPUT; } )
    CDF_CHECK( if(!cdf_type_iterable(pParent->m_iType)) { return CDF_NON_ITERABLE; } )

    size_t iChildSize = cdf_object_sizeof(pChild);
    size_t iOldLength = pParent->m_iLength;

    cdf_retcode iCode = cdf_object_realloc(pParent, pParent->m_iLength + iChildSize);

    return cdf_object_unwrap(pChild, pParent->m_pData + iOldLength);
}

cdf_retcode cdf_data_push_ex(cdf_document* pDoc, cdf_object* pParent, const char* sName, void* pData, size_t iDataLen, cdf_type iDataType) {
    CDF_CHECK( if(pParent == NULL || pData == NULL) { return CDF_INV_INPUT; } )

    cdf_object hObj;
    cdf_retcode iCode = cdf_object_init_ex(pDoc, &hObj, sName, 0, iDataType);
    if(iCode != CDF_OK) { return iCode; }
    hObj.m_iLength = iDataLen;

    size_t iChildLength = CDF_HEADER_SIZE + iDataLen;
    size_t iOldLength = pParent->m_iLength;
    iCode = cdf_object_realloc(pParent, pParent->m_iLength + iChildLength);

    if(iCode != CDF_OK) { return iCode; }

    memcpy(pParent->m_pData + iOldLength, &hObj, CDF_HEADER_SIZE);
    memcpy(pParent->m_pData + iOldLength + CDF_HEADER_SIZE, pData, iDataLen);

    return CDF_OK;
}

int cdf_object_iterate(cdf_object* pParent, cdf_object* ppCurrent, size_t* pIter) {
    CDF_CHECK( if(pParent == NULL || ppCurrent == NULL) { return 0; } )
    CDF_CHECK( if(!cdf_type_iterable(pParent->m_iType)) { return 0; } )

    cdf_object* pCurrent = (cdf_object*)( pParent->m_pData + (*pIter) );
    memcpy(ppCurrent, pCurrent, CDF_HEADER_SIZE);
    ppCurrent->m_pData = pCurrent + CDF_HEADER_SIZE;

    (*pIter) += cdf_object_sizeof(pCurrent);
    if((*pIter) > pParent->m_iLength) { return 0; }
    return 1;
}

#ifdef CDF_NAMETABLE_COPY
void cdf_nametable_free(char** pTable, cdf_uint iTableLen) {
    if(pTable != NULL) {
        for(size_t i = 1; i < iTableLen; i++) {
            if(pTable[i] != NULL) { free(pTable[i]); }
        }
        free(pTable);
    }
}
#endif

cdf_retcode cdf_nametable_write(FILE* hFile, char** aTable, cdf_uint iNum) {
    CDF_CHECK( if(feof(hFile) || aTable == NULL || iNum == 0) { return CDF_INV_INPUT; } )

    for(size_t i = 1; i < iNum; i++) {
        fwrite(aTable[i], 1, strlen(aTable[i])+1, hFile);
    }

    return CDF_OK;
}

cdf_retcode cdf_document_write(FILE* hFile, cdf_document* pDoc) {
    CDF_CHECK( if(hFile == NULL) { return CDF_INV_INPUT; } )
    CDF_CHECK( if(pDoc == NULL) { return CDF_INV_INPUT; } )

    fwrite(pDoc, sizeof(pDoc->m_iNames), 1, hFile);

    cdf_retcode iCode = cdf_nametable_write(hFile, pDoc->m_aNames, pDoc->m_iNames);
    if(iCode != CDF_OK) { return iCode; }

    fwrite(&pDoc->m_Root, CDF_HEADER_SIZE, 1, hFile);
    fwrite(pDoc->m_Root.m_pData, 1, pDoc->m_Root.m_iLength, hFile);

    return CDF_OK;
}

cdf_retcode cdf_nametable_read(FILE* hFile, char*** pTable, cdf_uint iNames) {
    CDF_CHECK( if(feof(hFile) || pTable == NULL) { return CDF_INV_INPUT; } )

    char** aNames = (char**) malloc(sizeof(char*) * iNames);
    if(aNames == NULL) { return CDF_ALLOC_FAILURE; }
    memset(aNames, 0, sizeof(char*)*iNames);

    aNames[0] = CDF_NAME_ERROR;
    for(size_t i = 1; i < iNames; i++) {
        char cCurrent;
        size_t j = 0, iSize = 64;
        aNames[i] = (char*) malloc(iSize);

        if(aNames[i] == NULL) { cdf_nametable_free(aNames, iNames); return CDF_ALLOC_FAILURE; }

        while( fread(&cCurrent, 1, 1, hFile) ) {
            if(feof(hFile)) {
                cdf_nametable_free(aNames, iNames);
                return CDF_UNEXPECTED_EOF;
            }

            if(j == iSize) {
                iSize = iSize*2;

                char* pTemp = (char*) realloc(aNames[i], iSize);
                if(pTemp == NULL) {
                    cdf_nametable_free(aNames, iNames);
                    return CDF_ALLOC_FAILURE;
                }
                aNames[i] = pTemp;
            }

            aNames[i][j] = cCurrent;
            if(cCurrent == '\0') { break; }

            j++;
        }

        //shrink the buffer to match the actual data length
        aNames[i] = (char*) realloc(aNames[i], j);
    }

    (*pTable) = aNames;
    return CDF_OK;
}

cdf_retcode cdf_document_read(FILE* hFile, cdf_document** ppDoc) {
    CDF_CHECK( if(hFile == NULL) { return NULL; } )

    uint32_t iNames = 0;
    char** aNames;

    int bCompressed = 0;
    if( fread(&iNames, sizeof(iNames), 1, hFile) != 1 ) { return CDF_UNEXPECTED_EOF; }

    cdf_retcode iCode = cdf_nametable_read(hFile, &aNames, iNames);
    if(iCode != CDF_OK) { return iCode; }

    cdf_document* pDoc = (cdf_document*) malloc(sizeof(cdf_document));
    if(pDoc == NULL) {
        cdf_nametable_free(aNames, iNames);
        return CDF_ALLOC_FAILURE;
    }

    pDoc->m_iNames = iNames;
    pDoc->m_aNames = aNames;

    if( fread(&pDoc->m_Root, CDF_HEADER_SIZE, 1, hFile) != 1 ) {
        cdf_nametable_free(aNames, iNames);
        free(pDoc);
        return CDF_UNEXPECTED_EOF;
    }

    pDoc->m_Root.m_pData = (uint8_t*) malloc(pDoc->m_Root.m_iLength);
    if(pDoc->m_Root.m_pData == NULL) {
        cdf_nametable_free(aNames, iNames);
        free(pDoc);
        return CDF_UNEXPECTED_EOF;
    }

    if( fread(pDoc->m_Root.m_pData, pDoc->m_Root.m_iLength, 1, hFile) != 1 ) {
        cdf_nametable_free(aNames, iNames);
        free(pDoc->m_Root.m_pData);
        free(pDoc);
        return CDF_UNEXPECTED_EOF;
    }

    (*ppDoc) = pDoc;
    return CDF_OK;
}

cdf_retcode cdf_object_realloc(cdf_object* pObj, size_t iNewLength) {
    CDF_CHECK( if(pObj == NULL) { return CDF_INV_INPUT; } )

    void* pTemp = realloc( pObj->m_pData, iNewLength );
    if(pTemp == NULL) { return CDF_ALLOC_FAILURE; }
    pObj->m_pData = pTemp;
    pObj->m_iLength = iNewLength;

    return CDF_OK;
}

cdf_retcode cdf_array_index(cdf_object* pObj, cdf_object* pTarget, cdf_uint iIndex) {
    CDF_CHECK( if(pObj == NULL || pTarget == NULL) { return CDF_INV_INPUT; } )
    if(pObj->m_iType != CDF_TYPE_ARRAY) { return CDF_NOT_AN_ARRAY; }
    if(pObj->m_iLength == 0) { return CDF_INV_INPUT; }

    if( iIndex > cdf_array_length(pObj) ) { return CDF_OUT_OF_BOUNDS; }

    cdf_object* pFirst = (cdf_object*)(pObj->m_pData);
    memcpy(pTarget, pFirst, CDF_HEADER_SIZE);
    pTarget->m_pData = (void*)pFirst + CDF_HEADER_SIZE + iIndex*pFirst->m_iLength;

    return CDF_OK;
}

cdf_retcode cdf_array_data_push_ex(cdf_object* pObj, void* pData, size_t iDataLen, cdf_type iDataType) {
    CDF_CHECK( if(pObj == NULL) { return CDF_INV_INPUT; } )

    //Arrays store only the header of the first member
    if( pObj->m_iLength == 0 ) {
        return cdf_data_push_ex(NULL, pObj, NULL, pData, iDataLen, iDataType);
    }

    cdf_object* pFirst = (cdf_object*)( pObj->m_pData );
    if(iDataLen != pFirst->m_iLength) { return CDF_TYPE_MISMATCH; }

    if(iDataLen == 0) { iDataLen = pFirst->m_iLength; }
    if(iDataType == 0) { iDataType = pFirst->m_iType; }

    size_t iOldLength = pObj->m_iLength;
    cdf_retcode iCode = cdf_object_realloc( pObj, pObj->m_iLength + iDataLen );
    if(iCode != CDF_OK) { return iCode; }

    memcpy(pObj->m_pData + iOldLength, pData, iDataLen);
    return CDF_OK;
}

cdf_retcode cdf_array_push_object(cdf_object* pParent, cdf_object* pChild) {
    CDF_CHECK( if(pParent == NULL) { return CDF_INV_INPUT; } )
    //return cdf_array_data_push_ex(pParent, pChild, cdf_object_sizeof(pChild), );
}

__cdf_arrpush_func(int32_t, CDF_TYPE_INT, int)
__cdf_arrpush_func(uint32_t, CDF_TYPE_UINT, uint)
__cdf_arrpush_func(float, CDF_TYPE_FLOAT, float)
__cdf_arrpush_func(double, CDF_TYPE_DOUBLE, double)
__cdf_arrpush_func(cdf_vector, CDF_TYPE_VECTOR, vector)

cdf_uint cdf_array_length(cdf_object* pObj) {
    CDF_CHECK( if(pObj == NULL) { return 0; } )
    cdf_object* pFirst = (cdf_object*)(pObj->m_pData);
    return (pObj->m_iLength - CDF_HEADER_SIZE) / pFirst->m_iLength;
}

cdf_type cdf_object_type(cdf_object* pObj) {
    CDF_CHECK( if(pObj == NULL) { return CDF_TYPE_INVALID; } )
    return pObj->m_iType;
}

cdf_uint cdf_object_length(cdf_object* pObj) {
    CDF_CHECK( if(pObj == NULL) { return 0; } )
    return pObj->m_iLength;
}

void* cdf_object_data(cdf_object* pObj) {
    CDF_CHECK( if(pObj == NULL) { return NULL; } )
    return pObj->m_pData;
}

__cdf_push_func(int32_t, CDF_TYPE_INT, int)
__cdf_push_func(uint32_t, CDF_TYPE_UINT, uint)
__cdf_push_func(float, CDF_TYPE_FLOAT, float)
__cdf_push_func(double, CDF_TYPE_DOUBLE, double)
__cdf_push_func(cdf_vector, CDF_TYPE_VECTOR, vector)

__cdf_get_func(int32_t, CDF_TYPE_INT, int)
__cdf_get_func(uint32_t, CDF_TYPE_UINT, uint)
__cdf_get_func(float, CDF_TYPE_FLOAT, float)
__cdf_get_func(double, CDF_TYPE_DOUBLE, double)
__cdf_get_func(cdf_vector, CDF_TYPE_VECTOR, vector)

__cdf_is_func(CDF_TYPE_INT, int)
__cdf_is_func(CDF_TYPE_UINT, uint)
__cdf_is_func(CDF_TYPE_FLOAT, float)
__cdf_is_func(CDF_TYPE_DOUBLE, double)
__cdf_is_func(CDF_TYPE_VECTOR, vector)
__cdf_is_func(CDF_TYPE_BINARY, binary)
__cdf_is_func(CDF_TYPE_STRING, string)
__cdf_is_func(CDF_TYPE_OBJECT, object)

int cdf_is_array(cdf_object* pObj) {
    if(pObj->m_iType != CDF_TYPE_ARRAY) {
        return 0;
    }
    //The array is empty so we can`t verify it`s data integrity
    if(pObj->m_iLength == 0) { return 1; }

    cdf_object* pFirst = (cdf_object*)(pObj->m_pData);
    return pObj->m_iLength % cdf_object_sizeof(pFirst) == 0;
    //This check can possibly give false positive results, but it should work
    //properly in the most scenarios
}

cdf_retcode cdf_push_binary(cdf_document* pDoc, cdf_object* pParent, const char* sName, uint8_t* Value, size_t iLen) {
    return cdf_data_push_ex(pDoc, pParent, sName, (void*)Value, iLen, CDF_TYPE_BINARY);
}

cdf_retcode cdf_push_string(cdf_document* pDoc, cdf_object* pParent, const char* sName, const char* sValue) {
    return cdf_data_push_ex(pDoc, pParent, sName, (void*)sValue, strlen(sValue), CDF_TYPE_STRING);
}

cdf_retcode cdf_version_write(FILE* hFile) {
    CDF_CHECK( if(hFile == NULL) { return CDF_INV_INPUT; } )

    static cdf_verinfo Ver;
    Ver.m_iIntSize = CDF_INT_SIZE;
    Ver.m_iVersionMajor = CDF_VERSION_MAJOR;
    Ver.m_iVersionMinor = CDF_VERSION_MINOR;
    fwrite(&Ver, sizeof(Ver), 1, hFile);
    return CDF_OK;
}

cdf_retcode cdf_version_read(FILE* hFile, cdf_verinfo* pTarget) {
    CDF_CHECK( if(hFile == NULL || pTarget == NULL) { return CDF_INV_INPUT; } )

    if( fread(pTarget, sizeof(cdf_verinfo), 1, hFile) != 1 ) { return CDF_UNEXPECTED_EOF; }
    return CDF_OK;
}

int cdf_check_header(FILE* hFile) {
    CDF_CHECK( if(hFile == NULL) { return 0; } )

    uint32_t iFileVer;
    if(fread(&iFileVer, 4, 1, hFile) != 1) { return 0; }

    return iFileVer == CDF_FILE_MARK;
}

#ifdef CDF_USE_ZLIB
cdf_retcode cdf_object_compress(cdf_object* pObj, void* pTarget, size_t* iCompressedSize) {
    CDF_CHECK( if(pObj == NULL) { return CDF_INV_INPUT; } )

    size_t iSize = cdf_object_sizeof(pObj);
    (*iCompressedSize) = iSize;
    void* pBuffer = malloc(iSize);
    if(pBuffer == NULL) {
        return CDF_ALLOC_FAILURE;
    }

    cdf_retcode iCode = cdf_object_unwrap(pObj, pBuffer);
    if(iCode != CDF_OK) { free(pBuffer); return iCode; }

    int iZlibCode = compress(pTarget, iCompressedSize, pBuffer, iSize);
    if(iZlibCode != Z_OK) { free(pBuffer); return CDF_ZLIB_ERROR; }

    return CDF_OK;
}

cdf_retcode cdf_object_decompress(cdf_object* pTarget, void* pSource, size_t iSourceLen) {
    CDF_CHECK( if(pTarget == NULL) { return CDF_INV_INPUT; } )

    
}
#endif
