#include <stdio.h>
#include "cdf/cdf.h"
#include <stdlib.h>

#include <stdio.h>

int main(int argc, char** argv) {
    cdf_document* pDoc = cdf_document_new();
    cdf_object* pRoot = cdf_document_root(pDoc);

    cdf_object* pTestFloat = cdf_object_new(pDoc, "look at me hector", CDF_TYPE_FLOAT);
    pTestFloat->m_uData.f32 = 12345.0f;

    cdf_object* pTestInt = cdf_object_new(pDoc, "los pollos hermanos", CDF_TYPE_INT);

    cdf_object_push(pRoot, pTestFloat);
    cdf_object_push(pRoot, pTestInt);

    cdf_object_destroy(NULL, &pTestFloat);
    cdf_object_destroy(NULL, &pTestInt);

    FILE* hTest = fopen("test.cdf", "wb");
    cdf_document_write(hTest, pDoc);
    fclose(hTest);

    cdf_document_destroy(&pDoc);

    hTest = fopen("test.cdf", "rb");
    cdf_document* pTest = cdf_document_read(hTest);
    fclose(hTest);

    size_t iIter = 0;
    cdf_object* pCurrent = NULL;

    while( cdf_object_iterate( cdf_document_root(pTest), &iIter, &pCurrent ) ) {
        uint32_t iNameID = pCurrent->m_iNameID;
        printf("'%s'\n", pTest->m_aNames[iNameID]);
    }

    return 0;
}
