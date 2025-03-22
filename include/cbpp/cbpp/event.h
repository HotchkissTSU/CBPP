#ifndef CBPP_EVENT_H
#define CBPP_EVENT_H

#include <stdint.h>
#include <uchar.h>
#include <time.h>

#include "cbpp/asset/resource.h"

//The maximal amount of events to store in the queue
#define CBPP_EVENTQUEUE_SIZE 256

namespace cbpp {
	struct Event {
		enum EType : uint8_t {
			KEYBOARD,
			MOUSE_BUTTON,
			MOUSE_MOVE
		};

		enum EState : uint8_t {
			KEY_PRESS,
			KEY_RELEASE
		};

		EType Type;
		time_t Timestamp;

		union {
			struct {
				EState State;
				uint32_t Key;
			} Keyboard;

			struct {
				EState State;
				uint8_t Button;
				uint32_t X, Y;
			} MouseButton;
		} Data;
	};

	pooled_struct (EventNode) {
		EventNode *m_pNextNode, *m_pPrevNode;
		Event m_Event;
	};

	void PushEvent(Event* pEv) noexcept;
	bool PollEvent(Event* pEv) noexcept;
	bool HasEvents() noexcept;
}

#endif
