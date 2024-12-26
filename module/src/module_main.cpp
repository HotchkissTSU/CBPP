#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

World* g_World;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");
        
        BaseEntity* eTest = CreateEntity("ent_test");
        printf("eTest = %s\n", eTest->Class());

        g_World = new World();

        for(int i = 0; i < 10000; i++) {
            BaseEntity* eTmp = CreateEntity("ent_test");
            printf("id: %d\n", g_World->SpawnEntity(eTmp));
        }

        return true;
    }

    void ModuleTick() {
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
