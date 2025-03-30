#include "cbpp/event.h"
#include <string.h>
#include "GLFW/glfw3.h"

namespace cbpp {
    EventNode* g_pEventQueue = NULL, *g_pEventQueueEnd = NULL;
    size_t g_iEventsCounter = 0;

    void PushEvent(Event* pEv) noexcept {
        if(g_iEventsCounter >= CBPP_EVENTQUEUE_SIZE) {
            return;
        }

        g_iEventsCounter++;

        EventNode* pNode = EventNode::Allocate();
        memcpy(&pNode->m_Event, pEv, sizeof(Event));

        if(g_pEventQueueEnd == NULL) {
            pNode->m_pNextNode = NULL;
            pNode->m_pPrevNode = NULL;
            g_pEventQueueEnd = pNode;
            g_pEventQueue = pNode;

            return;
        }

        pNode->m_pNextNode = g_pEventQueue;
        g_pEventQueue->m_pPrevNode = pNode;

        pNode->m_pPrevNode = NULL;
        g_pEventQueue = pNode;
    }

    bool PollEvent(Event* pEv) noexcept {
        if(g_pEventQueueEnd == NULL) {
            return false;
        }

        g_iEventsCounter--;

        if(pEv != NULL) { memcpy(pEv, &g_pEventQueueEnd->m_Event, sizeof(Event)); }

        EventNode* pPrev = g_pEventQueueEnd->m_pPrevNode;
        if(pPrev == NULL) {
            EventNode::Free(g_pEventQueueEnd);
            g_pEventQueue = NULL;
            g_pEventQueueEnd = NULL;

            return true;
        }

        pPrev->m_pNextNode = NULL;
        EventNode::Free(g_pEventQueueEnd);
        g_pEventQueueEnd = pPrev;

        return true;
    }

    bool HasEvents() noexcept {
        return g_iEventsCounter > 0;
    }

    void CreateEvent(Event* pTarget) noexcept {
        if(pTarget == NULL) {
            return;
        }

        pTarget->Timestamp = glfwGetTime();
    }
}
