#include "cbasset/sfx_asm.h"

namespace cbasset{
	namespace SFX{
		std::string _wipe_spaces(std::wstring str){
			std::string tmp;
			for(std::size_t i = 0; i < str.length(); i++){
				if(str[i] != ' '){
					char tmp_ar[2] = {(char)str[i], 0};
					tmp = tmp + std::string(tmp_ar);
				}
			}
			
			return tmp;
		}
		
		bool _is_number(std::string str){
			bool out = true;
			
			for(std::size_t i = 0; i < str.length(); i++){				
				if(!std::isdigit(str[i])){
					if(str[i] != '.' && str[i] != '-'){
						out = false;
						break;
					}
				}
			}
			
			return out;
		}
		
		std::string _wstring_to_string(std::wstring txt){
			std::string out;
			for(std::size_t i = 0; i < txt.length(); i++){
				char tmp[2] = {(char)((uint8_t)txt[i]), 0};
				out = out + std::string(tmp);
			}
			
			return out;
		}
		
		extern "C" ProgramBlock* ParseSFX(std::string sfx_name){
			ProgramBlock* out = new ProgramBlock();
			out->block_name = "MAIN";
			
			std::wifstream fin(std::string("assets/sfx/") + sfx_name + std::string(".sfx"));
			if(!fin.is_open()){ std::cout<<"no file\n";return out; }
			
			wchar_t ch;
			
			std::wstring raw_text;
			while(fin.get(ch)){
				wchar_t tmp[2] = {ch, 0};
				raw_text = raw_text + std::wstring(tmp);
			}
			
			auto lines = cbpp::SplitString(raw_text, '\n');
			std::wstring format1;
			
			for(std::size_t i = 0; i < lines.size(); i++){
				std::wstring new_line;
				std::wstring line = lines[i];
				
				for(std::size_t j = 0; j < line.length(); j++){ //удаляем из текста комментарии, переводы строк и табуляции
					wchar_t _ch = line[j];
					
					if(_ch == '#'){ break; }
					
					if(_ch != '\n' && _ch != '\t'){
						wchar_t tmp[2] = {_ch, 0};
						new_line = new_line + std::wstring(tmp);
					}
				}
				
				format1 = format1 + new_line;
			}
			
			std::vector<CommandBlock> comms;
			
			auto lines_ = cbpp::SplitString(format1, ';');
			for(std::size_t i = 0; i < lines_.size(); i++){
				std::wstring line = lines_[i];
				
				auto words = cbpp::SplitString(line, ' ');
				CommandBlock blk;
				
				std::string cmd = _wipe_spaces(words[0]);
				blk.is_num_arg = cmd[0] == '.';
				
				if(cmd[0] == '.'){
					std::string tmp_cmd;
					for(std::size_t i = 1; i < cmd.size(); i++){
						tmp_cmd += cmd[i];
					}
					
					cmd = tmp_cmd;
				}
				
				if(cmd.length() > 0){
					blk.command = cmd;
				}
				
				for(std::size_t j = 1; j < words.size(); j++){
					auto arg_word = _wipe_spaces(words[j]);

					ArgValue aval;
					
					if(_is_number(arg_word)){
						float fvalue = atof(arg_word.c_str());
						aval.float_value = fvalue;
						aval.state = true;
					}else{
						std::string svalue = arg_word;
						aval.string_value = svalue;
						aval.state = false;
					}
					
					blk.args.push_back(aval);
				}
				
				comms.push_back(blk);
			}
			
			bool is_block_open = false;
			int64_t block_length = 1;
			int64_t blocks_counter = 0;
			std::string block_name = "MAIN";
			
			ProgramBlock tmp;
			tmp.parent_prog = out;
			
			for(std::size_t i = 0; i < comms.size(); i++){
				CommandBlock blk = comms[i];
				
				auto cmd = blk.command;
				blk.block_name = block_name;
				
				if(!is_block_open && cmd != "subprog" && cmd != "block"){
					out->commands.push_back(blk);
				}else if(cmd != "subprog" && cmd != "block"){
					tmp.commands.push_back(blk);
				}
				
				if(cmd == "subprog"){
					if(blk.args.size() >= 1){
						if(blk.args[0].string_value != "end"){
							is_block_open = true;
							block_name = blk.args[0].string_value;
							
							tmp.commands.clear();
							tmp.sub_progs.clear();
							
							tmp.is_byteblock = false;
							
							for(std::size_t k = 1; k < blk.args.size(); k++){
								tmp.input_names.push_back(blk.args[k].string_value);
							}
							
							tmp.block_name = block_name;
						}else{
							out->sub_progs[block_name] = tmp;
							is_block_open = false;
							block_name = "MAIN";
						}
					}else{
						break;
					}
				}
				
				if(cmd == "inputs"){
					for(std::size_t k = 0; k < blk.args.size(); k++){
						out->input_names.push_back(blk.args[k].string_value);
					}
				}
				
				if(cmd == "block"){
					if(blk.args.size() >= 1){
						if(blk.args[0].string_value != "end"){
							is_block_open = true;
							
							char buff[11];
							itoa(blocks_counter, buff, 10);
							
							block_name = std::string("BLOCK_") + std::string(buff);
							
							blocks_counter++;
							
							tmp.commands.clear();
							tmp.sub_progs.clear();
							
							tmp.is_byteblock = true;
							
							tmp.block_name = block_name;
							block_length = (int)blk.args[1].float_value;
						}else{
							out->sub_progs[block_name] = tmp;
							
							is_block_open = false;
							block_name = "MAIN";
						}
					}else{
						break;
					}
				}
			}
			
			return out;
		}
		
