#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

struct STest {
    STest() {
        a = 5;
        b = 10;
    }

    ~STest() {
        a = 0;
        b = 0;
    }

    int a, b;
};

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        return true;
    }

    void ModuleTick() { 
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
