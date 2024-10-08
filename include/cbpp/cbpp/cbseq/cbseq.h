#ifndef CBPP_ASSET_CBSEQ_H
#define CBPP_ASSET_CBSEQ_H

#define CBSEQ_ACTUAL_VERSION 1

#include <map>
#include <cstdint>
#include <cctype>
#include <stack>

#include "cbpp/vec2.h"
#include "cbpp/cbseq/cbseq_base.h"

namespace cbpp {
    const char* CBSEQ_GetCommandName(CBPP_CBSEQ_COMMAND comid);

    CBSEQ_command_info_t CBSEQ_GetCommandInfo(const char* cname);

    extern const CBSEQ_command_info_t cbseq_commap[];

    class SequenceScript {
        public:
            SequenceScript() = default;

            void Interprete(std::string& source, bool do_calls = false);

            void SetCameraSpot(int32_t spot_id, Vec2 spot_pos);
            Vec2 GetCameraSpot(int32_t spot_id);

            bool Update();
            void Unlock();

            void SetTriggerCallback( void (*callback_ptr)( const char* ) );

            void GetBlocksNames(cbseq_words_t& out);
            CBSEQ_Block_t GetBlockCode(std::string& block_name);

            std::string GetErrorLog();

            void PrintDebug();

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
            void ParseCommandBlock(std::string& block_name, std::string& block_src, uint8_t recursion_depth = 0, CBPP_CBSEQ_BLOCK_TYPE blk_typ = CBSEQ_BTYPE_GENERIC, uint16_t exec_counter = 1);
            void CallBlock(std::string& block_name);
            void ExecCommand(CBSEQ_ccom_t& cmd); //execute run-time command

            
    };
}

#endif