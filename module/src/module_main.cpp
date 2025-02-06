#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        BaseEntity* eTest = CreateEntity("ent_test");
        EPropNode* pHead = eTest->GetProperties();
        int i = 0;

        while(pHead != NULL) {
            i++;
            printf("[%d] %s\n", i, pHead->m_pProperty->Name());

            pHead = pHead->m_pNextNode;
        }

        return true;
    }

    void ModuleTick() { }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
