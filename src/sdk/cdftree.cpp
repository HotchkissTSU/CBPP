#include "argparse.h"
#include "cdf/cdf.h"

#include <stdio.h>

char* g_sInputName = NULL;

void arg_callback(const char* cmd, const char* arg) {
    if( strcmp(cmd, "-cdf") == 0 ) {
        g_sInputName = strdup(arg);
    }
}

void print_tabs(int iDepth) {
    for(int i = 0; i < iDepth; i++) {
        printf("\t");
    }
}

void print_object(cdf_document* pDoc, cdf_object* pObj, int iDepth) {
    uint32_t iNameID = pObj->m_iNameID;

    print_tabs(iDepth);
    printf("%s (%d):\n", pDoc->m_aNames[iNameID], pObj->m_iType);

    cdf_iterator_init(Iter);
    while( cdf_object_iterate(pObj, &Iter) ) {
        cdf_object* pSubObj = cdf_object_from_iter(pObj, &Iter);
        print_object(pDoc, pSubObj, iDepth+1);
        cdf_object_destroy(NULL, &pSubObj, false);
    }
}

int main(int argc, char** argv) {
    sdk::ResolveArgs(argc, argv, arg_callback);

    FILE* hInput = fopen(g_sInputName, "rb");
    if(hInput == NULL) { return -1; }

    cdf_document* pDoc = cdf_document_read(hInput);
    fclose(hInput);

    print_object(pDoc, cdf_document_root(pDoc), 0);
}
