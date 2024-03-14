/*
Все принятые пакеты сервер складывает в очередь, откуда их читают все активные кластеры.
Пакет удаляется из очереди либо если его уже просмотрели все кластеры, либо если вышел
тайм-аут.
*/

#ifndef CBSV_SV_MAIN
#define CBSV_SV_MAIN

#include "SDL2/SDL_net.h"
#include "cbpp/vec2.h"
#include "cbpp/btable.h"

#include <string>
#include <queue>
#include <map>
#include <ctime>

#define CBSV_PKGRECV_TPS_LOCK 40
#define CBSV_PKG_MAXQUE 2048

#include <cstdint>

namespace cbsv{	
	static bool SV_PkgQue_busy = false; //флаг о том, что кто-то уже работает с этой очередью
	
	static bool server_is_run = false;
	
	static IPaddress SV_ip;
	static int SV_in_channel = -1;
	static UDPsocket SV_UDP_in_socket = nullptr;
	
	extern "C" struct Packet{
		cbpp::ByteTable btable;
		
		uint16_t proc_summ = 0; //счетчик кластеров, которые уже посмотрели этот пакет
		time_t time_received;
		
		IPaddress source;
	};
	
	static std::queue<Packet> SV_PkgQue;
	
	extern "C" void RunServer(uint16_t port);
	
	extern "C" uint16_t GetValidPort();
}

namespace cbsv{
	void PkgRecieverThread(uint16_t tps);
}

#endif