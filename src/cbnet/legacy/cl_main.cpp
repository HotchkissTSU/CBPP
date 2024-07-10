#include "cbsv/cl_main.h"

namespace cbsv{
	void ConnectToHost(std::string host, uint16_t port){
		const char* chost = host.c_str();
		
		if(!CL_is_connected){
			CL_UDP_socket = SDLNet_UDP_Open(0);
			SDLNet_ResolveHost(&CL_server_ip, chost, port);
			CL_connect_channel = SDLNet_UDP_Bind(CL_UDP_socket, -1, &CL_server_ip);
			
			CL_is_connected = true;
			
			LoginToServer(L"fat nigger");
		}
	}
	
	void SendToServer(Packet* pkg){
		if(pkg != nullptr && CL_is_connected){
			UDPpacket* pkg_raw = SDLNet_AllocPacket(CBSV_PKG_BUFFERSIZE);
			
			cbpp::SegArray<uint8_t> sarr = pkg->btable.Assemble();
			
			pkg_raw->data = sarr.AsArray();
			pkg_raw->len = sarr.Length();
			pkg_raw->maxlen = sarr.FullLength();
			
			SDLNet_UDP_Send(CL_UDP_socket, pkg_raw->channel, pkg_raw);
			
			SDLNet_FreePacket(pkg_raw);
		}
	}
	
	void LoginToServer(std::wstring usname){
		Packet* pack = new Packet;
		pack->SetHeader("cl_userlogin");
		pack->btable.WriteWString("username", usname);
		
		SendToServer(pack);
		delete pack;
	}
}