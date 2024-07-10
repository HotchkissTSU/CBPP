#include "cb_main/error.h"
#include <windows.h>

namespace cbpp {
	void DisplayError(const char* title, const char* text, bool kill){
		MessageBox(NULL, text, title, MB_OK);
		if(kill){ exit(1); }
	}
}