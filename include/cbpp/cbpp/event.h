#ifndef CBPP_EVENT_H
#define CBPP_EVENT_H

#include <queue>
#include <cstdint>
#include <uchar.h>
#include "cbpp/vec2.h"

#define CBPP_EVENTBUFFER_SIZE 256

namespace cbpp {
	enum CBPP_EVENT_TYPE : uint8_t {
		CBPP_INVALID_EVENT,
		CBPP_KEYBOARD_EVENT,
		CBPP_MOUSE_EVENT,
		CBPP_TEXTINPUT_EVENT
	};
	
	struct Event {
		CBPP_EVENT_TYPE Type = CBPP_INVALID_EVENT;

		struct {
			int32_t Key, Scancode, Action, Mods;
		} ButtonEvent;

		struct {
			int32_t Button;
			Vec2 Position;
		} MouseEvent;
		
		struct {
			char32_t Charcode;
		} TextInputEvent;
	};
	
	bool GetLastEvent(Event& ev);
	void PushEvent(Event& to_push);
	
	extern std::queue<Event> event_queue;
}

#endif