#include <stdio.h>
#include "cdf/cdf.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

int main(int argc, char** argv) {
    cdf_document* pDoc = cdf_document_new();
    cdf_object* pRoot = cdf_document_root(pDoc);

    char buff[128];
    for(int i = 0; i < 10; i++) {
        snprintf(buff, 128, "value_%d", i);
        cdf_push_int(pDoc, pRoot, buff, i);
    }

    FILE* hFile = fopen("test.cdf", "wb");
    cdf_document_write(hFile, pDoc);
    fclose(hFile);

    cdf_document_destroy(&pDoc);

    hFile = fopen("test.cdf", "rb");
    pDoc = cdf_document_read(hFile);
    fclose(hFile);

    pRoot = cdf_document_root(pDoc);

    cdf_iterator Iter;
    cdf_iterator_init(&Iter);

    int v;

    while( cdf_object_iterate(pRoot, &Iter) ){
        memcpy(&v, Iter.m_pData, 4);
        size_t iNameIndex = Iter.m_iNameID;
        printf("'%s' = %d\n", pDoc->m_aNames[iNameIndex], v);
    }

    return 0;
}
