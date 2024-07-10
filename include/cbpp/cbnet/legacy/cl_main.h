#ifndef CBSV_CL_MAIN
#define CBSV_CL_MAIN

#include <string>
#include <cstdint>

#include "SDL2/SDL_net.h"
#include "cbsv/sh_packet.h"
#include "cbpp/btable.h"

namespace cbsv{
	static IPaddress CL_server_ip;
	static UDPsocket CL_UDP_socket;
	static int CL_connect_channel = -1;
	static bool CL_is_connected = false;
	
	void ConnectToHost(std::string host, uint16_t port);
	void SendToServer(Packet* pkg);
	
	void LoginToServer(std::wstring username);
}

#endif