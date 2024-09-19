#include "cbpp/event.h"

namespace cbpp {
	std::queue<Event> event_queue;
	
	Event GetLastEvent() {
		Event target = event_queue.front();
		event_queue.pop();
		
		return target;
	}
	
	void PushEvent(Event& to_push) {
		event_queue.push(to_push);
	}
}