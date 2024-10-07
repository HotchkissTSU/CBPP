#include "cbpp/asset/cbseq_base.h"
#include <cstring>

namespace cbpp {
    CBSEQ_arg_t CBSEQ_ResolveString(std::string& str) {
        CBSEQ_arg_t out;

        char first = str[0];

        switch(first) {
            case '#':
                out.argType = CBSEQ_VTYPE_STRING;
                //grab a string from the current locale
                break;

            case '$':
                //grab a GVar from the engine table
                break;

            default:
                out.argType = CBSEQ_VTYPE_STRING;
        }

        return out;
    }

    void CBSEQ_SplitString(std::string& input, cbseq_words_t& out) {
        out.clear();
        CBSEQ_ClearWords(out);
        std::string tmp;

        bool is_quoted = false;
        int is_scoped = 0;

        for(std::size_t i = 0; i < input.size()+1; i++){
			if(i == input.size()){
				out.push_back( strdup(tmp.c_str()) );
				break;
			}

            if(input[i] == '}'){ is_scoped--; }

            if(input[i] == '"') {
                is_quoted = !is_quoted;
            }

            if((input[i] == ' ') && !is_quoted && (is_scoped == 0)) {
                out.push_back( strdup(tmp.c_str()) );
                tmp.clear();
            }else if (input[i] != '{' && input[i] != '}' && input[i] != '"' || (is_scoped >= 1)){
                tmp = tmp + input[i];
            }

            if(input[i] == '{'){ is_scoped++; }
        }
    }

    void CBSEQ_SplitCommands(std::string& input, cbseq_words_t& out) {
        CBSEQ_ClearWords(out);
        out.clear();
        std::string tmp;

        int is_quoted = 0;

        for(std::size_t i = 0; i < input.size()+1; i++){
			if(i == input.size()){
                if(tmp != "") {
				    out.push_back( strdup(tmp.c_str()) );
                }
				break;
			}

            if(input[i] == '{'){ is_quoted++; }
            if(input[i] == '}'){ is_quoted--; }

            if((input[i] == ';') && (is_quoted == 0)) {
                const char* new_str = strdup( tmp.c_str() );
                out.push_back( new_str );
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
        bool is_quoted = false;

        while (string_ptr < input.length()) {
            char current = input[string_ptr];
            if(current == '\t') {
                current = ' ';
            }

            if(current == '"') { is_quoted = !is_quoted; }

            if(current != ' ' && current != '\t') {
                inside_command = true;
            }

            if((current != '\n') && (current != '\t') && (current != '/') && inside_command || is_quoted) {
                out.push_back(current);
                out_ptr++;
            }

            if(current == ';' || current == '{') {
                inside_command = false;
            }

            if(current == '/' && !is_quoted) {
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
            if(wds[i] != NULL) { delete[] wds[i]; }
        }
    }

    bool CBSEQ_IsNumber(std::string& text) {
        char* p;
        strtod(text.c_str(), &p);
        return *p == 0;
    }
}
