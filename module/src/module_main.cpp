#include <stdio.h>
#include "cbpp.h"

using namespace cbpp;

extern "C" {
    void ModuleWindowHint() { }

    bool ModuleMain(int argc, char** argv) {
        printf("Module entry point!\n");

        DataFile test;

        DataFile::Block blk;
        blk.SetName("test_block");
        
        DataFile::Value val(DataFile::VType::U32);
        val.SetName("test_value_float");
        val.Data[0].u32 = 55599;

        blk.PushValue(val);

        blk.Data[0].Print();

        test.PushBlock(blk);

        if( !test.Save("test.cdf") ) {
            while(HasErrors()) {
                const ErrorInfo& err = GetLastError();
                printf("ERROR: %s", err.Msg);
            }
        }

        return true;
    }

    void ModuleTick() { 
    }

    bool ModuleEventCallback( cbpp::Event& ev ) { return true; }
}
