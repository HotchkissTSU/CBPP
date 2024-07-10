#include "cbsv/sh_packet.h"

namespace cbsv{
	time_t CurTimeMsec(){
		timeval time_now;
		mingw_gettimeofday(&time_now, nullptr);
		time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
		
		return msecs_time;
	}
	
	CBSV_NETSTATE NetStatus(){
		return net_status;
	}
	
	void SetNetStatus(CBSV_NETSTATE nstate){
		net_status = nstate;
	}
	
	void Packet::SetHeader(std::string head){
		btable.SetHeader(head);
	}
	
	Packet* AssemblePacket(std::string hdr){
		Packet* out = new Packet;
		
		out->btable.SetHeader(hdr);
		
		out->btable.WriteValueSingle<uint64_t>("_sent", (uint64_t)(CurTimeMsec()), cbpp::UINT64);
		
		uint8_t _dir = (NetStatus() == CBSV_CLIENT);
		
		out->btable.WriteValueSingle<uint8_t>("_dir", _dir, cbpp::UINT8);
		
		return out;
	}
}