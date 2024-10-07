#ifndef CBPP_CBSEQ_BASE_H
#define CBPP_CBSEQ_BASE_H

#include <string>
#include <vector>
#include <cstdint>

namespace cbpp {
    enum CBPP_CBSEQ_COMMAND : uint16_t {
        CBSEQ_COM_INVALID,

        //parse-time commands
        CBSEQ_COM_BLOCK,
        CBSEQ_COM_TYPE,
        CBSEQ_COM_CHAR, 
        CBSEQ_COM_VERSION,
        CBSEQ_COM_USECG,
        CBSEQ_COM_USESND,

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

        CBSEQ_COM_BPOINT,

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
        CBSEQ_COM_CALLBACK,
        CBSEQ_COM_CAMFOLLOW,
        CBSEQ_COM_REPEAT,

        CBSEQ_COM_MUSIC,
        CBSEQ_COM_AMBIENT,

        //TYPE_CHARACTER
        CBSEQ_COM_NAME,
        CBSEQ_COM_CDSPRITES, 

        CBSEQ_NUM_COMMANDS
    };

    struct CBSEQ_command_info_t {
        const char* name;
        CBPP_CBSEQ_COMMAND comId;
        uint16_t argnum;
    };

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
        CBSEQ_BTYPE_LOOP
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

    struct CBSEQ_Block_t {
        CBPP_CBSEQ_BLOCK_TYPE blk_type = CBSEQ_BTYPE_GENERIC;
        std::vector<CBSEQ_ccom_t> prog;

        uint16_t loop_exec_counter = 0, loop_iter_num = 1;
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

    typedef std::vector<const char*> cbseq_words_t; //due to STL`s shitness, we can`t store strings inside a vector (something mystical happens and they corrupt)
    void CBSEQ_ClearWords(cbseq_words_t& words);    //always clear these vectors after use!

    bool CBSEQ_IsNumber(std::string& text);

    void CBSEQ_SplitString(std::string& input, cbseq_words_t& vector_ref); //split string by spaces, taking "quoted like this" parts into account
    void CBSEQ_SplitCommands(std::string& input, cbseq_words_t& vector_ref); //split whole source code by semicolons, taking {block;code} into account
    void CBSEQ_Sanitize(std::string& text, std::string& out_ref);   //remove all unnecessary characters and commentaries
}

#endif