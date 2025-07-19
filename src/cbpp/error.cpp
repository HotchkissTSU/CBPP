#include "cbpp/error.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

namespace cbpp {
	//give user a funny string if their day is all bad :)
	const char* g_aTimeErrorStrings[] = {
		"At the edge of void",
		"Nowhere, at no time",
		"At the very end",
		"In the Cuber-epoch",
		"Sometimes, somewhere"
	};
	constexpr static uint8_t g_iTES_Length = sizeof(g_aTimeErrorStrings) / sizeof(g_aTimeErrorStrings[0]);

	size_t GetTimeString(char* sBuffer, size_t iBufferSize, const char* sFormat) {
		time_t iTimeRaw = time(NULL);
		if( iTimeRaw == (time_t)(-1) ) {
			snprintf(sBuffer, iBufferSize, "%s", g_aTimeErrorStrings[rand() % g_iTES_Length]);
			return -1;
		}

		struct tm* iCurTime = localtime(&iTimeRaw);
		if(iCurTime == NULL) {
			snprintf(sBuffer, iBufferSize, "%s", g_aTimeErrorStrings[rand() % g_iTES_Length]);
			return -1;
		}

		return strftime(sBuffer, iBufferSize, sFormat, iCurTime);
	}

	void Throw(const char* sMsg, const char* sFunc, size_t iLine, bool bKill) noexcept {
		FILE* hLog = fopen("logs/error.txt", "at");

		if(hLog != NULL && bKill) { //Unwind the tier 2 error stack
			ErrorInfo* pError;
			while(HasErrors()) {
				pError = GetLastError();

				Print(*pError, hLog);
				fprintf(hLog, "\n");

				Print(*pError);
				printf("\n");

				PopLastError();
			}
		}

		if(hLog != NULL) {
			char sTime[16];
			GetTimeString(sTime, 16, "%T");

			const char* sText;
			if(bKill) {
				sText = "ERROR:";
			}else{
				sText = "WARN :";
			}

			fprintf(hLog, "[%s](%s, %d) %s %s\n", sTime, sFunc, iLine, sText, sMsg);
			printf("[%s](%s, %d) %s %s\n", sTime, sFunc, iLine, sText, sMsg);

			fclose(hLog);
		}

		if(bKill) {
			exit(-1);
		}
    }

	const char* ErrorNameByCode(ERROR_CODE iCode) noexcept {
		switch (iCode) {
			case ERROR_GL: return "OpenGL API error";
			case ERROR_IO: return "Input/output error";
			case ERROR_MEM: return "Memory management failure";
			case ERROR_TCODE: return "Text encoding error";
            case ERROR_CDF: return "CDF API error";
            case ERROR_GF: return "Bad gamefile";
			default: return "(Unknown error type)";
		}
	}

	ErrorInfo::ErrorInfo(ERROR_CODE iCode, const char* sMessage) : m_iErrorClass(iCode) {
		m_sMessage = strdup(sMessage);
	}

	void ErrorInfo::Print(FILE* hTarget) const {
		fprintf(hTarget, "TIER 2: %s:\n%s\n", ErrorNameByCode(m_iErrorClass), m_sMessage);
	}

	size_t ErrorInfo::SPrint(char* sTarget, size_t iMaxWrite) const {
		return snprintf(sTarget, iMaxWrite, "TIER 2: %s:\n%s\n", ErrorNameByCode(m_iErrorClass), m_sMessage);
	}

	ErrorInfo::~ErrorInfo() {
		if(m_sMessage != NULL) {
			free(m_sMessage);
		}
	}

	ErrorInfo* g_aErrorStack[CBPP_T2ERROR_STACK_SIZE];
	size_t g_iErrorStackHead = 0;

	void PushError(ERROR_CODE iCode, const char* sMessage) noexcept {
		if(g_iErrorStackHead >= CBPP_T2ERROR_STACK_SIZE) { return; }

		ErrorInfo* pErrorData = new ErrorInfo(iCode, sMessage);
		
		g_iErrorStackHead++;
		g_aErrorStack[g_iErrorStackHead] = pErrorData;
	}

	ErrorInfo* GetLastError() noexcept {
		if(g_iErrorStackHead == 0) { return NULL; }

		return g_aErrorStack[g_iErrorStackHead];
	}

	void PopLastError() noexcept {
		if(g_iErrorStackHead == 0) { return; }

		delete g_aErrorStack[g_iErrorStackHead];
		g_iErrorStackHead--;
	}

	bool HasErrors() noexcept {
		return g_iErrorStackHead != 0;
	}
}
