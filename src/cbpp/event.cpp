#include "cbpp/event.h"

namespace cbpp {
	std::queue<Event> event_queue;
	
	bool GetLastEvent(Event& target) {
		if(event_queue.size() <= 0) {
			return false;
		}

		target = event_queue.front();
		event_queue.pop();
		
		return true;
	}
	
	void PushEvent(Event& to_push) {
		event_queue.push(to_push);
	}
}
