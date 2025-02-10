#ifndef CBPP_LINKLIST_H
#define CBPP_LINKLIST_H

#include <string.h>
#include <stddef.h>

namespace cbpp {
    template <typename T> struct LinkListNode {
        LinkListNode()                  : m_pNextNode(NULL) { memset(&m_Value, 0, sizeof(m_Value)); }
        LinkListNode(const T& refValue) : m_pNextNode(NULL), m_Value(refValue) {}

        LinkListNode<T>*& Next() {
            return m_pNextNode;
        }

        LinkListNode* m_pNextNode;
        T m_Value;
    };
}

#endif