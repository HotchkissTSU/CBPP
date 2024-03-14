#ifndef CBSV_SV_USER
#define CBSV_SV_USER

#include <string>
#include <cstdint>

namespace cbsv{	
	extern "C" struct User{ //клиент
		User();
		~User();
		
		std::wstring username;
		uint16_t userid, clusterid;
		bool assigned;
	};
}

#endif