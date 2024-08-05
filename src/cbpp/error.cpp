#include "cbpp/error.h"

#include <stdio.h>
#include <cstring>
#include <cstdint>

namespace cbpp {	
	Exception::Exception(const char* _what, const char* _file, const char* _func, std::size_t _line) {
		uint64_t file_len = strlen(_file), func_len = strlen(_func), what_len = strlen(_what);
		file = new char[file_len+1];
		strcpy(file, _file);
		
		func = new char[func_len+1];
		strcpy(func, _func);
		
		text = new char[what_len];
		strcpy(text, _what);
		line = _line;
		
		errtext = new char[512];
		memset(errtext, 0, 512);
		snprintf(errtext, 512, "[File: %s][Function: %s][Line: %i] -> %s\n", file, func, line, text);
	}
	
	const char* Exception::what() const noexcept {
		return const_cast<const char*>(errtext);
	}
	
	Exception::~Exception() {
		delete[] errtext, text, func, file;
	}
	
	void _ThrowError(Exception exc) {		
		FILE* error_log = fopen("logs/error.txt", "at");
		if(error_log == NULL) {
			return;
		}
		
		fprintf(error_log, "%s", exc.what());
		fclose(error_log);
		
		#ifdef CBPP_DEBUG
		throw exc;
		#endif
	}
}