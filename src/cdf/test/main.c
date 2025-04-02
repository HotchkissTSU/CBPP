#include <stdio.h>
#include "cdf/cdf.h"
#include <stdlib.h>

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

    return 0;
}
