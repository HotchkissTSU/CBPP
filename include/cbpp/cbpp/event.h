#ifndef CBPP_EVENT_H
#define CBPP_EVENT_H

#include <stdint.h>
#include <uchar.h>
#include "cbpp/vec2.h"

#define CBPP_EVENTBUFFER_SIZE 256

namespace cbpp {
	struct Event{
		int a;
	};

	class IEvent {
		public:
			enum CLASS : uint8_t {
				CLASS_TEST
			};

			virtual CLASS Class() const noexcept = 0;
	};

	class InputEvent : public IEvent {
		public:
			virtual IEvent::CLASS Class() const noexcept override;
	};
}

#endif
