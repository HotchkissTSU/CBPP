#ifndef CBNET_SHNETSTATE_H
#define CBNET_SHNETSTATE_H

#include <cstdint>

namespace cbsh{
	enum CBNET_NETSTATE : uint8_t {
		NET_CLIENT, NET_SERVER
	};
	
	static CBNET_NETSTATE c_netstate = NET_CLIENT;
	
	extern "C" void SetNetState(CBNET_NETSTATE);
	extern "C" CBNET_NETSTATE GetNetState();
}

#endif