#ifndef CBPP_AI_H
#define CBPP_AI_H

#include <cstdint>

namespace cbpp {
    enum CBPP_AI_TASKID : uint16_t {
        AI_TASK_INVALID
    };

    struct AI_Task {
        int16_t priority = 0;
        CBPP_AI_TASKID id = AI_TASK_INVALID;

        bool operator<(AI_Task& other) {
            return priority < other.priority;
        }
    };

    bool AiTaskComparator( AI_Task& a, AI_Task& b ) {
        return (a.priority > b.priority);
    }
}

#endif