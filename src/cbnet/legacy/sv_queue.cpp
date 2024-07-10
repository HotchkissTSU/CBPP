#include "cbsv/sv_queue.h"

namespace cbsv{
	void AllocatePacketQueue(uint32_t maxsize){
		if(que_alloc){
			FreePacketQueue();
		}
		
		SV_PkgQue.Allocate(maxsize, 128);
		
		que_alloc = true;
	}
	
	void AllocatePacketQueue(){
		AllocatePacketQueue(CBSV_PKG_MAXQUE);
	}
	
	void FreePacketQueue(){
		SV_PkgQue.Free();
		
		que_alloc = false;
	}
}