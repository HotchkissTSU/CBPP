#ifndef CBPP_CDF_H
#define CBPP_CDF_H

#include "cdf/cdf_macros.h"

#ifdef CDF_USE_ZLIB
#include "zlib.h"
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef enum cdf_retcode {
    CDF_OK,                     //Everything is probably cool
    CDF_ALLOC_FAILURE,          //The allocator has fallen, centurion
    CDF_INV_INPUT,              //One or many input args are NULL
    CDF_NON_ITERABLE,           //Attempt to push a sub-value into a primitive object
    CDF_TYPE_UNSUPPORTED,       //Unknown type ID found while reading
    CDF_UNEXPECTED_EOF,         //That EOF should not be there!
    CDF_OUT_OF_BOUNDS,          //Array index is out of bounds
    CDF_NOT_AN_ARRAY,           //Attempt to index an object that is not an array
    CDF_TYPE_MISMATCH,          //Types mismatch during data extraction
    CDF_NO_COMPRESSION,         //This build of libCDF does not support built-in compression
    CDF_ZLIB_ERROR
} cdf_retcode;

typedef enum cdf_type {
    CDF_TYPE_INVALID,   //A special error value

    CDF_TYPE_INT,       //cdf_int, int32_t by default
    CDF_TYPE_UINT,      //cdf_uint, uint32_t by default
    CDF_TYPE_FLOAT,     //32-bit floating point value
    CDF_TYPE_DOUBLE,    //64-bit floating point value
    CDF_TYPE_VECTOR,    //Two floats combined together

    CDF_TYPE_STRING,    //An UTF-8 encoded string without a null-terminator
    CDF_TYPE_BINARY,    //Any binary sequence

    CDF_TYPE_OBJECT,    //An object with sub-objects in it
    CDF_TYPE_ARRAY,     //An object that stores any amount of identical sub-objects

    CDF_TYPES_AMOUNT
} cdf_type;

typedef struct cdf_vector {
    float m_fX, m_fY;
} cdf_vector;

typedef struct cdf_object {
    cdf_uint m_iLength;
    uint8_t m_iType;
    cdf_uint m_iNameID;
    void* m_pData;
} __attribute__((packed)) cdf_object;

typedef struct cdf_document {
    cdf_uint m_iNames;
    char** m_aNames;
    cdf_object m_Root;

    #ifdef CDF_USE_ZLIB
    struct {
        uint8_t m_bCompressed;
        size_t m_iOrigSize;
    } m_CompressionInfo;
    #endif
} cdf_document;

typedef struct cdf_verinfo {
    uint16_t m_iVersionMajor, m_iVersionMinor;
    uint8_t m_iIntSize;
} __attribute__((packed)) cdf_verinfo;

//Get a string description of the chosen error code
const char* cdf_get_error(cdf_retcode iCode);

//Check if selected type lies before the CDF_TYPE_STRING in the types list
int cdf_type_primitive(cdf_type iType);

//Check if this type can be iterated (ARRAY and OBJECT)
int cdf_type_iterable(cdf_type iType);

//Get a size of a primitive value type
size_t cdf_sizeof(cdf_type iType);

//Get a size needed to unwrap this object
size_t cdf_object_sizeof(cdf_object* pObj);

cdf_type cdf_object_type(cdf_object* pObj);

//Get object`s length in bytes
cdf_uint cdf_object_length(cdf_object* pObj);
void* cdf_object_data(cdf_object* pObj);

cdf_document* cdf_document_create();
cdf_retcode cdf_document_destroy(cdf_document* pDoc);

cdf_retcode cdf_document_write(FILE* hFile, cdf_document* pDoc);
cdf_retcode cdf_document_read(FILE* hFile, cdf_document** ppDoc);

cdf_retcode cdf_version_write(FILE* hFile);
cdf_retcode cdf_version_read(FILE* hFile, cdf_verinfo* pTarget);

int cdf_check_header(FILE* hFile);

cdf_retcode cdf_nametable_write(FILE* hFile, char** aTable, cdf_uint iNum);
cdf_retcode cdf_nametable_read(FILE* hFile, char*** pTable, cdf_uint iNum);

cdf_uint cdf_nametable_push(cdf_document* pDoc, const char* sName);
cdf_uint cdf_nametable_find(cdf_document* pDoc, const char* sName);
cdf_retcode cdf_nametable_remove(cdf_document* pDoc, cdf_uint iNameIndex);

//Write an object to a plain buffer
cdf_retcode cdf_object_unwrap(cdf_object* pObj, void* pTarget);

