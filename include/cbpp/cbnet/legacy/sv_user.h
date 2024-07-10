/*
Чисто серверная абстракция, вся работа с клиентами ведется через эту структуру
Предполагается, что все здешние функции вызываются только на сервере
*/


#ifndef CBSV_SV_USER
#define CBSV_SV_USER

#include <string>
#include <cstdint>

#include "SDL2/SDL_Net.h"
#include "cbsv/sh_packet.h"

namespace cbsv{
	extern "C" struct User{
		User();
		~User();
		
		void Disconnect(std::string msg);
		void SendPacket(Packet* pkg);
		
		std::wstring username;
		uint16_t userid, clusterid;
		bool assigned;
		
		IPaddress cl_address;
		int cl_channel;
	};
}

#endif