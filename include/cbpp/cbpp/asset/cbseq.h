#ifndef CBPP_ASSET_CBSEQ_H
#define CBPP_ASSET_CBSEQ_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <cctype>

#include "cbpp/vec2.h"

namespace cbpp {
    enum CBPP_CBSEQ_COMMAND : uint16_t {
        CBSEQ_COM_INVALID,
        CBSEQ_COM_TYPE,
        CBSEQ_COM_INTERP,

        CBSEQ_COM_CHAR, //TYPE_SEQUENCE
        CBSEQ_COM_CHARPOS,
        CBSEQ_COM_CHARSHOW,
        CBSEQ_COM_CHARFADE,
        CBSEQ_COM_CHARNAME,
        CBSEQ_COM_CHARNAMECOLOR,

        CBSEQ_COM_SETSPOT, 
        CBSEQ_COM_SAY,
        CBSEQ_COM_SETSPRITE,

        CBSEQ_COM_NAME,
        CBSEQ_COM_CDSPRITES, //TYPE_CHARACTER
        CBSEQ_COM_SPRITE,

        CBSEQ_COM_BLOCK,
        CBSEQ_COM_CALLIF,

        CBSEQ_NUM_COMMANDS
    };

    struct CBSEQ_command_info_t {
        CBPP_CBSEQ_COMMAND comId;
        uint16_t argnum;
    };

    extern std::map<std::string, CBSEQ_command_info_t> cbseq_commap;

    enum CBPP_CBSEQ_SCRIPT_TYPE : uint8_t {
        CBSEQ_TYPE_INVALID,
        CBSEQ_TYPE_SEQUENCE,
        CBSEQ_TYPE_CHARACTER
    };

    enum CBPP_CBSEQ_COM_ARG_TYPE : uint8_t {
        CBSEQ_VTYPE_NUMBER,
        CBSEQ_VTYPE_STRING
    };

    struct CBSEQ_arg_t {
        CBPP_CBSEQ_COM_ARG_TYPE argType = CBSEQ_VTYPE_NUMBER;
        //union {
            float numValue = 0.0f;
            std::string strValue;   
        //} argValue;

        ~CBSEQ_arg_t() = default;
    };

    struct CBSEQ_ccom_t { //parsed command line
        CBPP_CBSEQ_COMMAND comId = CBSEQ_COM_INVALID;
        std::vector< CBSEQ_arg_t > args;

        
    };

    typedef std::vector<std::string> cbseq_words_t;

    bool CBSEQ_IsNumber(std::string text);

    cbseq_words_t CBSEQ_SplitString(std::string& input); //split string by spaces, taking "quoted like this" parts into account
    std::string CBSEQ_Sanitize(std::string& text, std::string& out_ref);   //remove all unnecessary characters and commentaries

    struct CBSEQ_Block {
        std::vector<CBSEQ_ccom_t> prog;
    };

    class SequenceScript {
        public:
            SequenceScript() = default;

            bool Interprete(std::string source);

            CBSEQ_ccom_t ParseCommandLine(cbseq_words_t& line);

            void Execute();
            void TellCameraSpot(int32_t spot_id, Vec2 spot_pos);
            Vec2 GetCameraSpot(int32_t spot_id);

            void UpdateCameraPos();

            void SetProgram();
            
        private:
            float interp_time = 1.0f;
            Vec2 current_camera_pos, target_camera_pos;

            uint64_t exec_pointer = 0;
            std::string exec_block = "main";

            std::map<int32_t, Vec2> sc_spots; //switchable camera spots
            std::map<std::string, CBSEQ_Block> sc_blocks; //code blocks
    };
}

#endif