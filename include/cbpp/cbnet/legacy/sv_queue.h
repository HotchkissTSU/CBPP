#ifndef CBSV_SV_QUEUE
#define CBSV_SV_QUEUE

#include <queue>
#include <cstdint>

#include "cbsv/sh_packet.h"
#include "cbpp/btable.h"

#define CBSV_PKG_MAXQUE 2048

namespace cbsv{
	static bool SV_PkgQue_busy = false; //флаг о том, что кто-то уже работает с этой очередью
	static cbpp::SegArray<Packet*> SV_PkgQue; //очередь пакетов
	static bool que_alloc = false;
	
	void AllocatePacketQueue(uint32_t maxsize);
	void AllocatePacketQueue();
	
	void FreePacketQueue();
	
	cbpp::SegArray<Packet>* GetQueue();
}

#endif