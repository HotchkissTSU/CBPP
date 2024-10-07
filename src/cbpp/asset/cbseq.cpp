#include "cbpp/asset/cbseq.h"

#include <stdio.h>
#include <math.h>
#include "cbpp/misc.h"

#define CBSEQ_ERR(e_format, ...) { sc_error = true; sc_terminated = true; char err[512]; snprintf(err, 511, e_format, __VA_ARGS__); sc_errlog.push_back( std::string(err) ); }

namespace cbpp {
    const CBSEQ_command_info_t cbseq_commap[] = {
        {"type", CBSEQ_COM_TYPE, 1},
        {"version", CBSEQ_COM_VERSION, 1},
        {"usesnd", CBSEQ_COM_USESND, 2},
        {"char", CBSEQ_COM_CHAR, 2},
        {"usecg", CBSEQ_COM_USECG, 2},
        {"cg", CBSEQ_COM_CG, 1},
        {"wait", CBSEQ_COM_WAIT, 0},
        {"charpos", CBSEQ_COM_CHARPOS, 2},
        {"interp", CBSEQ_COM_INTERP, 1},
        {"charshow", CBSEQ_COM_CHARSHOW, 1},
        {"charfade", CBSEQ_COM_CHARFADE, 1},
        {"charname", CBSEQ_COM_CHARNAME, 2},
        {"charnamecolor", CBSEQ_COM_CHARNAMECOLOR, 4},
        {"camspot", CBSEQ_COM_SETSPOT, 1},
        {"say", CBSEQ_COM_SAY, 2},
        {"sprite", CBSEQ_COM_SPRITE, 2},
        {"name", CBSEQ_COM_NAME, 1},
        {"cdsprites", CBSEQ_COM_CDSPRITES, 1},
        {"autofade", CBSEQ_COM_AUTOFADE, 1},
        {"autoscale", CBSEQ_COM_AUTOSCALE, 1},

        {"block", CBSEQ_COM_BLOCK, 1},
        {"callif", CBSEQ_COM_CALLIF, 2},
        {"callifn", CBSEQ_COM_CALLIFN, 2},
        {"call", CBSEQ_COM_CALL, 1},
        {"exit", CBSEQ_COM_EXIT, 0},
        {"if", CBSEQ_COM_IF, 2},
        {"ifnot", CBSEQ_COM_IFNOT, 2},
        {"return", CBSEQ_COM_RETURN, 0},
        {"trigger", CBSEQ_COM_TRIGGER, 1},
        {"camfollow", CBSEQ_COM_CAMFOLLOW, 1},
        {"bpoint", CBSEQ_COM_BPOINT, 0},
        {"repeat", CBSEQ_COM_REPEAT, 2},
        {"callback", CBSEQ_COM_CALLBACK, 2},

        {"music", CBSEQ_COM_MUSIC, 2},
        {"ambient", CBSEQ_COM_AMBIENT, 3}
    };

    constexpr uint16_t cbseq_commap_length = sizeof(cbseq_commap) / sizeof(CBSEQ_command_info_t);

    const char* CBSEQ_GetCommandName(CBPP_CBSEQ_COMMAND comid) {
        for(uint16_t i = 0; i < cbseq_commap_length; i++) {
            if( comid == cbseq_commap[i].comId ) {
                return cbseq_commap[i].name;
            }
        }

        return "UNDEFINED";
    }

    CBSEQ_command_info_t CBSEQ_GetCommandInfo(const char* cname){
        CBSEQ_command_info_t out;
        out.comId = CBSEQ_COM_INVALID;

        for(uint16_t i = 0; i < cbseq_commap_length; i++) {
            if( strcmp(cname, cbseq_commap[i].name) == 0 ) {
                out = cbseq_commap[i];
            }
        }

        return out;
    }

