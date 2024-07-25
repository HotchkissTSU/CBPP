#include "cbpp/error.h"

#include <cstring>

using std::size_t;

namespace cbpp {
	const char* GetErrorName() {
		return const_cast<const char*>(last_error_name);
	}
	
	const char* GetErrorInfo() {
		return const_cast<const char*>(last_error_msg);
	}
	
	void SetError(const char* name, const char* info) {		
		memset(last_error_msg, 0, CBPP_ERRORTEXT_LENGTH);
		memset(last_error_name, 0, CBPP_ERRORNAME_LENGTH);
		
		if(name == nullptr || info == nullptr) {
			return;
		}
		
		size_t name_ln = strlen(name);
		size_t text_ln = strlen(info);
		
		if(name_ln > CBPP_ERRORNAME_LENGTH) {
			name_ln = CBPP_ERRORNAME_LENGTH - 1;
		}
		
		if(text_ln > CBPP_ERRORTEXT_LENGTH) {
			text_ln = CBPP_ERRORTEXT_LENGTH - 1;
		}
		
		memcpy(last_error_name, name, name_ln);
		memcpy(last_error_msg, info, text_ln);
	}
}