		void ProgramBlock::SetVar(std::string name, float value){
			if(!_is_number(name)){
				if(parent_prog != nullptr){
					parent_prog->block_vars[name] = value;
				}else{
					block_vars[name] = value;
				}
			}
		}
		
		float ProgramBlock::GetVar(std::string name){
			if(parent_prog != nullptr){
				return parent_prog->block_vars[name];
			}else{
				return block_vars[name];
			}
		}
		
		void ProgramBlock::_call(CommandBlock cmd, bool _if){
			std::vector<ArgValue> args = cmd.args;
			if(args.size() >= 1){
				std::string call_target;
				if(args[0].string_value[0] != '!'){
					call_target = args[0].string_value;
					
					if(sub_progs.find(call_target) != sub_progs.end()){
						std::vector<float> call_args;
						
						std::size_t j = 0;
						if(_if){
							j = 2;
						}else{
							j = 1;
						}
						
						for(; j < args.size(); j++){
							if(cmd.is_num_arg){
								call_args.push_back(args[j].float_value);
							}else{
								call_args.push_back(GetVar(args[j].string_value));
							}
						}
						
						sub_progs[call_target].Execute(call_args);
					}
				}else{
					std::string call_tmp;
					for(std::size_t i = 1; i < args[0].string_value.size(); i++){
						call_tmp += args[0].string_value[i];
					}
					call_target = call_tmp;
					
					if(parent_prog != nullptr){
						if(parent_prog->sub_progs.find(call_target) != parent_prog->sub_progs.end()){
							std::vector<float> call_args;
							
							std::size_t j = 0;
							if(_if){
								j = 2;
							}else{
								j = 1;
							}
							
							for(; j < args.size(); j++){
								if(cmd.is_num_arg){
									call_args.push_back(args[j].float_value);
								}else{
									call_args.push_back(GetVar(args[j].string_value));
								}
							}
							
							parent_prog->sub_progs[call_target].Execute(call_args);
						}
					}
				}				
			}
		}
		
