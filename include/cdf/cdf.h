#ifndef CBPP_ASSET_CDF_H
#define CBPP_ASSET_CDF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CDF_ERRORNAME "[error]"

#define CDF_HEADER_SIZE offsetof(cdf_object, m_uData)

#ifdef CDF_SAFE_CHECKS
    #define CDF_CHECK(code) code
#else
    #define CDF_CHECK(code)
#endif

#define cdf_object_type(obj) obj->m_iType
#define cdf_iterator_init(iter) memset(iter, 0, sizeof(cdf_iterator))

//Push macros
#define cdf_push_binary(doc, obj, name, data, len)\
    cdf_push_generic(doc, obj, name, (void*)data, len, CDF_TYPE_BINARY)

#define cdf_push_vector(doc, obj, name, x, y)\
    {struct { float fx; float fy; } vec; vec.fx = x; vec.fy = y;\
    cdf_push_generic(doc, obj, name, (void*)(&vec), sizeof(float)*2, CDF_TYPE_VECTOR);}

#define cdf_push_float(doc, obj, name, f)\
    {float fd = f; cdf_push_generic(doc, obj, name, (void*)(&fd), sizeof(float), CDF_TYPE_FLOAT);}

#define cdf_push_int(doc, obj, name, i)\
    {int32_t id = i; cdf_push_generic(doc, obj, name, (void*)(&id), sizeof(int32_t), CDF_TYPE_INT);}

#define cdf_push_uint(doc, obj, name, i)\
    {uint32_t ud = i; cdf_push_generic(doc, obj, name, (void*)(&ud), sizeof(uint32_t), CDF_TYPE_UINT);}

#define cdf_push_string(doc, obj, name, string)\
    cdf_push_generic(doc, obj, name, string, strlen(string), CDF_TYPE_STRING)

//Get value macros
#define cdf_get_binary(obj, target, len)\
    cdf_get_generic(obj, (void*)target, len)

#define cdf_get_vector(obj, target)\
    cdf_get_generic(obj, (void*)target, sizeof(float)*2)

#define cdf_get_float(obj, target)\
    cdf_get_generic(obj, (void*)target, sizeof(float))

#define cdf_get_int(obj, target)\
    cdf_get_generic(obj, (void*)target, sizeof(int32_t))

#define cdf_get_uint(obj, target)\
    cdf_get_generic(obj, (void*)target, sizeof(uint32_t))

typedef enum CDF_OBJECT_TYPE {
    CDF_TYPE_INT,     //A signed 32-bit integer
    CDF_TYPE_UINT,    //An unsigned 32-bit integer
    CDF_TYPE_FLOAT,   //A 4-bytes float
    CDF_TYPE_VECTOR,  //A 2D vector
    CDF_TYPE_STRING,  //UTF-8 encoded string (w/o null-terminator)
    CDF_TYPE_BINARY,  //Any binary sequence
    
    CDF_TYPE_OBJECT,  //An object with sub-objects in it
    CDF_TYPE_ARRAY,   //A list of identical objects

    CDF_CORE_TYPES_AMOUNT
} CDF_OBJECT_TYPE;

typedef struct cdf_vector {
    float fX, fY;
} cdf_vector;

typedef struct cdf_object {
    uint32_t m_iNameID;
    uint32_t m_iLength;
    size_t m_iType;
    
    union {
        int32_t i32;
        uint32_t u32;
        float f32;

        cdf_vector v16;

        char* utf8;
        uint8_t* objinfo;
    } m_uData;
} cdf_object;

typedef struct cdf_iterator {
    uint32_t m_iNameID;
    uint32_t m_iLength;
    size_t m_iType;

    uint8_t* m_pData;
    size_t m_iIter;
} cdf_iterator;

typedef struct cdf_document {
    uint32_t m_iNames;
    char** m_aNames;

    cdf_object m_hRoot;
} cdf_document;

/*
    Check if the given type is a complex one:
    CDF_TYPE_ARRAY, CDF_TYPE_OBJECT or CDF_TYPE_STRING
*/
bool cdf_type_complex(CDF_OBJECT_TYPE iType);

