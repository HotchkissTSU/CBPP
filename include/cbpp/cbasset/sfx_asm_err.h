#ifndef CBASSET_SFX_ASM_ERRORS
#define CBASSET_SFX_ASM_ERRORS

#include <string>
#include <vector>

namespace cbasset{
	namespace SFX{
		extern "C" struct BaseError{
			BaseError(){}
			BaseError(std::string e_title, std::string e_about, std::string e_where){
				title = e_title;
				about = e_about;
				where = e_where;
			}
			
			std::string title = "base_title";
			std::string about = "base_desc";
			std::string where = "func at line 0";
			
			virtual std::string ErrorText(){
				return std::string("ERROR: ") + title + " , " + where + '\n' + about;
			}
		};
		
		extern "C" struct LessArgsError : public BaseError{
			int expect = 0, got = 0;
			
			LessArgsError(){}
			LessArgsError(std::string e_title, std::string e_about, std::string e_where, int e_expect, int e_got) : BaseError(e_title, e_about, e_where){
				expect = e_expect;
				got = e_got;
				
				char buff_expect[11];
				itoa(e_expect, buff_expect, 10);
				
				char buff_got[11];
				itoa(e_got, buff_got, 10);
				
				about = std::string("Too least arguments in function: expected ") + std::string(buff_expect) + std::string(", but got ") + std::string(buff_got) + '\n';
			}
		};
	}
}

#endif