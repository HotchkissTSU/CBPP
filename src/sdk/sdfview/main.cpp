#include <stdio.h>
#include "cbpp.h"
#include "argparse.h"

using namespace cbpp;

char* input = NULL;

void arg_callback(const char* cmd, const char* arg) {
    if(strcmp(cmd, "-img") == 0) {
        if(arg != NULL) {
            input = strdup(arg);
        }
    }

    if(strcmp(cmd, "-sdf") == 0) {
        if(arg != NULL) {
            output = strdup(arg);
        }
    }
}

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        sdk::ResolveArgs(argc, argv, arg_callback);

        return true;
    }

    void ModuleTick() { 
        
    }
}