/*
    Get the size of the selected CDF datatype.
    Returns 0 for CDF_TYPE_ARRAY and CDF_TYPE_OBJECT.
*/
size_t cdf_sizeof(CDF_OBJECT_TYPE iType);

/*
    Get amount of space needed to unwrap and store this object
*/
size_t cdf_object_sizeof(cdf_object* pObj);

/*
    Read a document from the file
*/
cdf_document* cdf_document_read(FILE* hFile);

/*
    Write a document in the file
*/
bool cdf_document_write(FILE* hFile, cdf_document* pDoc);

cdf_document* cdf_document_new();

/*
    Obtain a pointer to the document`s root value;
    It always has a type of CDF_TYPE_OBJECT, and
    you should not attempt to deallocate it via cdf_object_destroy
*/
cdf_object* cdf_document_root(cdf_document* pDoc);
void cdf_document_destroy(cdf_document** pDoc);

/*
    Add a new name to the table and get it`s index
*/
uint32_t cdf_nametable_push(cdf_document* pDoc, const char* sName);

/*
    Find the index of the providen name, or a free place if the
    second argument is NULL.
*/
uint32_t cdf_nametable_find(cdf_document* pDoc, const char* sName);

bool cdf_nametable_remove(cdf_document* pDoc, uint32_t iNameIndex);
bool cdf_nametable_remove_name(cdf_document* pDoc, const char* sName);

void cdf_nametable_free(char** pTable, size_t iTableLen);

/*
    Set up object\`s insides. Can be used to initialize stack allocated values
*/
void cdf_object_init(cdf_document* pDoc, cdf_object* pObj, const char* sName, CDF_OBJECT_TYPE iType);

/*
    Allocate a new object of the given type.
    If pDoc is not NULL, then this object`s name will be added to the
    document`s nametable.
*/
cdf_object* cdf_object_new(cdf_document* pDoc, const char* sName, CDF_OBJECT_TYPE iType);

/*
    Create a copy of the given object
*/
cdf_object* cdf_object_new_from_obj(cdf_object* pObj);

/*
    Iterate through object\`s content.
    Don\`t deallocate the pCurrent pointer as it points inside the parent object.
    If you want to get a standalone object from it, use cdf_object_from_iter.

    Iterator variable must be set to zero beforehand.
*/
bool cdf_object_iterate(cdf_object* pObj, cdf_iterator* pIter);

/*
    Allocate a new object from the iterator
*/
cdf_object* cdf_object_from_iter(cdf_object* pParent, cdf_iterator* pIter);

uint8_t* cdf_object_data_from_iter(cdf_object* pParent, size_t iIter);

/*
    Deallocate an object and set it\`s pointer to NULL. 
    If pDoc is not NULL, then this object`s name will be removed
    from the document`s nametable.
*/
void cdf_object_destroy(cdf_document* pDoc, cdf_object** pObj, bool bStack);

/*
    Push a sub-object. Parent`s type must be CDF_TYPE_ARRAY or CDF_TYPE_OBJECT. 
    All objects you are pushing into an array must have the same size.

    All pushed objects are copied, so the source ones can be deleted without any concernes.
*/
bool cdf_object_push(cdf_object* pParent, cdf_object* pChild);

/*
    Unwrap an object into a plain buffer
*/
bool cdf_object_copy(cdf_object* pObj, uint8_t* pTarget);

/*
    Obtain an object by it`s nameID
*/
bool cdf_object_by_nid(cdf_object* pParent, cdf_iterator* pTarget, uint32_t iNameIndex);

/*
    Obtain an object by it`s string name
*/
bool cdf_object_by_name(cdf_document* pDoc, cdf_object* pParent, cdf_iterator* pTarget, const char* sName);

/*
    Obtain an object by it`s array index.
*/
bool cdf_object_by_index(cdf_object* pParent, cdf_iterator* pTarget, uint32_t iIndex);

bool cdf_push_generic(cdf_document* pDoc, cdf_object* pParent, const char* sName, void* pBinary, size_t iLength, CDF_OBJECT_TYPE iType);
bool cdf_get_generic(cdf_object* pParent, void* pTarget, size_t iLength);

#endif
