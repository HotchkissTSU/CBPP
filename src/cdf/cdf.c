#include "cdf/cdf.h"
#include <stdlib.h>

size_t cdf_sizeof(CDF_OBJECT_TYPE iType) {
    switch (iType) {
        case CDF_TYPE_INT:
            return sizeof(int32_t);
        case CDF_TYPE_UINT:
            return sizeof(uint32_t);
        case CDF_TYPE_FLOAT:
            return sizeof(float);
        case CDF_TYPE_VECTOR:
            return sizeof(float)*2;
        
        default:
            return 0;
    }
}

bool cdf_type_complex(CDF_OBJECT_TYPE iType) {
    return (iType == CDF_TYPE_ARRAY || iType == CDF_TYPE_STRING || iType == CDF_TYPE_OBJECT);
}

size_t cdf_object_sizeof(cdf_object* pObj) {
    CDF_CHECK( if(pObj == NULL) { return 0; } )
    return pObj->m_iLength + offsetof(cdf_object, m_uData);
}

cdf_document* cdf_document_new() {
    cdf_document* pDoc = (cdf_document*) malloc(sizeof(cdf_document));
    pDoc->m_iNames = 1;
    pDoc->m_aNames = (char**) malloc(sizeof(char*));
    pDoc->m_aNames[0] = CDF_ERRORNAME;

    cdf_object_init(pDoc, &pDoc->m_hRoot, "root", CDF_TYPE_OBJECT);

    return pDoc;
}

cdf_object* cdf_document_root(cdf_document* pDoc) {
    CDF_CHECK( if(pDoc == NULL) { return; } )
    return &pDoc->m_hRoot;
}

void cdf_document_destroy(cdf_document** ppDoc) {
    cdf_document* pDoc = *ppDoc;

    CDF_CHECK( if(pDoc == NULL) { return; } )

    for(size_t i = 1; i < pDoc->m_iNames; i++) {
        if(pDoc->m_aNames[i] != NULL) {
            free(pDoc->m_aNames[i]);
        }
    }

    cdf_object* pObj = &pDoc->m_hRoot;

    if(pObj->m_iType == CDF_TYPE_ARRAY || pObj->m_iType == CDF_TYPE_OBJECT) {
        if(pObj->m_uData.objinfo != NULL) {
            free(pObj->m_uData.objinfo);
        }
    }

    free(pDoc);
    (*ppDoc) = NULL;
}

uint32_t cdf_nametable_push(cdf_document* pDoc, const char* sName) {
    CDF_CHECK( if(pDoc == NULL || sName == NULL) { return 0; } )
    
    uint32_t iIndex = cdf_nametable_find(pDoc, sName);

    if(iIndex != 0) {
        return iIndex;
    }

    iIndex = cdf_nametable_find(pDoc, NULL);
    if(iIndex != 0) {
        pDoc->m_aNames[iIndex] = strdup(sName);
        return iIndex;
    }

    pDoc->m_iNames++;
    char** pTemp = (char**) realloc(pDoc->m_aNames, sizeof(char*) * (pDoc->m_iNames));
    if(pTemp == NULL) { return 0; } //no free space, up-allocating
    pDoc->m_aNames = pTemp;

    pDoc->m_aNames[pDoc->m_iNames-1] = strdup(sName);

    return pDoc->m_iNames-1;
}

