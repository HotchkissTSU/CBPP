#ifndef CBPP_LOGSYS
#define CBPP_LOGSYS

#define CBPP_LOGPATH std::string("logs/")

#include <string>
#include <fstream>
#include <ctime>
#include <cstdint>
#include <locale>

namespace cbpp{
	extern "C" struct Log{
		Log(){}
		Log(std::string name);
		
		void Open();
		void Close();
		bool IsOpen();
		
		void _basewrite(std::wstring str);
		
		void WriteInfo(std::wstring str);
		void WriteWarning(std::wstring str);
		void WriteError(std::wstring str);
		
		bool valid;
		std::wofstream log;
		std::string log_name;
		
		~Log();
	};
}

#endif