		void ProgramBlock::Execute(std::vector<float> _args){
			for(std::size_t i = 0; i < std::min(_args.size(), input_names.size()); i++){ //создаем виртуальные переменные инпутов
				std::cout<<_args[i]<<std::endl;
				SetVar(input_names[i], _args[i]);
			}

			for(std::size_t i = 0; i < commands.size(); i++){
				CommandBlock cmd = commands[i];
				std::string comm = cmd.command;
				std::vector<ArgValue> args = cmd.args;
				
				if(comm == "call"){
					_call(cmd, false);
				}
				
				if(comm == "callif"){
					if(cmd.is_num_arg){
						if(std::abs(args[1].float_value) > EPSILON){
							_call(cmd, true);
						}
					}else{
						if(std::abs(GetVar(args[1].string_value)) > EPSILON){
							_call(cmd, true);
						}
					}
				}
				
				if(comm == "out"){
					for(std::size_t j = 0; j < args.size(); j++){
						auto arg = args[j];
						if(cmd.is_num_arg){
							if(args[j].string_value != "endl"){
								if(arg.state){
									std::cout<<arg.float_value<<" ";
								}else{
									std::cout<<arg.string_value<<" ";
								}
							}else{
								std::cout<<std::endl;
							}
						}else{
							std::cout<<GetVar(arg.string_value)<<" ";
						}
					}
				}
				
				if(comm == "set"){
					if(args.size() >= 2){
						if(cmd.is_num_arg){
							SetVar(args[0].string_value, args[1].float_value);
						}else{
							SetVar(args[0].string_value, GetVar(args[1].string_value));
						}
					}
				}
				
				if(comm == "add"){
					if(args.size() >= 2){
						if(cmd.is_num_arg){
							SetVar(args[0].string_value, GetVar(args[0].string_value) + args[1].float_value);
						}else{
							SetVar(args[0].string_value, GetVar(args[0].string_value) + GetVar(args[1].string_value));
						}
					}
				}
				
				if(comm == "sub"){
					if(args.size() >= 2){
						if(cmd.is_num_arg){
							SetVar(args[0].string_value, GetVar(args[0].string_value) - args[1].float_value);
						}else{
							SetVar(args[0].string_value, GetVar(args[0].string_value) - GetVar(args[1].string_value));
						}
					}
				}
				
				if(comm == "mul"){
					if(args.size() >= 2){
						if(cmd.is_num_arg){
							SetVar(args[0].string_value, GetVar(args[0].string_value) * args[1].float_value);
						}else{
							SetVar(args[0].string_value, GetVar(args[0].string_value) * GetVar(args[1].string_value));
						}
					}
				}
				
				if(comm == "div"){
					if(args.size() >= 2){
						if(cmd.is_num_arg){
							SetVar(args[0].string_value, GetVar(args[0].string_value) / args[1].float_value);
						}else{
							SetVar(args[0].string_value, GetVar(args[0].string_value) / GetVar(args[1].string_value));
						}
					}
				}
				
				if(comm == "cos"){ //cos A B; A = cos(B)
					if(args.size() >= 2){
						if(!cmd.is_num_arg){
							SetVar(args[0].string_value, std::cos(GetVar(args[1].string_value)));
						}else{
							SetVar(args[0].string_value, std::cos(args[1].float_value));
						}
					}
				}
				
				if(comm == "sin"){
					if(args.size() >= 2){
						if(!cmd.is_num_arg){
							SetVar(args[0].string_value, std::sin(GetVar(args[1].string_value)));
						}else{
							SetVar(args[0].string_value, std::sin(args[1].float_value));
						}
					}
				}
				
				if(comm == "tan"){
					if(args.size() >= 2){
						if(!cmd.is_num_arg){
							SetVar(args[0].string_value, std::tan(GetVar(args[1].string_value)));
						}else{
							SetVar(args[0].string_value, std::tan(args[1].float_value));
						}
					}
				}
				
				if(comm == "cmp"){ //cmp A B Result | A<B = -1; A>B = 1; A==B = 0;
					if(args.size() >= 3){
						float result = 0.0f;
						
						float v1 = GetVar(args[0].string_value);
						float v2 = 0.0f;
						
						if(cmd.is_num_arg){
							v2 = args[1].float_value;
						}else{
							v2 = GetVar(args[1].string_value);
						}
						
						if(v1 < v2){ result = -1; }
						if(v1 > v2){ result = 1; }
						if(std::abs(v1-v2) < EPSILON){ result = 0; }
						
						SetVar(args[2].string_value, result);
					}
				}
				
				if(comm == "not"){ //not A B; A = not(B)
					if(args.size() >= 2){
						SetVar(args[0].string_value, (float)((int)!GetVar(args[1].string_value)));
					}
				}
			}
		}
	}
}