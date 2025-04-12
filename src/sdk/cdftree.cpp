#include "argparse.h"
#include "cdf/cdf.h"

#include <stdio.h>

char* g_sInputName = NULL;

void arg_callback(const char* cmd, const char* arg) {
    if( strcmp(cmd, "-cdf") == 0 ) {
        g_sInputName = strdup(arg);
    }
}

int main(int argc, char** argv) {
    sdk::ResolveArgs(argc, argv, arg_callback);

    FILE* hInput = fopen(g_sInputName, "rb");
    if(hInput == NULL) { return -1; }

    cdf_document* pDoc;
    cdf_retcode iReadResult = cdf_document_read(&pDoc, hInput);

    if(iReadResult != CDF_OK) {
        return iReadResult;
    }

    
}
