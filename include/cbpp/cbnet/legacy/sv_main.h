/*
Все принятые пакеты сервер складывает в очередь, откуда их читают все активные кластеры.
*/

#ifndef CBSV_SV_MAIN
#define CBSV_SV_MAIN

#include "SDL2/SDL_net.h"
#include "cbpp/vec2.h"
#include "cbsv/sh_packet.h"

#include <string>
#include <queue>
#include <map>
#include <cstdint>

#define CBSV_PKGRECV_TPS_LOCK 40

namespace cbsv{		
	static bool server_is_run = false;
	
	static UDPsocket SV_UDP_socket = nullptr;
	
	extern "C" void RunServer(uint16_t port);
	
	extern "C" uint16_t GetValidPort();
	
	std::ostream& operator << (std::ostream &o, Packet const& t);
}

namespace cbsv{
	void PkgRecieverThread(uint16_t tps);
}

#endif