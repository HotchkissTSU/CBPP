#ifndef CBSV_PACKET_H
#define CBSV_PACKET_H

#include "SDL2/SDL_net.h"
#include "cbpp/btable.h"
#include "cbsv/sh_netstate.h"

#include <ctime>
#include <string>

#define CBSV_PKG_BUFFERSIZE 1024

namespace cbsv{
	extern "C" enum CBSV_PKGTYPE : uint16_t {
		SH_USERDROP,
		CL_CONNECT
	};
	
	extern "C" struct Packet{
		void SetHeader(std::string head);
		
		cbpp::ByteTable btable;
		
		time_t time_received;
		
		IPaddress source;
		
		uint16_t pid = 0;
	};
	
	Packet* AssemblePacket(std::string hdr);
	time_t CurTimeMsec();
}

/*

CB++ пакет должен иметь следующий заголовок:

uint64  _sent - время отправки пакета (миллисекунды)
uint8   _dir  - 1=от клиента серверу, 0=от сервера клиенту

*/

#endif