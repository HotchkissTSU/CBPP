#include "cbpp/argparse.h"

#include <string.h>

namespace cbpp {
    int DetectCMD( const CmdOptionInfo* aCommands, int iCommandsNum, const char* sCommand ) {
        for( int i = 0; i < iCommandsNum; i++ ) {
            const CmdOptionInfo& Current = aCommands[i];
            if( (strcmp(sCommand, Current.sCommandFull) == 0) || (sCommand[0] == Current.cCommandShort) ) {
                
            }
        }

        return 0;
    }

    /*
        CmdCurrentOption Current;
        while( ParseCMD(comms, icomms, Current, argc, argv) ) {
            ...
        }
    */

    bool ParseCMD(const CmdOptionInfo* aCommands, int iCommandsNum, CmdCurrentOption& Current, int argc, char** argv) {
        for( int i = 1; i < argc; i++ ) {
            const char* sCurrent = argv[i];
        }

        return false;
    }
}
