#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        cbpp::List<int> test;
        
        for(int i = 0; i < 10; i++) {
            printf("\nPBACK №%i:\n", i);
            test.PushBack(i*10);
            printf("\tValue = %i\n", i*10);
            printf("\tImgLength = %i\n", test.Length());
            printf("\tPhyLength = %i\n", test.LengthPhysical());
        }

        return false;
    }

    void ModuleTick() { 
        ddraw::SetColor( Color{255,255,0,255} );
        Vec2 pt = cbvs::GetNormalizedMousePos() * Vec2(cbvs::ScreenRatio, 1.0f);

        float_t dist = pt.Length();

        ddraw::CircleOutline( Vec2(0), dist );
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
