#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

static Image testi;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

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