uint32_t cdf_nametable_find(cdf_document* pDoc, const char* sName) {
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

bool cdf_nametable_remove(cdf_document* pDoc, uint32_t iNameIndex) {
    CDF_CHECK( 
        if(pDoc == NULL) { return false; }
        if(iNameIndex > pDoc->m_iNames || iNameIndex == 0) { return false; }
    )

    if( pDoc->m_aNames[iNameIndex] == NULL ) { return false; }

    free(pDoc->m_aNames[iNameIndex]);
    pDoc->m_aNames[iNameIndex] = NULL;

    return true;
}

bool cdf_nametable_remove_name(cdf_document* pDoc, const char* sName) {
    CDF_CHECK( if(sName == NULL) { return false; } )

    uint32_t iIndex = cdf_nametable_find(pDoc, sName);
    if(iIndex != 0) {
        return cdf_nametable_remove(pDoc, iIndex);
    }

    return false;
}

void cdf_object_init(cdf_document* pDoc, cdf_object* pObj, const char* sName, CDF_OBJECT_TYPE iType) {
    CDF_CHECK( if(pObj == NULL) { return; } )

    pObj->m_iType = iType;
    pObj->m_iLength = cdf_sizeof(iType);

    if(pDoc != NULL) {
        pObj->m_iNameID = cdf_nametable_push(pDoc, sName);
    }else{
        pObj->m_iNameID = 0;
    }

    memset(&pObj->m_uData, 0, sizeof(pObj->m_uData));
}

bool cdf_object_iterate(cdf_object* pObj, cdf_iterator* pIter) {
    CDF_CHECK( if(pObj == NULL || pIter == NULL) { return false; } )

    if(pObj->m_iType != CDF_TYPE_ARRAY && pObj->m_iType != CDF_TYPE_OBJECT) { return false; }

    cdf_object* pCurrent = (cdf_object*)(pObj->m_uData.objinfo + pIter->m_iIter);
    memcpy(pIter, pCurrent, CDF_HEADER_SIZE);

    pIter->m_pData = cdf_object_data_from_iter(pObj, pIter->m_iIter);
    pIter->m_iIter += cdf_object_sizeof(pCurrent);

    if(pIter->m_iIter > pObj->m_iLength) { return false; }

    return true;
}

uint8_t* cdf_object_data_from_iter(cdf_object* pParent, size_t iIter) {
    CDF_CHECK( if(pParent == NULL) { return NULL; } )
    return (uint8_t*)(pParent->m_uData.objinfo + iIter + sizeof(cdf_object) - sizeof(pParent->m_uData));
}

cdf_object* cdf_object_new(cdf_document* pDoc, const char* sName, CDF_OBJECT_TYPE iType) {
    cdf_object* pObj = (cdf_object*) malloc(sizeof(cdf_object));
    cdf_object_init(pDoc, pObj, sName, iType);
    return pObj;
}

bool cdf_object_copy(cdf_object* pObj, uint8_t* pTarget) {
    CDF_CHECK( if(pObj == NULL || pTarget == NULL) { return false; } )

    if( cdf_type_complex(pObj->m_iType) ) {
        size_t iStructSize = offsetof(cdf_object, m_uData);
        memcpy(pTarget, pObj, iStructSize);
        memcpy(pTarget + iStructSize, pObj->m_uData.objinfo, pObj->m_iLength);
    }else{
        memcpy(pTarget, pObj, cdf_object_sizeof(pObj));
    }

    return true;
}

bool cdf_object_push(cdf_object* pParent, cdf_object* pChild) {
    CDF_CHECK(
        if(pParent == NULL || pChild == NULL) { return false; }
        if(pParent->m_iType != CDF_TYPE_OBJECT) { return false; }
    )

    size_t iChildSize = cdf_object_sizeof(pChild);

    uint8_t* pTemp = (uint8_t*) realloc(pParent->m_uData.objinfo, pParent->m_iLength + iChildSize);
    if(pTemp == NULL) {
        return false;
    }
    pParent->m_uData.objinfo = pTemp;

    if(!cdf_object_copy(pChild, pParent->m_uData.objinfo + pParent->m_iLength)) { return false; }
    pParent->m_iLength += iChildSize;

    return true;
}

bool cdf_object_by_nid(cdf_object* pParent, cdf_iterator* pTarget, uint32_t iNameIndex) {
    CDF_CHECK( if(pParent == NULL || pTarget == NULL) { return false; } )

    cdf_iterator Iter;
    cdf_iterator_init(&Iter);

    while( cdf_object_iterate(pParent, &Iter) ) {
        if(Iter.m_iNameID == iNameIndex) {
            *pTarget = Iter;
            return true;
        }
    }

    return false;
}

bool cdf_object_by_name(cdf_document* pDoc, cdf_object* pParent, cdf_iterator* pTarget, const char* sName) {
    CDF_CHECK( if(pDoc == NULL || pParent == NULL || sName == NULL) { return false; } )

    uint32_t iNameIndex = cdf_nametable_find(pDoc, sName);
    if(iNameIndex == 0) { return false; }

    return cdf_object_by_nid(pParent, pTarget, iNameIndex);
}

bool cdf_object_by_index(cdf_object* pParent, cdf_iterator* pTarget, uint32_t iIndex) {
    CDF_CHECK(
        if(pParent == NULL) { return NULL; }
        if(pParent->m_iType != CDF_TYPE_ARRAY) { return NULL; }
        if(pParent->m_iLength == 0 || pParent->m_uData.objinfo == NULL) { return NULL; }
    )

    cdf_object* pFirst = (cdf_object*)( pParent->m_uData.objinfo );

    pTarget->m_iLength = pFirst->m_iLength;
    pTarget->m_iType = pFirst->m_iType;
    pTarget->m_iNameID = 0;
    pTarget->m_pData = (uint8_t*)( pParent->m_uData.objinfo + cdf_object_sizeof(pFirst)*iIndex );

    return true;
}

cdf_object* cdf_object_from_iter(cdf_object* pParent, cdf_iterator* pIter) {
    CDF_CHECK( if(pParent == NULL || pIter == NULL) { return false; } )

    cdf_object* pObj = (cdf_object*) malloc(sizeof(cdf_object));
    memcpy(pObj, pIter, CDF_HEADER_SIZE);

    if( cdf_type_complex(pIter->m_iType) ) {
        pObj->m_uData.objinfo = (uint8_t*) malloc(pIter->m_iLength);
        memcpy(pObj->m_uData.objinfo, pIter->m_pData, pIter->m_iLength);
    }else{
        memcpy(&pObj->m_uData, pIter->m_pData, pIter->m_iLength);
    }

    return pObj;
}

void cdf_object_destroy(cdf_document* pDoc, cdf_object** pObj, bool bStack) {
    CDF_CHECK( if((*pObj) == NULL) { return; } )

    if(pDoc != NULL) {
        cdf_nametable_remove(pDoc, (*pObj)->m_iNameID);
    }

    if( cdf_type_complex((*pObj)->m_iType) ) {
        if((*pObj)->m_uData.objinfo != NULL) {
            free((*pObj)->m_uData.objinfo);
        }
    }

    if(!bStack) {
        free((*pObj));
        (*pObj) = NULL;
    }
}

void cdf_nametable_free(char** pTable, size_t iTableLen) {
    if(pTable != NULL) {
        for(size_t i = 1; i < iTableLen; i++) {
            if(pTable[i] != NULL) { free(pTable[i]); }
        }
        free(pTable);
    }
}

bool cdf_document_write(FILE* hFile, cdf_document* pDoc) {
    CDF_CHECK( if(hFile == NULL) { return false; } )
    CDF_CHECK( if(pDoc == NULL) { return false; } )

    size_t iStart = ftell(hFile);

    fwrite(pDoc, sizeof(pDoc->m_iNames), 1, hFile);

    for(size_t i = 1; i < pDoc->m_iNames; i++) {
        fwrite(pDoc->m_aNames[i], 1, strlen(pDoc->m_aNames[i])+1, hFile);
    }

    size_t iLen = cdf_object_sizeof(&pDoc->m_hRoot);
    uint8_t* pData = (uint8_t*) malloc(iLen);

    cdf_object_copy(&pDoc->m_hRoot, pData);
    fwrite(pData, 1, iLen, hFile);

    free(pData);
    return true;
}

cdf_document* cdf_document_read(FILE* hFile) {
    CDF_CHECK( if(hFile == NULL) { return NULL; } )

    uint32_t iNames = 0;
    if( fread(&iNames, sizeof(iNames), 1, hFile) != 1 ) { return NULL; }

    char** aNames = (char**) malloc(sizeof(char*) * iNames);
    if(aNames == NULL) { return NULL; }
    memset(aNames, 0, sizeof(char*)*iNames);

    aNames[0] = CDF_ERRORNAME;
    for(size_t i = 1; i < iNames; i++) {
        char cCurrent;
        size_t j = 0, iSize = 64;
        aNames[i] = (char*) malloc(iSize);

        while( fread(&cCurrent, 1, 1, hFile) ) {
            if(j == iSize) {
                iSize = iSize*2;

                char* pTemp = (char*) realloc(aNames[i], iSize);
                if(pTemp == NULL) {
                    cdf_nametable_free(aNames, iNames);
                    return NULL;
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

    cdf_document* pDoc = (cdf_document*) malloc(sizeof(cdf_document));
    if(pDoc == NULL) {
        cdf_nametable_free(aNames, iNames);
        return NULL;
    }

    pDoc->m_iNames = iNames;
    pDoc->m_aNames = aNames;

    if( fread(&pDoc->m_hRoot, sizeof(cdf_object) - sizeof(pDoc->m_hRoot.m_uData), 1, hFile) != 1 ) {
        cdf_nametable_free(aNames, iNames);
        free(pDoc);
        return NULL;
    }

    pDoc->m_hRoot.m_uData.objinfo = (uint8_t*) malloc(pDoc->m_hRoot.m_iLength);
    if(pDoc->m_hRoot.m_uData.objinfo == NULL) {
        cdf_nametable_free(aNames, iNames);
        free(pDoc);
        return NULL;
    }

    if( fread(pDoc->m_hRoot.m_uData.objinfo, pDoc->m_hRoot.m_iLength, 1, hFile) != 1 ) {
        cdf_nametable_free(aNames, iNames);
        free(pDoc->m_hRoot.m_uData.objinfo);
        free(pDoc);
        return NULL;
    }

    return pDoc;
}

bool cdf_push_generic(cdf_document* pDoc, cdf_object* pParent, const char* sName, void* pBinary, size_t iLength, CDF_OBJECT_TYPE iType) {
    CDF_CHECK( if(pParent == NULL) { return false; } )

    cdf_object hObj;
    cdf_object_init(pDoc, &hObj, sName, iType);

    if(iType == CDF_TYPE_ARRAY || iType == CDF_TYPE_BINARY || iType == CDF_TYPE_STRING) {
        hObj.m_uData.objinfo = (uint8_t*) malloc(iLength);
        if(hObj.m_uData.objinfo == NULL) { return false; }
        memcpy(hObj.m_uData.objinfo, pBinary, iLength);
    }else{
        memcpy(&hObj.m_uData, pBinary, iLength);
    }

    cdf_object* pObj = &hObj;
    bool bPush = cdf_object_push(pParent, pObj);
    cdf_object_destroy(NULL, &pObj, true);

    return bPush;
}

bool cdf_get_generic(cdf_object* pParent, void* pTarget, size_t iLength) {
    CDF_CHECK( if(pParent == NULL) { return false; } )
    if(pParent->m_iLength < iLength) { return false; }

    void* pSource;
    if( cdf_type_complex(pParent->m_iType) ) {
        pSource = pParent->m_uData.objinfo;
    }else{
        pSource = &pParent->m_uData;
    }

    memcpy(pTarget, pSource, iLength);
    return true;
}
