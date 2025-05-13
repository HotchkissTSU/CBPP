#include <stdio.h>
#include "cbpp.h"

#include "cbvs/error_check.h"

using namespace cbpp;

cbvs::Texture* g_pTest;

extern "C" {
    void ModulePreMain() { 
    }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        ddraw::SetColor(Color(255,255,0,255));

        g_pTest = new cbvs::Texture;
        g_pTest->Load("weapon/sheet_weapon_01.png", cbvs::IMG_RGBA);

        GLuint hTest = g_pTest->GetHandle();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glCheck();

        CbBenchmark(stdout);

        Event test;
        memset(&test, 0, sizeof(test));

        for(size_t i = 0; i < 128; i++) {
            PushEvent(&test);
        }

        while( PollEvent(&test) ) {
            //printf("D = %ld\n", test.Timestamp);
        }

        return true;
    }

    void ModuleTick() { 
        ddraw::Texture(Vec2(-1), Vec2(2), *g_pTest, true);
    }

    //Return TRUE if the event was processed by the module and the engine should not use it
    bool ModuleEventCallback( cbpp::Event& ev ) { return false; }
}
