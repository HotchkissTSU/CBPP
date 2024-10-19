#include <stdio.h>
#include "cbvs/shader.h"

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        cbvs::Shader testv(GL_VERTEX_SHADER, cbvs::default_vtx);
        cbvs::Shader testf(GL_FRAGMENT_SHADER, cbvs::default_frag);

        cbvs::Pipe testp(&testv, &testf);

        printf("tv:%d, tf:%d, pipe:%d\n", testv.IsValid(), testf.IsValid(), testp.IsValid());
        
        return true;
    }

    void ModuleTick() { }
}
