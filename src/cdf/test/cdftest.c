#include <stdio.h>

#include "cdf/cdf.h"

int main(int argc, char** argv) {
    cdf_document* pDoc = cdf_document_create();
    cdf_object* pRoot = &pDoc->m_Root;

    char buff[16];
    for(int i = 0; i < 20; i++) {
        snprintf(buff, 16, "val_%d", i);
        cdf_push_int(pDoc, pRoot, buff, i*1000);
    }

    cdf_object pCurrent;
    size_t iIter = 0;

    while( cdf_object_iterate(pRoot, &pCurrent, &iIter) ) {
        cdf_uint iNameID = pCurrent.m_iNameID;
        printf("'%s'\n", pDoc->m_aNames[iNameID]);
    }

    FILE* hTest = fopen("test.cdf", "wb");
    cdf_document_write(pDoc, hTest);
    fclose(hTest);

    cdf_document* pRead;
    hTest = fopen("test.cdf", "rb");

    cdf_document_read(&pRead, hTest);

    iIter = 0;
    while( cdf_object_iterate(&pRead->m_Root, &pCurrent, &iIter) ) {
        size_t nid = pCurrent.m_iNameID;
        printf("read: '%s'\n", pRead->m_aNames[nid]);
    }

    return 0;
}
