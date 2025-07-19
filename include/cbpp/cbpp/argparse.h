#ifndef CBPP_ARGPARSE_H
#define CBPP_ARGPARSE_H

namespace cbpp {
    struct CmdOptionInfo {
        char* sCommandFull;
        char cCommandShort;
        bool bHasArgument;
    };

    struct CmdCurrentOption {
        char cCommandShort;
        char* sCommandArgument;
        int iCurrentIndex;
    };

    bool ParseCMD(const CmdOptionInfo* aCommands, int iCommandsNum, CmdCurrentOption& pCurrent, int argc, char** argv);
}

#endif
