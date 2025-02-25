#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

cbvs::Texture* g_pTest;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        ddraw::SetColor(Color(255,255,0,255));

        g_pTest = new cbvs::Texture;
        g_pTest->Load("hohma.png");

        return true;
    }

    void ModuleTick() { 
        ddraw::Texture(Vec2(-0.5f), 0.5f, *g_pTest);
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
