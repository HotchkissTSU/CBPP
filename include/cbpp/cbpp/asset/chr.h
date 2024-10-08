#ifndef CBPP_CBSEQ_CHR_H
#define CBPP_CBSEQ_CHR_H

#include <cstdint>
#include <string>
#include <map>

#define CHR_ACTUAL_VERSION 1

namespace cbpp {
    enum CHR_COMMMAND : uint16_t {
        CHR_COM_INVALID,

        CHR_COM_VERSION,

        CHR_COM_ANIM,
        CHR_COM_SPRITE,
        CHR_COM_LOOP,

        CHR_COM_NAME,
        CHR_COM_COLOR,

        CHR_NUM_COMMANDS
    };

    struct CHR_command_info {
        const char* name = NULL;
        CHR_COMMAND comId = CHR_COM_INVALID;
        uint16_t agrnum = 0;
    };

    struct CHR_anim_t {
        bool loop = false;
    };

    extern CHR_command_info chr_commap[];

    class SeqCharacter {
        public:
            SeqCharacter() = default;

            bool Load(std::string& src);

            void SetVisible(bool vis);
            void SetAnimation(std::string& anim_name);

        private:
            std::map<std::string, CHR_anim_t> sc_anims;

            CBSEQ_ccom_t SeqCharacter::ParseCommandLine( cbseq_words_t& com_decomp );
    };
}

#endif