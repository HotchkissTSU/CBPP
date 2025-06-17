#include <stdio.h>
#include "cbpp.h"

#include "cbvs/error_check.h"

using namespace cbpp;

spriteid_t g_iSprite;

uint32_t g_iMX, g_iMY;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        glCheck();

        printf("Module entry point!\n");

        LoadTextureSheet("weapon01");
        LoadTextureSheet("eule");
        LoadTextureSheet("fx");

        g_iSprite = GetSpriteID("wife-material");

        return true;
    }

    void ModuleTick() {
        cbvs::RenderSprite(g_iSprite, Vec2(-1), Vec2(0.5f), Color(255));
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
