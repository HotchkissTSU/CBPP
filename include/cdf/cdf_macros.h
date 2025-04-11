#ifndef CBPP_CDF_MACRO_H
#define CBPP_CDF_MACRO_H

#include <stddef.h>
#include <stdint.h>

#define CDF_VERSION_MAJOR 1
#define CDF_VERSION_MINOR 0

#define CDF_NAMETABLE_COPY

/*
    Should i perform null-checks in every function?
*/
#ifdef CDF_SAFE_CHECK
    #define CDF_CHECK(code) code
#else
    #define CDF_CHECK(code)
#endif

/*
    Should i copy a string while appending it to the nametable instead of referencing it?
*/
#ifdef CDF_NAMETABLE_COPY
    #define CDF_COPY(string) strdup(string)
#else
    #define CDF_COPY(string) string
#endif

#define CDF_NAME_ERROR "<ERROR>"
#define CDF_NAME_ROOT "root"
#define CDF_HEADER_SIZE sizeof(cdf_object) - sizeof(void*)

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

#define __cdf_push_func_decl(type, itype, deconame)\
cdf_retcode cdf_push_##deconame (cdf_document* pDoc, cdf_object* pParent, const char* sName, type Value);

#define __cdf_push_func(type, itype, deconame)\
cdf_retcode cdf_push_##deconame (cdf_document* pDoc, cdf_object* pParent, const char* sName, type Value) {\
    return cdf_data_push_ex(pDoc, pParent, sName, &Value, sizeof(Value), itype);}

#endif
