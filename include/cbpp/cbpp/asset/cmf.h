#ifndef CBPP_CMF_H
#define CBPP_CMF_H

#include <cstdint>

#define CMF_ARRAY(_type, _name, _lentype) { _lentype _name##_length;\
_type _name*; }

namespace cbpp {
	struct CMF_Header {
		uint16_t version;
		CMF_ARRAY(uint8_t, mapname, uint8_t)
	};
}

#endif