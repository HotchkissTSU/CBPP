#ifndef CBPP_ERROR_H
#define CBPP_ERROR_H

/*
	There are two levels of CBPP exceptions:

	Tier 1 errors are meant to be used only if there`s really no step back, and runtime evaporation is the only viable choice.
	(The only one ((maybe)) reasonable cause for this is a complete physical destruction of the client`s computer)

	Tier 2 errors are used only as hints in the way like "something bad happened deeply inside the function call stack, so we
	will return false and push this error".

	The main rule is that no call should be able to kill the runtime - because nobody really likes game crashes.
	Better do completely wrong than do nothing.

	Hold the line, leave no quarter!
*/

#define CBPP_T2ERROR_STACK_SIZE 128

#define CbThrowError(sMsg) cbpp::Throw(sMsg, __PRETTY_FUNCTION__, __LINE__, true);
#define CbThrowErrorf(sFormat, ...) { char sErrorBuffer[1024]; snprintf(sErrorBuffer, 1024, sFormat, __VA_ARGS__); CbThrowError(sErrorBuffer); }

#define CbThrowWarning(sMsg) cbpp::Throw(sMsg, __PRETTY_FUNCTION__, __LINE__, false);
#define CbThrowWarningf(sFormat, ...) { char sErrorBuffer[1024]; snprintf(sErrorBuffer, 1024, sFormat, __VA_ARGS__); CbThrowWarning(sErrorBuffer); }

#include <stdint.h>

#include "cbpp/print.h"

namespace cbpp {
	/*
		Tier 1 error.
		Commit a warcrime and brutally murder the engine
	*/
	void Throw(const char* sMsg, const char* sFunc, size_t iLine, bool bKill) noexcept;

	enum ERROR_CODE : uint8_t {
		ERROR_INVALID,      //No errors, everything is probably cool
		ERROR_IO,           //Error within the Input/Output system
		ERROR_MEM,          //Error with the memory management (NULL-pointer, invalid size etc.)
		ERROR_TCODE,        //Error with text encoding
		ERROR_GL,           //Error in the OpenGL API
		ERROR_CDF,	        //CDF API outage
        ERROR_GF            //Something incomprehensible in our gamefile
	};

	const char* ErrorNameByCode(ERROR_CODE iCode) noexcept;

	struct ErrorInfo : public BasePrintable {
		ErrorInfo(ERROR_CODE iCode, const char* sMessage);
		~ErrorInfo();

		virtual void Print(FILE* hStream = stdout) const;
    	virtual size_t SPrint(char* sTarget, size_t iMaxWrite) const;

		char* m_sMessage = NULL;
		ERROR_CODE m_iErrorClass = ERROR_INVALID;
	};

	extern ErrorInfo* g_aErrorStack[];
	extern size_t g_iErrorStackHead;

	/*
		Push a tier 2 error in the stack.
		The passed string is copied.
	*/
	void PushError(ERROR_CODE iClass, const char* sMsg) noexcept;
	bool HasErrors() noexcept;
	ErrorInfo* GetLastError() noexcept;
	void PopLastError() noexcept;

	size_t GetTimeString(char* sBuffer, size_t iBufferSize, const char* sFormat);
}

#endif
