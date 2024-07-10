#ifndef CBPP_EP_CWD_H
#define CBPP_EP_CWD_H

namespace cbpp {
	static wchar_t* CWD = nullptr;
	
	void SetCWD(const wchar_t* cwd);
	const wchar_t* GetCWD();
}

#endif