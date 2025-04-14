#ifndef CBPP_CDF_MACRO_H
#define CBPP_CDF_MACRO_H

#include <stddef.h>
#include <stdint.h>

#define CDF_VERSION_MAJOR 1
#define CDF_VERSION_MINOR 0

/*
    Should we perform null-checks in every function?
*/
#ifdef CDF_SAFE_CHECK
    #define CDF_CHECK(code) code
#else
    #define CDF_CHECK(code)
#endif

/*
    Should we copy a string while appending it to the nametable instead of referencing it?
*/
#ifdef CDF_NAMETABLE_COPY
    #define CDF_COPY(string) strdup(string)
#else
    #define CDF_COPY(string) string
#endif

#define CDF_NAME_ERROR "<ERROR>"
#define CDF_NAME_ROOT "root"
#define CDF_FILE_MARK *((uint32_t*)("CDF"))
#define CDF_HEADER_SIZE (sizeof(cdf_object) - sizeof(void*))
#define CDF_COMPRESSION_SUPPORTED defined(CDF_USE_ZLIB)

/*
    The integer types used by the format
*/
typedef uint32_t cdf_uint;
typedef int32_t  cdf_int;

#define CDF_INT_SIZE sizeof(cdf_uint)

#define cdf_object_create(pDoc, sName, iType)\
    cdf_object_create_ex(pDoc, sName, cdf_sizeof(iType), iType)

#define cdf_object_destroy(pDoc, pObj)\
    cdf_object_destroy_ex(pDoc, pObj, false);

#define cdf_document_root(pDoc)\
    &pDoc->m_Root

#define __cdf_push_func_decl(type, itype, deconame)\
cdf_retcode cdf_push_##deconame (cdf_document* pDoc, cdf_object* pParent, const char* sName, type Value);

#define __cdf_push_func(type, itype, deconame)\
cdf_retcode cdf_push_##deconame (cdf_document* pDoc, cdf_object* pParent, const char* sName, type Value) {\
    return cdf_data_push_ex(pDoc, pParent, sName, &Value, sizeof(Value), itype);}

#define __cdf_arrpush_func_decl(type, itype, deconame)\
cdf_retcode cdf_array_push_##deconame (cdf_object* pObj, type value);

#define __cdf_arrpush_func(type, itype, deconame)\
cdf_retcode cdf_array_push_##deconame (cdf_object* pObj, type value) {\
    return cdf_array_data_push_ex(pObj, &value, sizeof(value), itype);}

#define __cdf_get_func(type, itype, name)\
cdf_retcode cdf_as_##name (cdf_object* pObj, type* pTarget) {\
    if(pObj->m_iType != itype) { memset(pTarget, 0, sizeof(type)); return CDF_TYPE_MISMATCH; }\
    memcpy(pTarget, pObj->m_pData, sizeof(type)); return CDF_OK;}

#define __cdf_get_func_decl(type, itype, name)\
cdf_retcode cdf_as_##name (cdf_object* pObj, type* pTarget);

#define __cdf_is_func_decl(i, name)\
int cdf_is_##name (cdf_object* pObj);

#define __cdf_is_func(i, name)\
int cdf_is_##name (cdf_object* pObj) { return cdf_object_type(pObj) == i; }

#define CDF_ENAME(e) case e: return #e;

#endif
