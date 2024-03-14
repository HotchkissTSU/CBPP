#include "cbsv/sv_main.h"

#include <windows.h>
#include <thread>

#include "cbsv/sh_netstate.h"

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
		}
		
		return out_port;
	}
	
	void PkgReceiverThread(uint16_t tps){
		int tick_delay = (int)(1000.0f/tps);
		while (server_is_run){
			Sleep(tick_delay);
			
			if(server_is_run){
				UDPpacket pkg_raw;
				int8_t recv_num = SDLNet_UDP_Recv(SV_UDP_in_socket, &pkg_raw);
				
				if(recv_num == 1 && SV_PkgQue.size() <= CBSV_PKG_MAXQUE){ //не набиваем очередь выше лимита
					cbpp::SegArray<uint8_t> pkg_bytes(pkg_raw.data, pkg_raw.len);
				
					Packet packet;					
					packet.btable.Disassemble(pkg_bytes);
					
					time(&packet.time_received);
					
					packet.source = pkg_raw.address;
					
					SV_PkgQue.push(packet);
				}
			}
		}
	}
	
	void RunServer(uint16_t port){
		SV_UDP_in_socket = SDLNet_UDP_Open(port);
		SDLNet_ResolveHost(&SV_ip, NULL, port);
		
		server_is_run = (SV_UDP_in_socket != NULL);
		
		std::cout<<SV_ip.host<<":"<<SV_ip.port<<std::endl;
		
		std::thread PkgReceiver(PkgReceiverThread, CBSV_PKGRECV_TPS_LOCK);
		PkgReceiver.detach();
		
		SetNetStatus(CBSV_SERVER);
	}
}