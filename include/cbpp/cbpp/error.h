#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

#define CBPP_DEBUG       //if in debug mode, STL exceptions can be thrown
#define CBPP_EXC_MSGBOX  //show messageboxes upon exceptions
#define CBPP_WARN_MSGBOX //show messageboxes upon warnings

#define CBPP_ERROR_LOG_SIZE 256

/*
	These macro`s exist for decapitating your runtime if you encounter something so terrible
	and impossible to comprehend, that you think it is a good reason to crash the game

	(I insist: the only one maybe "good" reason for the game crash is a complete physical destruction of your computer)

	Also, they will create a log output
*/

#define CbThrowError(err_text) cbpp::_ThrowError(cbpp::Exception( err_text , __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define CbThrowWarning(warn_text) cbpp::_ThrowWarning(cbpp::Exception( warn_text , __FILE__, __PRETTY_FUNCTION__, __LINE__))
#define CbThrowErrorf(warn_text, ...) { char err_log[CBPP_ERROR_LOG_SIZE]; snprintf(err_log, CBPP_ERROR_LOG_SIZE, warn_text, __VA_ARGS__); CbThrowError(err_log); }
#define CbThrowWarningf(warn_text, ...) { char err_log[CBPP_ERROR_LOG_SIZE]; snprintf(err_log, CBPP_ERROR_LOG_SIZE, warn_text, __VA_ARGS__); CbThrowWarning(err_log); }

#include <stdexcept>
#include <cstdint>
#include <stdio.h>
#include <stack>

namespace cbpp {
	enum ERROR_CODE : uint8_t {
		ERROR_INVALID, //no errors, everything is probably cool
		ERROR_IO       //error within the Input/Output system
	};

	struct ErrorInfo {
		ERROR_CODE Code = ERROR_INVALID;
		const char* Msg;
	};

	extern void DisplayError(const char* title, const char* text, bool kill = false);

	extern std::stack<ErrorInfo> _cb_errors;
	extern ErrorInfo _no_error;
	
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

	/*
		These three following pals are meant to be used upon non-lethal errors,
		if we also need some sort of callback from the class of function
	*/
	void PushError(ERROR_CODE errcd, const char* msg);
	const ErrorInfo& GetLastError();
	void ClearErrors();
}

#endif