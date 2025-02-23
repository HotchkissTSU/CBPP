// This shit is impossibly monke-bugged, REWRITE ME AS SOON AS POSSIBLE

#include "cbpp/error.h"

#include <stdio.h>
#include <cstring>
#include <cstdint>
#include <time.h>
#include <stdlib.h>

namespace cbpp {
	std::stack<ErrorInfo> _cb_errors;

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

	//give user a funny string if his day is all bad :)
	const char* time_err_table[] = {
		"At the edge of void",
		"Nowhere, at no time",
		"At the very end",
		"In the Cuber-epoch",
		"Sometime, somewhere"
	};
	constexpr static char time_err_table_ln = sizeof(time_err_table) / sizeof(time_err_table[0]);

	size_t GetTimeString(char* buffer, size_t buffer_ln, const char* format) {
		time_t cur_time_raw = time(NULL);
		if( cur_time_raw == (time_t)(-1) ) {
			snprintf(buffer, buffer_ln, "%s", time_err_table[rand() % time_err_table_ln]);
			return -1;
		}

		struct tm* cur_time = localtime(&cur_time_raw);
		if(cur_time == NULL) {
			snprintf(buffer, buffer_ln, "%s", time_err_table[rand() % time_err_table_ln]);
			return -1;
		}

		return strftime(buffer, buffer_ln, format, cur_time);
	}

	void _ThrowError(Exception exc) {		
		FILE* error_log = fopen("logs/error.txt", "at");
		if(error_log == NULL) {
			return;
		}

		/*ErrorInfo* pError;
		while(HasErrors()) {
			fprintf(error_log, "\nTIER 2: %s:\n%s\n\n", ErrorNameByCode(pError->Code), pError->Msg);
			PopError();
		}*/

		char ftime_buff[64];
		GetTimeString(ftime_buff, 64, "%T");

		fprintf(error_log, "[%s] ERROR: %s", ftime_buff, exc.what());

		//fprintf(error_log, "ERROR: %s", exc.what());
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

		char ftime_buff[64];
		GetTimeString(ftime_buff, 64, "%T");

		fprintf(error_log, "[1][%s] WARN  : %s", ftime_buff, exc.what());
		fclose(error_log);
		#ifdef CBPP_DEBUG
			#ifdef CBPP_WARN_MSGBOX
				char msgbox_text[512];
				snprintf(msgbox_text, 512, "%s, Line: %i\n%s", exc.func, exc.line, exc.text);
				DisplayError("CBPP Warning", msgbox_text, false);
			#endif
		#endif
	}

	const char* ErrorNameByCode(ERROR_CODE iCode) noexcept {
		switch (iCode) {
			case ERROR_GL: return "OpenGL API error";
			case ERROR_IO: return "Input/output error";
			case ERROR_MEM: return "Memory management failure";
			case ERROR_TCODE: return "Text incoding error";
			default: return "Unknown error type";
		}
	}

	void ErrorInfo::Print(FILE* hTarget) const noexcept {
		fprintf(hTarget, "\nTIER 2: %s:\n%s\n\n", ErrorNameByCode(Code), Msg);
	}

	size_t ErrorInfo::SPrint(char* sTarget, size_t iMaxWrite) const noexcept {
		return snprintf(sTarget, iMaxWrite, "\nTIER 2: %s:\n%s\n\n", ErrorNameByCode(Code), Msg);
	}

	ErrorInfo::~ErrorInfo() {
	}
	
	ErrorInfo* GetLastError() {
		if( _cb_errors.size() != 0 ) {
			ErrorInfo& info = _cb_errors.top();
			return &info;
		}else{
			return NULL;
		}
	}

	void PopError() {
		if(!_cb_errors.empty()){
			_cb_errors.pop();
		}
	}

	void PushError( ERROR_CODE code, const char* msg ) {
		//printf("TIER 2 Error: %d, %s\n", code, msg);
		ErrorInfo errinf;
		errinf.Code = code;
		errinf.Msg = strdup(msg);

		Print(errinf);

		_cb_errors.push(errinf);
	}

	void ClearErrors() {
		while(!_cb_errors.empty()) { //i hate std::stack with all the hate a human being can produce
			free(_cb_errors.top().Msg);
			_cb_errors.pop();        //why no `clear` method? why!?
		}
	}

	bool HasErrors() {
		return !_cb_errors.empty();
	}
}
