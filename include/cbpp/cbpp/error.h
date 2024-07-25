#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

#define CBPP_ERRORTEXT_LENGTH 512
#define CBPP_ERRORNAME_LENGTH 64

namespace cbpp {
	static char last_error_msg[CBPP_ERRORTEXT_LENGTH];
	static char last_error_name[CBPP_ERRORNAME_LENGTH];
	
	const char* GetErrorName();
	const char* GetErrorInfo();
	
	void SetError(const char* err_name, const char* err_info);
}

#endif