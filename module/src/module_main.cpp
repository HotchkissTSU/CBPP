#include <stdio.h>
#include "cbpp.h"

#include "cbvs/error_check.h"

using namespace cbpp;

spriteid_t g_iSprite;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        glCheck();

        printf("Module entry point!\n");

        LoadTextureSheet("uvtest");
        LoadTextureSheet("weapon01");

        g_iSprite = GetSpriteID("hcal_carbine_side");

        return true;
    }
    
    void ModuleTick() {
        //ddraw::Texture(Vec2(-0.25f), Vec2(0.5f), g_hTexture, true);
        cbvs::RenderSprite(g_iSprite, Vec2(-0.25f), Vec2(0.5f), Color(255,0,0));
    }

    //Return TRUE if the event was processed by the module and engine should not use it
    bool ModuleEventCallback( cbpp::Event& ev ) { return false; }
}
