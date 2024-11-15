#include "cbpp/error.h"

#include <stdio.h>
#include <cstring>
#include <cstdint>

namespace cbpp {
	std::stack<ErrorInfo> _cb_errors;
	
	ErrorInfo _no_error = {
		ERROR_INVALID,
		"No errors occured!"
	};

	void DisplayError(const char* title, const char* text, bool kill){
		//printf("\n*** THIS IS A MESSAGEBOX! ***\n%s -> %s\n\n", title, text);
		//we need a real messagebox here
		if(kill){ exit(1); }
	}

	Exception::Exception(const char* _what, const char* _file, const char* _func, std::size_t _line) {
		uint64_t file_len = strlen(_file), func_len = strlen(_func), what_len = strlen(_what);
		file = new char[file_len+1];
		strcpy(file, _file);
		
		func = new char[func_len+1];
		strcpy(func, _func);
		
		text = new char[what_len];
		strcpy(text, _what);
		line = _line;
		
		memset(errtext, 0, 512);
		snprintf(errtext, 512, "[%s, %i] -> %s\n", func, line, text);
	}
	
	const char* Exception::what() const noexcept {
		return const_cast<const char*>(errtext);
	}
	
	Exception::~Exception() {
		delete[] text, func, file;
	}
	
	void _ThrowError(Exception exc) {		
		FILE* error_log = fopen("logs/error.txt", "at");
		if(error_log == NULL) {
			return;
		}
		
		fprintf(error_log, "ERROR: %s", exc.what());
		fclose(error_log);
		#ifdef CBPP_DEBUG
			#ifdef CBPP_EXC_MSGBOX
				char msgbox_text[512];
				snprintf(msgbox_text, 512, "%s, Line: %i\n%s", exc.func, exc.line, exc.text);
				DisplayError("CBPP Exception", msgbox_text, false);
			#endif
			throw exc;
		#endif
	}
	
	void _ThrowWarning(Exception exc) {
		FILE* error_log = fopen("logs/error.txt", "at");
		if(error_log == NULL) {
			return;
		}
		
		fprintf(error_log, "ERROR: %s", exc.what());
		fclose(error_log);
		#ifdef CBPP_DEBUG
			#ifdef CBPP_WARN_MSGBOX
				char msgbox_text[512];
				snprintf(msgbox_text, 512, "%s, Line: %i\n%s", exc.func, exc.line, exc.text);
				DisplayError("CBPP Warning", msgbox_text, false);
			#endif
		#endif
	}

	const ErrorInfo& GetLastError() {
		if( _cb_errors.size() > 0 ) {
			const ErrorInfo& info = _cb_errors.top();
			return info;
		}else{
			return const_cast<const ErrorInfo&>(_no_error);
		}
	}

	void PopError() {
		if(!_cb_errors.empty()){
			_cb_errors.pop();
		}
	}

	void PushError( ERROR_CODE code, const char* msg ) {
		ErrorInfo errinf = {
			code,
			strdup(msg)
		};

		_cb_errors.push(errinf);
	}

	void ClearErrors() {
		while(!_cb_errors.empty()) { //i hate std::stack with the all hate that a human being can produce
			free(_cb_errors.top().Msg);
			_cb_errors.pop();        //why no `clear` method? why!?
		}
	}

	bool HasErrors() {
		return !_cb_errors.empty();
	}
}
