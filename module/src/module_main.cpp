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
        g_pTest->Load("hohma.png", cbvs::IMG_RGB);

        CbBenchmark(stdout);

        Event test;
        memset(&test, 0, sizeof(test));

        for(size_t i = 0; i < 128; i++) {
            test.Timestamp++;
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

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
