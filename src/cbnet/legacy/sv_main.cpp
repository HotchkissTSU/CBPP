#include "cbsv/sv_main.h"

#include <windows.h>
#include <thread>
#include <string>


#include "cbpp/misc.h"

#include "cbsv/sv_queue.h"
#include "cbsv/sv_user.h"
#include "cbsv/sv_cluster.h"
#include "cbsv/sh_netstate.h"
#include "cbsv/sv_queue.h"

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
	
	bool AppendPacketToQue(Packet* pkg){
		bool found = false;
		for(uint32_t i = 0; i < SV_PkgQue.Length(); i++){
			if(SV_PkgQue[i] == nullptr){
				SV_PkgQue[i] = pkg;
				found = true;
				break;
			}
		}
		
		return found;
	}
	
	void PkgReceiverThread(uint16_t tps){
		int tick_delay = (int)(1000.0f/tps);
		
		std::cout<<"server tick start\n";
		
		UDPpacket* pkg_raw = SDLNet_AllocPacket(CBSV_PKG_BUFFERSIZE);
		
		while (server_is_run){
			Sleep(tick_delay);
			
			if(server_is_run && !SV_PkgQue_busy){			
				int8_t recv_num = SDLNet_UDP_Recv(SV_UDP_socket, pkg_raw);
				
				SV_PkgQue_busy = true;
				
				if(recv_num == 1){
					cbpp::SegArray<uint8_t> pkg_bytes(pkg_raw->data, pkg_raw->len);
					std::cout<<"GOT PACKET\n";
					Packet* packet = new Packet;
					packet->btable.Disassemble(pkg_bytes);
					
					time(&packet->time_received);
					
					packet->source = pkg_raw->address;
					packet->pid = (uint16_t)( (packet->time_received % 511) + (packet->time_received << 3) );
					
					if(packet->btable.GetHeader() == std::string("cl_userlogin")){ //типок хочет залогиниться
						std::wstring username_(L"USERNAME");
						if(packet->btable.HasKey("username")){
							username_ = std::wstring(packet->btable.ReadWString("username"));
						}
						
						User* new_us = new User();
						
						UserID new_us_pos = AddUser(new_us);
						
						new_us->username = username_;
						new_us->userid = new_us_pos.user_id;
						new_us->clusterid = new_us_pos.cluster_id;
						new_us->assigned = new_us_pos.allocated;
						
						std::cout<<"USER "<<pkg_raw->address.host<<" ALLOCATED\n";
						
					}else{
						AppendPacketToQue(packet);
					}
					
					memset(pkg_raw->data, 0, pkg_raw->maxlen);
				}
				
				SV_PkgQue_busy = false;
			}
		}
		
		SDLNet_FreePacket(pkg_raw);
	}
	
	void RunServer(uint16_t port){
		SV_UDP_socket = SDLNet_UDP_Open(port);
		AllocatePacketQueue();
		//AllocateClusters(2, 16);
		
		server_is_run = (SV_UDP_socket != NULL);
		
		std::thread PkgReceiver(PkgReceiverThread, CBSV_PKGRECV_TPS_LOCK);
		PkgReceiver.detach();
		
		SetNetStatus(CBSV_SERVER);
	}
}