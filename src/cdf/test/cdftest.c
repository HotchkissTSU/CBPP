#include <stdio.h>

#include "cdf/cdf.h"

int main(int argc, char** argv) {
    cdf_document* pDoc = cdf_document_create();
    cdf_object* pRoot = cdf_document_root(pDoc);

    cdf_object* pArray = cdf_object_create(pDoc, "array_test", CDF_TYPE_ARRAY);
    for(int i = 0; i < 100; i++) {
        cdf_array_push_int(pArray, i*10);
    }

    cdf_object_push(pRoot, pArray);

    cdf_object Tmp;
    int32_t iValue;

    printf("arrln=%d\n", cdf_array_length(pArray));

    for(cdf_uint i = 0; i < cdf_array_length(pArray); i++) {
        cdf_array_index(pArray, &Tmp, i);
        cdf_as_int(&Tmp, &iValue);
        printf("arr[%d] = %d\n", i, iValue);
    }

    FILE* hFile = fopen("arrtest.cdf", "wb");
    cdf_document_write(hFile, pDoc);

    return 0;
}
