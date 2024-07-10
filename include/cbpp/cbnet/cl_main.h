#ifndef CBNET_SVMAIN_H
#define CBNET_SVMAIN_H

#include "SDL2/SDL_Net.h"

#include "cbnet/sh_netstate.h"
#include "cbnet/sh_packet.h"

namespace cbcl{
	bool ConnectToHost(const char* host, uint16_t port);
	void SendToServer(cbsh::Packet pkg);
	
	static bool is_connected = false;
}

#endif