    CBSEQ_ccom_t SequenceScript::ParseCommandLine(cbseq_words_t& cmline){
        CBSEQ_ccom_t out;

        if(cmline.size() < 1) {
            return out;
        }

        const char* tc = cmline[0];
        CBSEQ_command_info_t cinf = CBSEQ_GetCommandInfo(tc);
        
        CBPP_CBSEQ_COMMAND comId = cinf.comId;
        uint16_t arg_num = cinf.argnum;
        //printf("\n\nCOM ID EQUALS %d\n\n", comId);
        out.comId = comId;

        if(comId == CBSEQ_COM_INVALID) {
            CBSEQ_ERR("Undefined command: '%s'", tc);
            return out;
        }

        if( (cmline.size()-1) < arg_num ) {
            CBSEQ_ERR("Too less arguments are passed to the command '%s' - %d/%d", tc, arg_num, cmline.size()-1);
            return out;
        }

        for(uint16_t i = 1; i < cmline.size(); i++) {
            std::string arg = cmline[i];
            CBSEQ_arg_t argument;

            if( CBSEQ_IsNumber(arg) ) {
                argument.argType = CBSEQ_VTYPE_NUMBER;
                argument.numValue = (float)atof(arg.c_str());
            }else{
                argument.argType = CBSEQ_VTYPE_STRING;
                argument.strValue = arg;
            }

            out.args.push_back(argument);
        }

        return out;
    }

    void SequenceScript::Interprete(std::string& block_src, bool call_blocks) {
        std::string src_sanitized;
        CBSEQ_Sanitize(block_src, src_sanitized);

        //printf("SANITIZED SRC: %s\n", src_sanitized.c_str());

        cbseq_words_t coms_raw;
        cbseq_words_t com_decomp;
        std::string buff;
        CBSEQ_SplitCommands(src_sanitized, coms_raw);

        for(uint64_t i = 0; i < coms_raw.size(); i++) {
            buff = coms_raw[i];
            //printf("cm %d: %s\n", i, buff);
            CBSEQ_ClearWords(com_decomp);
            CBSEQ_SplitString(buff, com_decomp);

            CBSEQ_ccom_t ccom = this->ParseCommandLine(com_decomp);

            switch( ccom.comId ) {
                case CBSEQ_COM_TYPE:
                    if(ccom.args[0].strValue == "sequence") {
                        sc_type = CBSEQ_TYPE_SEQUENCE;
                    }else if(ccom.args[0].strValue == "character") {
                        sc_type = CBSEQ_TYPE_CHARACTER;
                    }
                    break;

                case CBSEQ_COM_CHAR:
                    //TODO
                    //here we must poke the game to load character`s sprites
                    break;

                case CBSEQ_COM_VERSION:
                    if( (int)ccom.args[0].numValue != CBSEQ_ACTUAL_VERSION ) {
                        CBSEQ_ERR("Version mismatch: actual one is %d, got %d", CBSEQ_ACTUAL_VERSION, (int)ccom.args[0].numValue);
                    }
                    break;

                case CBSEQ_COM_USECG:
                    break;

                case CBSEQ_COM_USESND:
                    break;

                case CBSEQ_COM_BLOCK:
                    if(call_blocks) {
                        this->ParseCommandBlock(ccom.args[0].strValue, ccom.args[1].strValue, 1, CBSEQ_BTYPE_GENERIC);
                    }
                    break;

                default:
                    CBSEQ_ERR("Command '%s' can not be present outside of any block", com_decomp[0]);
                    break;
            }
        }

        CBSEQ_ClearWords(coms_raw);
    }

