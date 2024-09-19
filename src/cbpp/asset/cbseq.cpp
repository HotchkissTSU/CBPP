#include "cbpp/asset/cbseq.h"

#include <stdio.h>

#include "cbpp/misc.h"

namespace cbpp {
    std::map<std::string, CBSEQ_command_info_t> cbseq_commap {

        {"type", {CBSEQ_COM_TYPE, 1}},
        {"char", {CBSEQ_COM_CHAR, 2}},
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

        {"block", {CBSEQ_COM_BLOCK, 1}}
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

    cbseq_words_t CBSEQ_SplitString(std::string& input) {
        std::string tmp;
        cbseq_words_t out;

        bool is_quoted = false;

        for(std::size_t i = 0; i < input.size()+1; i++){
			if(i == input.size()){
				out.push_back(tmp);
				break;
			}

            if(input[i] == '"' && (i > 0 && input[i-1] != '\\')) {
                is_quoted = !is_quoted;
            }

            if((input[i] == ' ' || input[i] == '{' || input[i] == '}') && !is_quoted) {
                out.push_back(tmp);
                tmp.clear();
            }else if( input[i] != '"' && ((i > 0 && input[i-1] != '\\') || i == 0) ) {
                tmp = tmp + input[i];
            }
		}

        return out;
    }

    std::string CBSEQ_Sanitize(std::string& input, std::string& out) {
        out.clear();

        uint64_t string_ptr = 0;
        uint64_t out_ptr = 0;

        while (string_ptr < input.length()) {
            char current = input[string_ptr];

            if(current != '\n' && current != '/') {
                out.push_back(current);
                out_ptr++;
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

        return out;
    }

    CBSEQ_ccom_t SequenceScript::ParseCommandLine(cbseq_words_t& cmline){
        CBSEQ_ccom_t out;

        char err_log[512];
        int16_t log_ptr = 0;

        if(cmline.size() < 1) {
            //invalid command line!
        }

        std::string tc = cmline[0];
        uint16_t comId = cbseq_commap[tc].comId;
        uint16_t arg_num = cbseq_commap[tc].argnum;

        if(comId == CBSEQ_COM_INVALID) {
            //please do something with this!!!
        }

        if( (cmline.size() - 1) < arg_num ) {
            //please do something with this!!!
        }

        if(comId == CBSEQ_COM_BLOCK) {
            
        }

        std::vector<CBSEQ_arg_t> cm_args;
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

            cm_args.push_back(argument);
        }

        out.args = cm_args;

        return out;
    }
}