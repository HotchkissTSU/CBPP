#ifndef CBNET_SHPACKET_H
#define CBNET_SHPACKET_H

namespace cbsh{
	extern "C" enum CBNET_PKGTYPE : uint8_t {
		SH_VOID,
		
		SH_CONNECT, SV_DROPCON, CL_DROPCON,
		
		SV_MKENT, SV_ENTSYNC, SV_RMENT
	};
	
	extern "C" struct Packet{
		
	};
}

#endif