#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");
        String test = U"word1,'word2,word3',word4";

        size_t split_ln;
        String* splitted = test.SplitEx(split_ln, U',', U'\'');

        printf("split len: %lu\n", split_ln);

        for(size_t i = 0; i < split_ln; i++) {
            splitted[i].Print();
            printf("\n");
        }

        return true;
    }
    
    void ModuleTick() {
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
