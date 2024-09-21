#include "cbpp/asset/cbseq.h"

#include <stdio.h>
#include "cbpp/misc.h"

#define CBSEQ_ERR(e_format, ...) { sc_error = true; char err[512]; snprintf(err, 511, e_format, __VA_ARGS__); sc_errlog.push_back( std::string(err) ); }

namespace cbpp {
    std::map<std::string, CBSEQ_command_info_t> cbseq_commap {

        {"type", {CBSEQ_COM_TYPE, 1}},
        {"version", {CBSEQ_COM_VERSION, 1}},
        {"char", {CBSEQ_COM_CHAR, 2}},
        {"wait", {CBSEQ_COM_WAIT, 0}},
        {"charpos", {CBSEQ_COM_CHARPOS, 2}},
        {"interp", {CBSEQ_COM_INTERP, 1}},
        {"charshow", {CBSEQ_COM_CHARSHOW, 1}},
        {"charfade", {CBSEQ_COM_CHARFADE, 1}},
        {"charname", {CBSEQ_COM_CHARNAME, 2}},
        {"charnamecolor", {CBSEQ_COM_CHARNAMECOLOR, 2}},
        {"setspot", {CBSEQ_COM_SETSPOT, 1}},
        {"say", {CBSEQ_COM_SAY, 3}},
        {"setsprite", {CBSEQ_COM_SETSPRITE, 2}},
        {"name", {CBSEQ_COM_NAME, 1}},
        {"cdsprites", {CBSEQ_COM_CDSPRITES, 1}},
        {"sprite", {CBSEQ_COM_SPRITE, 2}},
        {"autofade", {CBSEQ_COM_AUTOFADE, 1}},
        {"autoscale", {CBSEQ_COM_AUTOSCALE, 1}},

        {"block", {CBSEQ_COM_BLOCK, 1}},
        {"callif", {CBSEQ_COM_CALLIF, 2}},
        {"callifn", {CBSEQ_COM_CALLIFN, 2}},
        {"call", {CBSEQ_COM_CALL, 1}},
        {"exit", {CBSEQ_COM_EXIT, 0}},
        {"goto", {CBSEQ_COM_GOTO, 1}},
        {"flag", {CBSEQ_COM_GOTOMARK, 1}}
    };

    bool CBSEQ_IsNumber(std::string text) {
        for(uint64_t i = 0; i < text.length(); i++) {
            char cur = text[i];

            if(cur != '-' || cur != '.' || !std::isdigit(cur)) {
                return false;
            }
        }

        return true;
    }

    void CBSEQ_SplitString(std::string& input, cbseq_words_t& out) {
        out.clear();
        std::string tmp;

        bool is_quoted = false;
        int is_scoped = 0;

        for(std::size_t i = 0; i < input.size()+1; i++){
			if(i == input.size()){
				out.push_back( strdup(tmp.c_str()) );
				break;
			}

            if(input[i] == '{'){ is_scoped++; }
            if(input[i] == '}'){ is_scoped--; }

            if((input[i] == '"') && ((i > 0 && input[i-1] != '\\') || i == 0)) {
                is_quoted = !is_quoted;
            }

            if((input[i] == ' ') && !is_quoted && (is_scoped == 0)) {
                out.push_back( strdup(tmp.c_str()) );
                tmp.clear();
            }else if( (input[i] != '"') && ((i > 0 && input[i-1] != '\\') || i == 0) || (is_scoped != 0)) {
                if(input[i] != '{' && input[i] != '}' || (is_scoped != 1)){ tmp = tmp + input[i]; }
            }
        }
    }

    void CBSEQ_SplitCommands(std::string& input, cbseq_words_t& out) {
        out.clear();
        std::string tmp;

        //printf("INPUT: %s\n", input.c_str());

        int is_quoted = 0;

        for(std::size_t i = 0; i < input.size()+1; i++){
			if(i == input.size()){
				out.push_back( strdup(tmp.c_str()) );
				break;
			}

            if(input[i] == '{'){ is_quoted++; }
            if(input[i] == '}'){ is_quoted--; }

            if((input[i] == ';') && (is_quoted == 0)) {
                out.push_back( strdup(tmp.c_str()) );
                tmp.clear();
            }else{
                tmp = tmp + input[i];
            }
		}
    }

    void CBSEQ_Sanitize(std::string& input, std::string& out) {
        out.clear();

        uint64_t string_ptr = 0;
        uint64_t out_ptr = 0;

        bool inside_command = false;

        while (string_ptr < input.length()) {
            char current = input[string_ptr];
            if(current == '\t') {
                current = ' ';
            }

            if(current != ' ' && current != '\t') {
                inside_command = true;
            }

            if((current != '\n') && (current != '\t') && (current != '/') && inside_command) {
                out.push_back(current);
                out_ptr++;
            }

            if(current == ';' || current == '{') {
                inside_command = false;
            }

            if(current == '/') {
                char tmp = 'a';
                while (tmp != '\n') {
                    string_ptr++;
                    tmp = input[string_ptr];
                }
            }else{
                string_ptr++;
            }
        }
    }

    void CBSEQ_ClearWords(cbseq_words_t& wds) {
        for(uint64_t i = 0; i < wds.size(); i++) {
            delete[] wds[i];
        }
    }

