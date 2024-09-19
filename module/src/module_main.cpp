#include <stdio.h>

extern "C" {
    
bool ModuleMain(int argc, char** argv) {
    printf("Module entry point!\n");
    
    return true;
}

void ModuleTick() {
    return;
}

void ModuleWindowHint() {
    
}
    
}