cdf_retcode cdf_object_realloc(cdf_object* pObj, size_t iNewLength);

//Initialize an object thet already exists
cdf_retcode cdf_object_init_ex(cdf_document* pDoc, cdf_object* pObj, const char* sName, size_t iSize, cdf_type iType);

//Allocate and initialize a new object
cdf_object* cdf_object_create_ex(cdf_document* pDoc, const char* sName, cdf_uint iSize, cdf_type iType);
void cdf_object_destroy_ex(cdf_document* pDoc, cdf_object* pObj, int bStackAllocated);

//Append a sub-object
cdf_retcode cdf_object_push(cdf_object* pParent, cdf_object* pChild);

//Append a primitive data field
cdf_retcode cdf_data_push_ex(cdf_document* pDoc, cdf_object* pParent, const char* sName, void* pData, size_t iDataLen, cdf_type iDataType);

//Access an array member by it`s index
cdf_retcode cdf_array_index(cdf_object* pObj, cdf_object* pTarget, cdf_uint iIndex);
cdf_uint cdf_array_length(cdf_object* pObj);

/*
    Push some data into an array
*/
cdf_retcode cdf_array_data_push_ex(cdf_object* pObj, void* pData, size_t iDataLen, cdf_type iDataType);

/*
    Iterate an object. The iterator variable must be zeroed beforehand!
*/
int cdf_object_iterate(cdf_object* pParent, cdf_object* pCurrent, size_t* pIter);

#ifdef CDF_NAMETABLE_COPY
    void cdf_nametable_free(char** aTable, cdf_uint iLen);
#else
    #define cdf_nametable_free(p, i)
#endif

#ifdef CDF_USE_ZLIB
    cdf_retcode cdf_object_compress(cdf_object* pObj, void* pTarget, size_t* iCompressedSize);
    cdf_retcode cdf_object_decompress(cdf_object* pTarget, void* pSource, size_t iSourceLen);
#else
    #define cdf_object_compress(o, p, i) CDF_NO_COMPRESSION
    #define cdf_object_decompress(o, p, i) CDF_NO_COMPRESSION
#endif

__cdf_push_func_decl(int32_t, CDF_TYPE_INT, int)
__cdf_push_func_decl(uint32_t, CDF_TYPE_UINT, uint)
__cdf_push_func_decl(float, CDF_TYPE_FLOAT, float)
__cdf_push_func_decl(double, CDF_TYPE_DOUBLE, double)
__cdf_push_func_decl(cdf_vector, CDF_TYPE_VECTOR, vector)

__cdf_arrpush_func_decl(int32_t, CDF_TYPE_INT, int)
__cdf_arrpush_func_decl(uint32_t, CDF_TYPE_UINT, uint)
__cdf_arrpush_func_decl(float, CDF_TYPE_FLOAT, float)
__cdf_arrpush_func_decl(double, CDF_TYPE_DOUBLE, double)
__cdf_arrpush_func_decl(cdf_vector, CDF_TYPE_VECTOR, vector)

__cdf_get_func_decl(int32_t, CDF_TYPE_INT, int)
__cdf_get_func_decl(uint32_t, CDF_TYPE_UINT, uint)
__cdf_get_func_decl(float, CDF_TYPE_FLOAT, float)
__cdf_get_func_decl(double, CDF_TYPE_DOUBLE, double)
__cdf_get_func_decl(cdf_vector, CDF_TYPE_VECTOR, vector)

__cdf_is_func_decl(CDF_TYPE_INT, int)
__cdf_is_func_decl(CDF_TYPE_UINT, uint)
__cdf_is_func_decl(CDF_TYPE_FLOAT, float)
__cdf_is_func_decl(CDF_TYPE_DOUBLE, double)
__cdf_is_func_decl(CDF_TYPE_VECTOR, vector)
__cdf_is_func_decl(CDF_TYPE_BINARY, binary)
__cdf_is_func_decl(CDF_TYPE_STRING, string)
__cdf_is_func_decl(CDF_TYPE_OBJECT, object)
__cdf_is_func_decl(CDF_TYPE_ARRAY, array)

cdf_retcode cdf_push_binary(cdf_document* pDoc, cdf_object* pParent, const char* sName, uint8_t* Value, size_t iLen);
cdf_retcode cdf_push_string(cdf_document* pDoc, cdf_object* pParent, const char* sName, const char* sValue);

cdf_retcode cdf_array_push_object(cdf_object* pParent, cdf_object* pChild);

#endif
