#ifndef CBPP_CMF_H
#define CBPP_CMF_H

#include <cstdint>

namespace cbpp {
	struct CMF_Header {
		uint16_t version;
		uint8_t name_length;
		uint16_t* name;
	};
}

#endif