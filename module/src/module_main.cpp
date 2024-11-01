#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

static Image testi;

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

        for(int i = 0; i < 10; i++) {
            printf("%i ", test[i]);
        }
        printf("\n");

        testi = LoadImage("assets/textures/sdf_test.png", LOAD_RGBA, FLAG_POWER_OF_2 | FLAG_INV_Y);

        return true;
    }

    void ModuleTick() { 
        ddraw::SetColor( Color{255,255,0,255} );
        Vec2 pt = cbvs::GetNormalizedMousePos() * Vec2(cbvs::ScreenRatio, 1.0f);
        ddraw::Image(pt, Vec2(0.3f), testi);
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