    CBSEQ_ccom_t SequenceScript::ParseCommandLine(cbseq_words_t& cmline){
        CBSEQ_ccom_t out;

        if(cmline.size() < 1) {
            return out;
        }

        std::string tc = cmline[0];
        
        uint16_t comId = cbseq_commap[tc].comId;
        uint16_t arg_num = cbseq_commap[tc].argnum;
        //printf("\n\nCOM ID EQUALS %d\n\n", comId);
        out.comId = (CBPP_CBSEQ_COMMAND)comId;

        if(comId == CBSEQ_COM_INVALID) {
            CBSEQ_ERR("Undefined command: '%s'", tc.c_str());
            return out;
        }

        if( (cmline.size()-1) < arg_num ) {
            CBSEQ_ERR("Too less arguments passed to the command '%s' - %d/%d", tc.c_str(), arg_num, cmline.size()-1);
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
                    //here we must tell the game to load character`s sprites
                    break;

                case CBSEQ_COM_BLOCK:
                    if(call_blocks) {
                        this->ParseCommandBlock(ccom.args[0].strValue, ccom.args[1].strValue, 1);
                    }
                    break;

                default:
                    //if we are here, it means that current command can`t be present outside of any block
                    //this must cause an error!
                    CBSEQ_ERR("Command '%s' can not be present outside of any block", com_decomp[0]);
                    break;
            }
        }
    }

    void SequenceScript::ParseCommandBlock(std::string& block_name, std::string& block_source_, uint8_t recursion_depth) {
        std::string block_source;
        CBSEQ_Sanitize(block_source_, block_source);

        //printf("\n\n\t\t===COMBLOCK %s===\n%s\n\n", block_name.c_str(), block_source.c_str());

        cbseq_words_t coms_raw;
        cbseq_words_t com_decomp;
        std::string buff;
        CBSEQ_SplitCommands(block_source, coms_raw);
        CBSEQ_Block_t seq_block;

        for(uint64_t i = 0; i < coms_raw.size(); i++) {
            buff = coms_raw[i];
            CBSEQ_SplitString(buff, com_decomp);
            CBSEQ_ccom_t ccom = this->ParseCommandLine(com_decomp);
            
            if(ccom.comId == CBSEQ_COM_BLOCK){
                if(recursion_depth <= block_recursion_limit) {
                    //printf("REGISTERED BLOCK CALL: REC DEPTH: %d/%d NAME: %s SRC: %s\n", recursion_depth, block_recursion_limit, 
                    //ccom.args[0].strValue.c_str(), ccom.args[1].strValue.c_str());
                    this->ParseCommandBlock(ccom.args[0].strValue, ccom.args[1].strValue, recursion_depth + 1);
                }
            }else if(ccom.comId != CBSEQ_COM_INVALID){
                seq_block.prog.push_back(ccom);
            }       
        }

        if(sc_blocks.count(block_name) <= 0){
            sc_blocks[block_name] = seq_block;
        }else{
            //this is bad! throw an error!
            CBSEQ_ERR("Undefined reference to block '%s'", block_name.c_str());
        }
    }

    void SequenceScript::CallBlock(std::string& blk_name) {
        if(sc_blocks.count(blk_name) <= 0) {
            CBSEQ_ERR("Undefined reference to block '%s'", blk_name.c_str());
            return;
        }

        CBSEQ_CallPoint_t cpt;
        cpt.block_addr = exec_pointer + 1;
        cpt.block_name = exec_block;

        sc_call_stack.push(cpt);

        exec_pointer = 0;
        exec_block = blk_name;
    }

    void SequenceScript::Unlock() {
        sc_run = true;
    }

    void SequenceScript::ExecCommand(CBSEQ_ccom_t& cmd) {
        switch (cmd.comId) {
            case CBSEQ_COM_CALLIF:
                //here we must grab a glob-variable and check its state
                this->CallBlock(cmd.args[1].strValue);
                break;
        }
    }

    bool SequenceScript::Update() {
        CBSEQ_Block_t cblk;
        if(sc_blocks.count(exec_block) > 0) {
            cblk = sc_blocks[exec_block];
        }else{
            CBSEQ_ERR("Undefined reference to block '%s'", exec_block.c_str());
            return false;
        }

        CBSEQ_ccom_t& cur_com = cblk.prog[exec_pointer];
        CBSEQ_CallPoint_t cur_cpt = { exec_block, exec_pointer };

        if(exec_pointer < cblk.prog.size() && sc_run) {
            if( !(cur_cpt == sc_check_cpt) && sc_run ) {
                sc_check_cpt = cur_cpt;
                //here we execute the current command
                this->ExecCommand(cur_com);
                printf("BLOCK '%s' : Executed command: %d\n", exec_block.c_str(), cur_com.comId);
            }

            exec_pointer++;
        }else {
            if(!sc_call_stack.empty()) {
                CBSEQ_CallPoint_t prev_cpt = sc_call_stack.top();
                sc_call_stack.pop();
                exec_pointer = prev_cpt.block_addr;
                exec_block = prev_cpt.block_name;
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

    CBSEQ_Block_t SequenceScript::GetBlockCode(std::string& block_name) {
        return sc_blocks.at(block_name);
    }
}