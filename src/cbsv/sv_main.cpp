#include "cbsv/sv_main.h"

#include <windows.h>
#include <thread>
#include <string>

#include "cbsv/sh_netstate.h"
#include "cbpp/misc.h"

using std::string;

namespace cbsv{
	uint16_t GetValidPort(){
		UDPsocket sock = nullptr;
		
		uint16_t out_port = 0;
		
		while(!sock){
			out_port++;
			
			if(out_port == UINT16_MAX){
				break;
			}
			
			sock = SDLNet_UDP_Open(out_port);
		}
		
		if(sock){
			SDLNet_UDP_Close(sock);
			sock = nullptr;
		}
		
		return out_port;
	}
	
	void PkgReceiverThread(uint16_t tps){
		int tick_delay = (int)(1000.0f/tps);
		
		UDPpacket* pkg_raw = SDLNet_AllocPacket(CBSV_PKG_BUFFERSIZE);
		
		while (server_is_run){
			Sleep(tick_delay);
			
			if(server_is_run){				
				int8_t recv_num = SDLNet_UDP_Recv(SV_UDP_in_socket, pkg_raw);
				
				SV_PkgQue_busy = true;
				
				if(recv_num == 1 && SV_PkgQue.size() <= CBSV_PKG_MAXQUE){ //не набиваем очередь выше лимита
					cbpp::SegArray<uint8_t> pkg_bytes(pkg_raw->data, pkg_raw->len);
				
					Packet packet;
					packet.btable.Disassemble(pkg_bytes);
					
					time(&packet.time_received);
					
					packet.source = pkg_raw->address;
					packet.pid = (uint16_t)(packet.time_received % 511);
					
					std::cout<<packet<<std::endl;
					
					SV_PkgQue.push(packet);
				}
				
				SV_PkgQue_busy = false;				
			}
		}
		
		SDLNet_FreePacket(pkg_raw);
	}
	
	void RunServer(uint16_t port){
		SV_UDP_in_socket = SDLNet_UDP_Open(port);
		SDLNet_ResolveHost(&SV_ip, NULL, port);
		
		server_is_run = (SV_UDP_in_socket != NULL);
		
		std::cout<<SDLNet_ResolveIP(&SV_ip)<<std::endl;
		
		std::thread PkgReceiver(PkgReceiverThread, CBSV_PKGRECV_TPS_LOCK);
		PkgReceiver.detach();
		
		SetNetStatus(CBSV_SERVER);
	}
	
	std::ostream& operator << (std::ostream &o, Packet const& t){
		std::string out;		
		
		out += std::string("Packet PID: ") + cbpp::IntToString((int64_t)t.pid) + std::string("\n");
		out += std::string("Received: ") + cbpp::IntToString((int64_t)t.time_received) + std::string("\n");
		
		return o<<out<<std::endl;
	}
}