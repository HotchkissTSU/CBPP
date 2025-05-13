#include "cdf/cdf.h"

#include <stdio.h>
#include <unistd.h>  
#include <stdint.h>

const char* g_sHelpString = "CBPP SDK\ncdftree:\n'-h' - display this message\n'-f' - specify a source CDF file\n\
'-b' - specify the output length of the CDF_TYPE_BINARY values\n'-d' - set the maximal depth for the recursion\n";

int g_iMaxDepth = 32;
int g_iBinaryOutputLength = 16;
cdf_document* g_pDoc;

void puttabs(int iAmount) {
    for(int i = 0; i < iAmount; i++) { printf("\t"); }
}

void validate_cdf(cdf_retcode iCode) {
    if(iCode != CDF_OK) {
        printf("\nCDF error: %s\n", cdf_get_error(iCode));
        exit(EXIT_FAILURE);
    }
}

void print_cdf_object(cdf_object* pCurrent, int iDepth, int bSupressNaming) {
    if(iDepth > g_iMaxDepth) {
        puttabs(iDepth); printf("The recursion limit (%d) exeeded\n", g_iMaxDepth);
        return;
    }

    if(iDepth > 0) {
        puttabs(iDepth);
        if(bSupressNaming) {
            
        }else{
            const char* sName = cdf_object_name(g_pDoc, pCurrent);
            printf("%s, ", sName);
        }
    }else{
        printf("ROOT: ");
    }

    switch (cdf_object_type(pCurrent)) {
        case CDF_TYPE_INT: {
            int32_t iValue;
            cdf_retcode iCode = cdf_get_int(pCurrent, &iValue);
            validate_cdf(iCode);
            printf("int: %d\n", iValue);
            break;
        }

        case CDF_TYPE_UINT: {
            uint32_t iValue;
            cdf_retcode iCode = cdf_get_uint(pCurrent, &iValue);
            validate_cdf(iCode);
            printf("uint: %u\n", iValue);
            break;
        }

        case CDF_TYPE_FLOAT: {
            float iValue;
            cdf_retcode iCode = cdf_get_float(pCurrent, &iValue);
            validate_cdf(iCode);
            printf("float: %f\n", iValue);
            break;
        }

        case CDF_TYPE_DOUBLE: {
            double iValue;
            cdf_retcode iCode = cdf_get_double(pCurrent, &iValue);
            validate_cdf(iCode);
            printf("double: %lf\n", iValue);
            break;
        }

        case CDF_TYPE_LONG: {
            int64_t iValue;
            cdf_retcode iCode = cdf_get_long(pCurrent, &iValue);
            validate_cdf(iCode);
            printf("long: %ld\n", iValue);
            break;
        }

        case CDF_TYPE_ULONG: {
            uint64_t iValue;
            cdf_retcode iCode = cdf_get_ulong(pCurrent, &iValue);
            validate_cdf(iCode);
            printf("ulong: %lu\n", iValue);
            break;
        }

        case CDF_TYPE_VECTOR: {
            cdf_vector iValue;
            cdf_retcode iCode = cdf_get_vector(pCurrent, &iValue);
            validate_cdf(iCode);
            printf("vector: (%f;%f)\n", iValue.m_fX, iValue.m_fY);
            break;
        }

        case CDF_TYPE_STRING: {
            char* sString;
            cdf_retcode iCode = cdf_get_binary(pCurrent, (uint8_t**)&sString);
            validate_cdf(iCode);
            cdf_uint iLen = cdf_object_length(pCurrent);

            printf("string (%d): \"", iLen);
            for(size_t i = 0; i < iLen; i++) { putc(sString[i], stdout); }
            printf("\"\n");

            break;
        }

        case CDF_TYPE_BINARY: {
            uint8_t* pData;
            cdf_retcode iCode = cdf_get_binary(pCurrent, &pData);
            validate_cdf(iCode);
            cdf_uint iLen = cdf_object_length(pCurrent);

            if(iLen > g_iBinaryOutputLength) {
                iLen = g_iBinaryOutputLength;
            }

            printf("binary (%d): [", cdf_object_length(pCurrent));
            for(size_t i = 0; i < iLen; i++) {
                printf("0x%x", pData[i]);
                if(i != iLen-1) { printf(" "); }
            }
            printf("]\n");
            break;
        }
        
        case CDF_TYPE_ARRAY: {
            cdf_object pObj;
            cdf_uint iLen;
            iLen = cdf_array_length(pCurrent);
            printf("array (%d):\n", iLen);
            for(size_t i = 0; i < iLen; i++) {
                cdf_retcode iCode = cdf_array_index(pCurrent, &pObj, i);
                validate_cdf(iCode);
                print_cdf_object(&pObj, iDepth+1, 1);
            }
            break;
        }

        case CDF_TYPE_OBJECT: {
            cdf_object Obj;
            size_t iIter = 0;
            printf("\n");
            while(cdf_object_iterate(pCurrent, &Obj, &iIter)) {
                print_cdf_object(&Obj, iDepth+1, 0);
            }
            break;
        }
    }
}

int main( int argc, char** argv ) {
    const char* sFileName;
    int bHasFilename = 0;

    char c;
    while( (c = getopt(argc, argv, "hf:d:b:")) != -1 ) {
        const char* sOpt = optarg;
        switch ( c ) {
            case 'h': printf("%s", g_sHelpString); exit(EXIT_SUCCESS);
            case 'f': sFileName = sOpt; bHasFilename = 1; break;
            case 'd': g_iMaxDepth = atoi(sOpt); break;
            case 'b': g_iBinaryOutputLength = atoi(sOpt); break;
        }
    }

    if(bHasFilename == 0) {
        printf("No input file providen\n");
        exit(EXIT_FAILURE);
    }

    FILE* hInput = fopen(sFileName, "rb");
    if(hInput == NULL) {
        printf("Failed to open file '%s'\n", sFileName);
        exit(EXIT_FAILURE);
    }

    cdf_verinfo Version;
    int16_t iClassID;

    cdf_retcode iCode = cdf_file_read(hInput, &g_pDoc, &Version, &iClassID);
    validate_cdf(iCode);

    fclose(hInput);

    cdf_object* pRoot = cdf_document_root(g_pDoc);
    print_cdf_object(pRoot, 0, 0);

    cdf_document_destroy(g_pDoc);

    return 0;
}
