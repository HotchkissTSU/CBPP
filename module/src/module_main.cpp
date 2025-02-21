#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        BaseEntity* eTest = CreateEntity("ent_test");

        Print(*eTest);

        File* hTest = OpenFile("hohma.png", "rb");
        delete hTest;
        MountLocale("ru");

        ddraw::SetColor(Color(255,255,0,255));

        return true;
    }

    void ModuleTick() { 
        //ddraw::Line(Vec2(-1), Vec2(1), 3.0f);
        ddraw::RectOutline(Vec2(-0.8), Vec2(0.8), 5.0f);
        ddraw::CircleOutline(Vec2(0.1f), 0.6f, 2.0f);
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
