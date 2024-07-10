#ifndef CBPP_EVENT_H
#define CBPP_EVENT_H

#include <cstdint>

namespace cbpp {
	enum EVENT_TYPE : uint16_t {
		EVENT_NONE,
		EVENT_KEY
	};
}

#endif