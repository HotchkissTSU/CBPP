#include "cb_main/cwd.h"
#include <cstring>
#include <cstdint>

namespace cbpp {
	void SetCWD(const wchar_t* new_cwd){
		int64_t strln = wcslen(new_cwd);
		
		if(CWD != nullptr){ delete[] CWD; }
		
		CWD = new wchar_t[strln+1];
		wcscpy(CWD, new_cwd);
	}
	
	const wchar_t* GetCWD(){
		return const_cast<wchar_t*>(CWD);
	}
}