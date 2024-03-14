#ifndef CBASSET_SFX_ASM
#define CBASSET_SFX_ASM

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <cmath>

#include "cbpp/misc.h"
#include "cbasset/sfx_asm_err.h"

#define EPSILON 0.0001f

namespace cbasset{
	namespace SFX{
		extern "C" struct ArgValue{
			ArgValue(){}
			ArgValue(float val);
			ArgValue(std::string val);
			
			float float_value = 0.0f;
			std::string string_value;
			bool state = true; //true = float, false = string
		};
		
		extern "C" struct CommandBlock{
			CommandBlock(){}
			
			bool is_num_arg = true;
			std::string command;
			std::vector<ArgValue> args;
			
			std::string block_name = "MAIN";
		};
		
		extern "C" struct ProgramBlock{
			ProgramBlock(){}
			
			void Execute(std::vector<float> block_args);
			void SetVar(std::string name, float value);
			float GetVar(std::string name);
			
			void _call(CommandBlock cmd, bool _if);
			
			std::vector<CommandBlock> commands;
			std::map<std::string, ProgramBlock> sub_progs;
			std::map<std::string, float> block_vars;
			
			std::string block_name = "MAIN";
			
			bool is_byteblock = false;
			
			std::vector<std::string> input_names;
			
			ProgramBlock* parent_prog = nullptr;
			
			std::vector<BaseError> prog_errors;
		};
	}
	
	namespace SFX{
		extern "C" ProgramBlock* ParseSFX(std::string sfx_name);
	}
}

#endif