#ifndef CBPP_TASKPOOL_H
#define CBPP_TASKPOOL_H

#include <cstdint>

namespace cbpp {
	enum TASKTYPE : uint8_t {
		TASK_NONE,
		TASK_LOAD_RESOURCE
	};
}

#endif