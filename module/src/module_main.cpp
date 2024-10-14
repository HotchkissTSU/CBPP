#include <stdio.h>
#include "cbpp/cbstring.h"

extern "C" {
    void ModuleWindowHint() {
        
    }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        cbpp::String test = "test string";
        
        return true;
    }

    void ModuleTick() {
        return;
    }
}
