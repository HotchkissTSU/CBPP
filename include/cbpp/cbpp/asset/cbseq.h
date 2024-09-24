#ifndef CBPP_ASSET_CBSEQ_H
#define CBPP_ASSET_CBSEQ_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <cctype>
#include <stack>

#include "cbpp/vec2.h"

namespace cbpp {
    enum CBPP_CBSEQ_COMMAND : uint16_t {
        CBSEQ_COM_INVALID,

        //parse-time commands
        CBSEQ_COM_BLOCK,
        CBSEQ_COM_TYPE,
        CBSEQ_COM_CHAR, 
        CBSEQ_COM_VERSION,
        CBSEQ_COM_USECG,

        //TYPE_SEQUENCE
        CBSEQ_COM_CHARPOS,
        CBSEQ_COM_CHARSHOW,
        CBSEQ_COM_CHARFADE,
        CBSEQ_COM_CHARNAME,
        CBSEQ_COM_CHARNAMECOLOR,

        CBSEQ_COM_INTERP,
        CBSEQ_COM_WAIT,
        CBSEQ_COM_AUTOFADE,
        CBSEQ_COM_AUTOSCALE,

        CBSEQ_COM_SETSPOT, 
        CBSEQ_COM_SAY,
        CBSEQ_COM_SPRITE,

        CBSEQ_COM_CG,

        CBSEQ_COM_CALLIF,
        CBSEQ_COM_CALLIFN,
        CBSEQ_COM_CALL,
        CBSEQ_COM_GOTO,
        CBSEQ_COM_GOTOMARK,
        CBSEQ_COM_EXIT,
        CBSEQ_COM_IF,
        CBSEQ_COM_IFNOT,
        CBSEQ_COM_RETURN,
        CBSEQ_COM_TRIGGER,
        CBSEQ_COM_CAMFOLLOW,

        //TYPE_CHARACTER
        CBSEQ_COM_NAME,
        CBSEQ_COM_CDSPRITES, 

        CBSEQ_NUM_COMMANDS
    };

    const char* CBSEQ_GetCommandName(CBPP_CBSEQ_COMMAND comid);

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

    enum CBPP_CBSEQ_BLOCK_TYPE : uint8_t {
        CBSEQ_BTYPE_GENERIC,
        CBSEQ_BTYPE_IF,
        CBSEQ_BTYPE_ENUM
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

    CBSEQ_arg_t CBSEQ_ResolveString(std::string& str);

    typedef std::vector<const char*> cbseq_words_t; //due to STL`s shitness, we can`t store strings inside a vector (something mystical happens and strings corrupt)
    void CBSEQ_ClearWords(cbseq_words_t& words);    //always clear these vectors after use!

    bool CBSEQ_IsNumber(std::string& text);

    void CBSEQ_SplitString(std::string& input, cbseq_words_t& vector_ref); //split string by spaces, taking "quoted like this" parts into account
    void CBSEQ_SplitCommands(std::string& input, cbseq_words_t& vector_ref); //split whole source code by semicolons, taking {block;code} into account
    void CBSEQ_Sanitize(std::string& text, std::string& out_ref);   //remove all unnecessary characters and commentaries

    struct CBSEQ_Block_t {
        CBPP_CBSEQ_BLOCK_TYPE blk_type = CBSEQ_BTYPE_GENERIC;
        std::vector<CBSEQ_ccom_t> prog;
    };

    struct CBSEQ_CallPoint_t {
        std::string block_name;
        uint64_t block_addr;

        bool operator==(CBSEQ_CallPoint_t& other) {
            return (block_addr == other.block_addr) && (block_name == other.block_name);
        }

        bool operator!=(CBSEQ_CallPoint_t& other) {
            return (block_addr != other.block_addr) || (block_name != other.block_name);
        }
    };

    class SequenceScript {
        public:
            SequenceScript() = default;

            void Interprete(std::string& source, bool do_calls = false);

            void TellCameraSpot(int32_t spot_id, Vec2 spot_pos);
            Vec2 GetCameraSpot(int32_t spot_id);

            bool Update();
            void Unlock();

            void SetTriggerCallback( void (*callback_ptr)( const char* ) );

            void GetBlocksNames(cbseq_words_t& out);
            CBSEQ_Block_t GetBlockCode(std::string& block_name);

            ~SequenceScript() = default;
            
        private:
            void (*trigger_callback)(const char*) = NULL;

            float interp_time = 1.0f;
            Vec2 current_camera_pos, target_camera_pos;

            bool sc_run = false, sc_error = false, sc_terminated = false, sc_allow_recursion = false;

            std::stack<CBSEQ_CallPoint_t> sc_call_stack;
            CBSEQ_CallPoint_t sc_check_cpt;

            uint64_t exec_pointer = 0;
            std::string exec_block = "main";

            std::vector<std::string> sc_errlog;

            uint8_t block_recursion_limit = 64;

            CBPP_CBSEQ_SCRIPT_TYPE sc_type = CBSEQ_TYPE_INVALID;

            std::map<int32_t, Vec2> sc_spots; //switchable camera spots
            std::map<std::string, CBSEQ_Block_t> sc_blocks; //code blocks
            std::map<std::string, CBSEQ_CallPoint_t> sc_flags; //flags for the 'goto' command
            cbseq_words_t sc_triggers;

            CBSEQ_ccom_t ParseCommandLine(cbseq_words_t& line);
            void ParseCommandBlock(std::string& block_name, std::string& block_src, uint8_t recursion_depth = 0, CBPP_CBSEQ_BLOCK_TYPE blk_typ = CBSEQ_BTYPE_GENERIC);
            void CallBlock(std::string& block_name);
            void ExecCommand(CBSEQ_ccom_t& cmd); //execute run-time command
    };
}

#endif