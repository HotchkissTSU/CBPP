#include "cbpp/error.h"

#include <stdio.h>
#include <cstring>
#include <cstdint>

namespace cbpp {	
	void DisplayError(const char* title, const char* text, bool kill){
		printf("\n*** THIS IS A MESSAGEBOX! ***\n%s -> %s\n\n", title, text);
		//we need messagebox here
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
}
