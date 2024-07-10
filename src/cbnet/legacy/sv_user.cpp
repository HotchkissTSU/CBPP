#include "cbsv/sv_user.h"
#include "cbsv/sv_main.h"

namespace cbsv{
	User::User(){
		assigned = false;
	}
	
	void User::SendPacket(Packet* pkg){
		UDPpacket* packet = SDLNet_AllocPacket(CBSV_PKG_BUFFERSIZE);
		packet->address = cl_address;
		
		cbpp::SegArray<uint8_t> sarr(CBSV_PKG_BUFFERSIZE, 128);
		pkg->btable.Disassemble(sarr);
		
		for(int i = 0; i < CBSV_PKG_BUFFERSIZE; i++){
			packet->data[i] = sarr.At(i);
		}
		
		packet->len = sarr.Length();
		
		SDLNet_UDP_Send(SV_UDP_socket, cl_channel, packet);
		
		SDLNet_FreePacket(packet);
		sarr.Free();
	}
	
	void User::Disconnect(std::string msg){
		Packet* pkg = AssemblePacket("cbnet_userdrop");
		
		pkg->btable.WriteValue<uint16_t>("msg", (uint16_t*)(msg.c_str()), msg.length(),cbpp::UINT16);
		
		SendPacket(pkg);
		
		delete pkg;
	}
	
	User::~User(){
		
	}
}