    void SequenceScript::ParseCommandBlock(std::string& block_name, std::string& block_source_, uint8_t recursion_depth, CBPP_CBSEQ_BLOCK_TYPE blk_typ, uint16_t exec_cnt) {
        std::string block_source;
        CBSEQ_Sanitize(block_source_, block_source);

        cbseq_words_t coms_raw;
        cbseq_words_t com_decomp;
        std::string buff;
        CBSEQ_SplitCommands(block_source, coms_raw);
        CBSEQ_Block_t seq_block;

        seq_block.blk_type = blk_typ;
        seq_block.loop_iter_num = exec_cnt;

        for(uint64_t i = 0; i < coms_raw.size(); i++) {
            buff = coms_raw[i];
            CBSEQ_ClearWords(com_decomp);
            CBSEQ_SplitString(buff, com_decomp);
            CBSEQ_ccom_t ccom = this->ParseCommandLine(com_decomp);

            switch (ccom.comId) { //these commands are not removed from the run-time code
                case CBSEQ_COM_TRIGGER:{
                    const char* arg = ccom.args[0].strValue.c_str();
                    if(arg != "") {
                        sc_triggers.push_back( arg );
                    }
                    break;
                }
            }

            switch (ccom.comId) { //these commands does not exist during the run-time
                case CBSEQ_COM_BLOCK:
                    if(recursion_depth <= block_recursion_limit) {
                        this->ParseCommandBlock(ccom.args[0].strValue, ccom.args[1].strValue, recursion_depth + 1, CBSEQ_BTYPE_GENERIC, 1);
                    }
                    break;

                case CBSEQ_COM_IF:{ //IF`s are basically syntax sugar, and they are replaced by a simple callif command and a new generated block
                    if(recursion_depth <= block_recursion_limit) {
                        char blk_name_buffer[64];
                        snprintf(blk_name_buffer, 63, "%s_IF%x", block_name.c_str(), i);
                        std::string str_name(blk_name_buffer);

                        this->ParseCommandBlock(str_name, ccom.args[1].strValue, recursion_depth + 1, CBSEQ_BTYPE_IF);

                        CBSEQ_ccom_t cmd{ CBSEQ_COM_CALLIF, { {CBSEQ_VTYPE_STRING, 0.0f, ccom.args[0].strValue}, {CBSEQ_VTYPE_STRING, 0.0f, str_name.c_str()} } };
                        seq_block.prog.push_back( cmd );
                    }
                    break;
                }

                case CBSEQ_COM_IFNOT:{
                    if(recursion_depth <= block_recursion_limit) {
                        char blk_name_buffer[64];
                        snprintf(blk_name_buffer, 63, "%s_IFN%x", block_name.c_str(), i);
                        std::string str_name(blk_name_buffer);

                        this->ParseCommandBlock(str_name, ccom.args[1].strValue, recursion_depth + 1, CBSEQ_BTYPE_IF, 1);

                        CBSEQ_ccom_t cmd{ CBSEQ_COM_CALLIFN, { {CBSEQ_VTYPE_STRING, 0.0f, ccom.args[0].strValue}, {CBSEQ_VTYPE_STRING, 0.0f, str_name.c_str()} } };
                        seq_block.prog.push_back( cmd );
                    }
                    break;
                }

                case CBSEQ_COM_REPEAT:{
                    if(recursion_depth <= block_recursion_limit) {
                        char blk_name_buffer[64];
                        snprintf(blk_name_buffer, 63, "%s_REPT%x", block_name.c_str(), i);
                        std::string blk_name(blk_name_buffer);

                        this->ParseCommandBlock( blk_name, ccom.args[1].strValue, recursion_depth+1, CBSEQ_BTYPE_LOOP, std::abs(ccom.args[0].numValue) );

                        CBSEQ_ccom_t cmd{ CBSEQ_COM_CALL, { {CBSEQ_VTYPE_STRING, 0.0f, blk_name} } };
                        seq_block.prog.push_back( cmd );
                    }
                    break;
                }

                default:
                    if(ccom.comId != CBSEQ_COM_INVALID){
                        seq_block.prog.push_back(ccom);
                    }
            }     
        }

        CBSEQ_ClearWords(coms_raw);

        if(sc_blocks.count(block_name) <= 0){
            sc_blocks[block_name] = seq_block;
        }else{
            //this is bad! throw an error!
            CBSEQ_ERR("Blocks names duplication: '%s'", block_name.c_str());
        }
    }

    void SequenceScript::CallBlock(std::string& blk_name) {
        if(sc_blocks.count(blk_name) <= 0) {
            CBSEQ_ERR("Undefined reference to block '%s'", blk_name.c_str());
            return;
        }

        if(blk_name == exec_block && !sc_allow_recursion) {
            CBSEQ_ERR("Recursion is forbidden", "");
            return;
        }

        CBSEQ_CallPoint_t cpt;
        cpt.block_addr = exec_pointer + 1;
        cpt.block_name = exec_block;

        sc_call_stack.push(cpt);

        exec_pointer = -1; //MAGIC! exec_pointer somehow gets incremented by 1 somewhere before the actual block execution, so i counter it this way
        exec_block = blk_name;
    }

    void SequenceScript::Unlock() {
        sc_run = true;
    }

