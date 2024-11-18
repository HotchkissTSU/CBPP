#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        String test_a(U"готовые движки ");
        String test_b(U"для слабых и глупых ");

        String test = test_a + test_b * 5;

        test.Print();printf("\n");

        return true;
    }

    void ModuleTick() {
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
