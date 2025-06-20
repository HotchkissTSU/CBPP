#include <stdio.h>
#include "cbpp.h"

#include "cbvs/error_check.h"

using namespace cbpp;

spriteid_t g_iSprite;

uint32_t g_iMX, g_iMY;

float_t g_fNigga = 0.0f;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        glCheck();

        printf("Module entry point!\n");

        LoadSheet("weapon01");
        LoadSheet("eule");
        LoadSheet("fx");

        g_iSprite = GetSpriteID("weapon_hcal_carbine_side");

        return true;
    }
    
    void ModuleTick() {
        g_fNigga += 0.01f;
        cbvs::RenderSprite(g_iSprite, Vec2(0.0f, 0.0f), Vec2(0.7f), g_fNigga, Color(255), 1.0f);
    }

    //Return TRUE if the event was processed by the module and engine should not use it
    bool ModuleEventCallback( cbpp::Event& ev ) {
        switch (ev.Type) {
            case Event::MOUSE_MOVE:
                g_iMX = ev.Data.MouseMove.X;
                g_iMY = ev.Data.MouseMove.Y;
                return true;
        }

        return false;
    }
}
