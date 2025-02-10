#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        BaseEntity* eTest = CreateEntity("ent_test");
        
        PrintEntity(eTest);

        return true;
    }

    void ModuleTick() { }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
