#include <stdio.h>
#include "cbpp.h"

#include "cbvs/error_check.h"

using namespace cbpp;

cbvs::SpriteBatcher* g_pTest;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        g_pTest = new cbvs::SpriteBatcher();

        int x,y;
        bool res = g_pTest->LocateFreeRegion(5,20,x,y);

        if(res) {
            g_pTest->MarkRegionAs(x,y,5,20,true);
        }

        g_pTest->LocateFreeRegion(5,10,x,y);
        g_pTest->MarkRegionAs(x,y,5,12, true);

        g_pTest->PrintMappingInfo();

        return true;
    }

    void ModuleTick() {}

    //Return TRUE if the event was processed by the module and engine should not use it
    bool ModuleEventCallback( cbpp::Event& ev ) { return false; }
}
