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

        Array<STest> A(5);
        Array<STest> B(5);

        Array<STest> C = A + B;

        for(int i = 0; i < C.Length(); i++) {
            printf("%d, %d\n", C.At(i).a, C.At(i).b);
        }

        return true;
    }

    void ModuleTick() { 
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
