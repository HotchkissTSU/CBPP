#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");        
        return true;
    }

    void ModuleTick() { 
        ddraw::SetColor( Color{255,255,0,255} );
        Vec2 pt = cbvs::GetNormalizedMousePos();

        float_t dist = pt.Length();

        ddraw::Circle( Vec2(0), dist );
    }
}