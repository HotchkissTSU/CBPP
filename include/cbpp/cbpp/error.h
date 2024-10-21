#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

/*
	This is possible that CbThrowError will not kill the runtime
	(if the engine is compiled without CBPP_DEBUG), so you must
	always think that these throws won`t do anything
*/

#define CBPP_DEBUG       //if in debug mode, exceptions can be raised
#define CBPP_EXC_MSGBOX  //show messageboxes upon exceptions
#define CBPP_WARN_MSGBOX //show messageboxes upon warnings

#define CbThrowError(err_text) cbpp::_ThrowError(cbpp::Exception( err_text , __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define CbThrowWarning(warn_text) cbpp::_ThrowWarning(cbpp::Exception( warn_text , __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define CbThrowErrorf(warn_text, ...) { char err_log[256]; snprintf(err_log, 255, warn_text, __VA_ARGS__); CbThrowError(err_log); }
#define CbThrowWarningf(warn_text, ...) { char err_log[256]; snprintf(err_log, 255, warn_text, __VA_ARGS__); CbThrowWarning(err_log); }

#include <stdexcept>
#include <stdio.h>

namespace cbpp {
	extern void DisplayError(const char* title, const char* text, bool kill = false);
	
	class Exception : public std::exception {
		public:
			Exception(const char* what, const char* file = NULL, const char* func = NULL, std::size_t line = -1);
			
			virtual const char* what() const noexcept;
			
			~Exception();
			
			char *file, *func, *text;
			char errtext[512];
			
			std::size_t line;
	};
	
	extern void _ThrowError(Exception exc);
	extern void _ThrowWarning(Exception exc);
}

#endif