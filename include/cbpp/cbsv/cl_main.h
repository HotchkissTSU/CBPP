#ifndef CBSV_CL_MAIN
#define CBSV_CL_MAIN

#include <string>
#include <cstdint>

#include "SDL2/SDL_net.h"
#include "cbsv/sh_packet.h"

namespace cbsv{
	static IPaddress server_ip;
	static UDPsocket CL_UDP_socket;
	static int connect_channel;
	static bool CL_is_connected;
	
	IPaddress& ServerIP(){ return &server_ip; }
	UDPsocket& CL_Socket(){ return &CL_UDP_socket; }
	int ConnectionChannel(){ return connect_channel; }
	
	void ConnectToHost(std::string host, uint16_t port);
	void SendToServer(Packet pkg);
}

#endif