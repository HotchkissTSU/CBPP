#ifndef CBPP_SDK_ARG_H
#define CBPP_SDK_ARG_H

#include <cstring>

namespace sdk {
    void ResolveArgs(int argc, char** argv, void (*callback)(const char*, const char*)) {
        char* next = NULL;
        for(int i = 0; i < argc; i++) {
            if( i < argc+1 ) {
                next = argv[i+1];
            }else{
                next = NULL;
            }

            callback(argv[i], next);
        }
    }
}

#endif
