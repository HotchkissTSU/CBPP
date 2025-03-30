#ifndef CBPP_ASSET_CDF_H
#define CBPP_ASSET_CDF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define CDF_ERRORNAME "[error]"

#ifdef CDF_SAFE_CHECKS
    #define CDF_CHECK(code) code
#else
    #define CDF_CHECK(code)
#endif

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

typedef struct cdf_object {
    uint32_t m_iNameID;
    uint32_t m_iLength;
    CDF_OBJECT_TYPE m_iType;
    union {
        int32_t i32;
        uint32_t u32;
        float f32;

        struct {
            float fX;
            float fY;
        } v16;

        char* utf8;
        uint8_t* objinfo;
    } m_uData;
} cdf_object;

typedef struct cdf_document {
    uint32_t m_iNames;
    char** m_aNames;

    cdf_object m_hRoot;
} cdf_document;

/*
    Get the size of the selected CDF datatype.
    Returns 0 for CDF_TYPE_ARRAY and CDF_TYPE_OBJECT.
*/
size_t cdf_sizeof(CDF_OBJECT_TYPE iType);

/*
    Get amount of space needed to unwrap and store this object
*/
size_t cdf_object_sizeof(cdf_object* pObj);

cdf_document* cdf_document_read(FILE* hFile);
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
bool cdf_object_iterate(cdf_object* pObj, size_t* pIter, cdf_object** pCurrent);

/*
    Allocate a new object from the iterator
*/
cdf_object* cdf_object_from_iter(cdf_object* pParent, size_t iIter);

/*
    Deallocate an object and set it\`s pointer to NULL. 
    If pDoc is not NULL, then this object`s name will be removed
    from the document`s nametable.
*/
void cdf_object_destroy(cdf_document* pDoc, cdf_object** pObj);

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
cdf_object* cdf_object_by_nid(cdf_object* pParent, uint32_t iNameIndex);

/*
    Obtain an object by it`s string name
*/
cdf_object* cdf_object_by_name(cdf_document* pDoc, cdf_object* pParent, const char* sName);

/*
    Obtain an object by it`s array index.
*/
cdf_object* cdf_object_by_index(cdf_object* pParent, uint32_t iIndex);

#endif
