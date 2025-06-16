#include <stdio.h>
#include "cbpp.h"

#include "cbvs/error_check.h"

using namespace cbpp;

cbvs::SpriteComposer* g_pTest;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        glCheck();

        printf("Module entry point!\n");

        LoadTextureSheet("uvtest.cta");

        //CompileTextureSheets();

        return true;
    }

    void ModuleTick() {
        cbvs::RenderSprite("uv_debug", Vec2(-0.25f), Vec2(0.5f), Color(255));
    }
    
    //Return TRUE if the event was processed by the module and engine should not use it
    bool ModuleEventCallback( cbpp::Event& ev ) { return false; }
}
