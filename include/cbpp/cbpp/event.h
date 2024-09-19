#ifndef CBPP_EVENT_H
#define CBPP_EVENT_H

#include <queue>
#include <cstdint>

namespace cbpp {
	enum CBPP_EVENT_TYPE : uint16_t {
		CBPP_INVALID_EVENT,
		CBPP_KEYBOARD_EVENT,
		CBPP_MOUSE_EVENT
	};
	
	struct ButtonEvent {
		int key, scancode, action, mods;
	};
	
	struct MouseEvent {
		int mx, my, button;
	};
	
	struct Event {
		CBPP_EVENT_TYPE type = CBPP_INVALID_EVENT;
		ButtonEvent button;
		MouseEvent mouse;
	};
	
	Event GetLastEvent();
	void PushEvent(Event& to_push);
	
	extern std::queue<Event> event_queue;
}

#endif