    void SequenceScript::ExecCommand(CBSEQ_ccom_t& cmd) {
        switch (cmd.comId) {
            case CBSEQ_COM_CALLIF:
                //here we must grab a glob-variable and check its value
                if(true){
                    this->CallBlock(cmd.args[1].strValue);
                }
                break;

            case CBSEQ_COM_CALL:
                this->CallBlock(cmd.args[0].strValue);
                break;

            case CBSEQ_COM_WAIT:
                sc_run = false;
                break;

            case CBSEQ_COM_RETURN:{
                if(sc_blocks.at(exec_block).blk_type == CBSEQ_BTYPE_IF && sc_call_stack.size() >= 2) {
                    CBSEQ_CallPoint_t cpt;
                    sc_call_stack.pop(); //IF`s technically are hidden blocks, so we need to pop the call stack twice when returning from them
                    cpt = sc_call_stack.top();
                    sc_call_stack.pop();

                    exec_pointer = cpt.block_addr;
                    exec_block = cpt.block_name;
                }else if(sc_call_stack.size() > 0) {
                    CBSEQ_CallPoint_t cpt = sc_call_stack.top();
                    sc_call_stack.pop();

                    exec_pointer = cpt.block_addr-1;
                    exec_block = cpt.block_name;
                }else{
                    CBSEQ_ERR("Nowhere to return: Block '%s', Command '%d'", exec_block, exec_pointer);
                }
                break;
            }

            case CBSEQ_COM_TRIGGER:
                if(trigger_callback != NULL) {
                    trigger_callback( cmd.args[0].strValue.c_str() );
                    sc_run = false;
                }
                break;
                
            case CBSEQ_COM_EXIT:
                CBSEQ_ERR("TERMINATED", "");
                break;
        }
    }

    bool SequenceScript::Update() {
        if(sc_terminated) { return false; }

        CBSEQ_Block_t cblk;
        if(sc_blocks.count(exec_block) > 0) {
            cblk = sc_blocks[exec_block];
        }else{
            CBSEQ_ERR("Undefined reference to block '%s'", exec_block.c_str());
            return false;
        }

        CBSEQ_ccom_t& cur_com = cblk.prog[exec_pointer];
        CBSEQ_CallPoint_t cur_cpt = { exec_block, exec_pointer };
        //printf("[%s, %d] ", exec_block.c_str(), exec_pointer);
        if(exec_pointer < cblk.prog.size() && sc_run) {
            if(cur_cpt != sc_check_cpt){ //do not execute one command more than once
                printf("[%d] %s->%s\n", exec_pointer, exec_block.c_str(), CBSEQ_GetCommandName(cur_com.comId));
                this->ExecCommand(cur_com);
                exec_pointer++;
            }
        }else {
            if(!sc_call_stack.empty()) {
                if(sc_blocks[exec_block].loop_exec_counter >= sc_blocks[exec_block].loop_iter_num - 1) {
                    CBSEQ_CallPoint_t prev_cpt = sc_call_stack.top();
                    sc_call_stack.pop();

                    //sc_blocks[exec_block].loop_exec_counter = 0;
                    exec_pointer = prev_cpt.block_addr;
                    exec_block = prev_cpt.block_name;
                }else{
                    sc_blocks[exec_block].loop_exec_counter++;
                    exec_pointer = 0;
                }
            }else{
                return false;
            }
        }

        return true;
    }

    void SequenceScript::GetBlocksNames(cbseq_words_t& out) {
        for(auto it = sc_blocks.begin(); it != sc_blocks.end(); it++) {
            out.push_back( strdup(it->first.c_str()) );
        }
    }

    std::string SequenceScript::GetErrorLog() {
        std::string out;
        for(uint32_t i = 0; i < sc_errlog.size(); i++) {
            out += sc_errlog[i] + "\n";
        }

        return out;
    }

    CBSEQ_Block_t SequenceScript::GetBlockCode(std::string& block_name) {
        return sc_blocks.at(block_name);
    }

    void SequenceScript::SetTriggerCallback( void (*callback_ptr)( const char* ) ) {
        trigger_callback = callback_ptr;
    }

    void SequenceScript::PrintDebug() {
        cbseq_words_t blk_names;
        this->GetBlocksNames(blk_names);

        for(uint16_t i = 0; i < blk_names.size(); i++) {
            const char* bname = blk_names[i];
            printf("BLOCK '%s':\n", bname);

            CBSEQ_Block_t blk = this->sc_blocks[bname];
            for(uint32_t j = 0; j < blk.prog.size(); j++) {
                CBSEQ_ccom_t ccom = blk.prog[j];
                printf("\t%s ", CBSEQ_GetCommandName(ccom.comId));
                for(uint16_t k = 0; k < ccom.args.size(); k++) {
                    CBSEQ_arg_t arg = ccom.args[k];
                    if(arg.argType == CBSEQ_VTYPE_NUMBER) {
                        printf("%f ", arg.numValue);
                    }else{
                        printf("'%s' ", arg.strValue.c_str());
                    }
                }
                printf("\n");
            }
        }

        CBSEQ_ClearWords(blk_names);
    }
}