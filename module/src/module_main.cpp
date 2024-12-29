#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

World* g_World;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        g_World = new World();

        size_t iChunkID = g_World->WorldToChunk(Vec2(-17,5));
        Vec2 vChunkPos = g_World->ChunkToWorld(iChunkID);

        printf("%d, %f, %f\n", iChunkID, vChunkPos.x, vChunkPos.y);

        return true;
    }

    void ModuleTick() {
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
