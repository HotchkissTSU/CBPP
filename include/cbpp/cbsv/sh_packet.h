#ifndef CBSV_PACKET_H
#define CBSV_PACKET_H

#include "SDL2/SDL_net.h"
#include "cbpp/btable.h"

#include <ctime>

namespace cbsv{
	extern "C" struct Packet{
		cbpp::ByteTable btable;
		
		uint16_t proc_summ = 0; //счетчик кластеров, которые уже посмотрели этот пакет
		time_t time_received;
		
		IPaddress source;
		
		uint16_t pid = 0;
	};
}

#endif