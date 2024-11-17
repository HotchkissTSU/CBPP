#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        setlocale(LC_ALL, "");

        printf("Module entry point!\n");

        const Char* test_u32 = U"🍑";

        printf("ASS LETTER (REAL): %ls\n", test_u32);

        const char* test_u8 = String::U32_U8(test_u32);
        const Char* u32_after = String::U8_U32(test_u8);

        if(u32_after == NULL) {
            printf("error : %s\n", GetLastError().Msg);
        }

        printf("ASS LETTER: %ls\n", u32_after);

        return true;
    }

    void ModuleTick() { 

    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
