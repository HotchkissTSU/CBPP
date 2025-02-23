#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

#define CBPP_DEBUG       //if in debug mode, STL exceptions can be thrown, and some info is printed in the terminal
#define CBPP_EXC_MSGBOX  //show messageboxes upon exceptions
#define CBPP_WARN_MSGBOX //show messageboxes upon warnings

#define CBPP_ERROR_LOG_SIZE 256

/*
	TIER 1 ERRORS

	These macros exist for decapitating your runtime if you encounter something so terrible
	and impossible to comprehend, that you think it is a good reason to evaporate the runtime.

	(I insist: the only one (barely forgivable) reason for the game crash is a complete physical destruction of your computer)

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

#include "cbpp/print.h"

namespace cbpp {
	enum ERROR_CODE : uint8_t {
		ERROR_INVALID, //No errors, everything is probably cool
		ERROR_IO,      //Error within the Input/Output system
		ERROR_MEM,     //Error with the memory management (NULL-pointer, invalid size etc.)
		ERROR_TCODE,   //Error with text encoding
		ERROR_GL       //Error in the OpenGL API
	};

	const char* ErrorNameByCode(ERROR_CODE iCode) noexcept;

	struct ErrorInfo : public BasePrintable {
		virtual void Print(FILE* hTarget = stdout) const noexcept;
		virtual size_t SPrint(char* sTarget, size_t iMaxWrite) const noexcept;

		ERROR_CODE Code = ERROR_INVALID;
		char* Msg;

		~ErrorInfo();
	};

	extern const char* time_err_table[];
	size_t GetTimeString(char* buffer, size_t buffer_ln, const char* format);

	void DisplayError(const char* title, const char* text, bool kill = false);

	extern std::stack<ErrorInfo> _cb_errors;
	
	class Exception : public std::exception {
		public:
			Exception(const char* what, const char* file = NULL, const char* func = NULL, std::size_t line = -1);
			
			virtual const char* what() const noexcept;
			
			~Exception();
			
			char *file, *func, *text;
			char errtext[512];
			
			std::size_t line;
	};
	
	void _ThrowError(Exception exc);
	void _ThrowWarning(Exception exc);

	/*
		TIER 2 ERRORS

		These following pals are meant to be used upon non-lethal errors,
		if we also need some sort of callback from a class or a function
	*/
	void PushError(ERROR_CODE errcd, const char* msg);
	ErrorInfo* GetLastError();
	bool HasErrors();
	void PopError();
	void ClearErrors();
}

#endif