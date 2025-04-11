#ifndef CBPP_CDF_H
#define CBPP_CDF_H

#include "cdf/cdf_macros.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum cdf_retcode {
    CDF_OK,                     //Everything is probably cool
    CDF_ALLOC_FAILURE,          //The allocator has fallen, centurion
    CDF_INV_INPUT,              //One or many input args are NULL
    CDF_NON_ITERABLE,           //Attempt to push a sub-value into a primitive object
    CDF_TYPE_UNSUPPORTED,       //Unknown type ID found while reading
    CDF_UNEXPECTED_EOF,         //That EOF should not be there!
    CDF_OUT_OF_BOUNDS,
    CDF_NOT_AN_ARRAY
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
    cdf_uint m_iType;
    cdf_uint m_iNameID;
    void* m_pData;
} __attribute__((packed)) cdf_object;

typedef struct cdf_document {
    cdf_uint m_iNames;
    char** m_aNames;
    cdf_object m_Root;
} cdf_document;

typedef struct cdf_verinfo {
    uint16_t m_iVersionMajor, m_iVersionMinor;
    uint8_t m_iIntSize;
} __attribute__((packed)) cdf_verinfo;

//Get a string description of the chosen error code
const char* cdf_get_error(cdf_retcode iCode);

//Check if selected type lies before the CDF_TYPE_STRING in the types list
int cdf_type_primitive(cdf_type iType);

//Check is this type can be iterated (ARRAY and OBJECT)
int cdf_type_iterable(cdf_type iType);

//Get a size of a primitive value type
size_t cdf_sizeof(cdf_type iType);

//Get a size needed to unwrap this object
size_t cdf_object_sizeof(cdf_object* pObj);

cdf_document* cdf_document_create();
cdf_retcode cdf_document_destroy(cdf_document* pDoc);

cdf_retcode cdf_document_write(cdf_document* pDoc, FILE* hFile);
cdf_retcode cdf_document_read(cdf_document** ppDoc, FILE* hFile);

cdf_uint cdf_nametable_push(cdf_document* pDoc, const char* sName);
cdf_uint cdf_nametable_find(cdf_document* pDoc, const char* sName);
cdf_retcode cdf_nametable_remove(cdf_document* pDoc, cdf_uint iNameIndex);

//Write an object to a plain buffer
cdf_retcode cdf_object_unwrap(cdf_object* pObj, void* pTarget);

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

/*
    Iterate an object. The iterator variable must be zeroed beforehand!
*/
int cdf_object_iterate(cdf_object* pParent, cdf_object* pCurrent, size_t* pIter);

#ifdef CDF_NAMETABLE_COPY
    void cdf_nametable_free(char** pTable, size_t iLen);
#else
    #define cdf_nametable_free(p, i)
#endif

__cdf_push_func_decl(cdf_int, CDF_TYPE_INT, int)
__cdf_push_func_decl(cdf_uint, CDF_TYPE_UINT, uint)
__cdf_push_func_decl(float, CDF_TYPE_FLOAT, float)
__cdf_push_func_decl(double, CDF_TYPE_DOUBLE, double)
__cdf_push_func_decl(cdf_vector, CDF_TYPE_VECTOR, vector)

cdf_retcode cdf_push_binary(cdf_document* pDoc, cdf_object* pParent, const char* sName, uint8_t* Value, size_t iLen);
cdf_retcode cdf_push_string(cdf_document* pDoc, cdf_object* pParent, const char* sName, const char* sValue);

#endif
