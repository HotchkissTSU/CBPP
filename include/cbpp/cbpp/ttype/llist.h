#ifndef CBPP_LINKLIST_H
#define CBPP_LINKLIST_H

#include <string.h>
#include <stddef.h>

namespace cbpp {
    template <typename T> struct LLNode {
        size_t m_iNextNode = 0;
        T m_Data;
    };

    //An one-way linked list
    template <typename T> class LinkList {
        public:
            LinkList() : m_aNodeList(NULL), m_iNodeListSize(0) {};
            
        private:
            size_t m_iNodes, m_iNodeListSize;
            size_t m_iEndNode;
            LLNode<T>* m_aNodeList;
    }; 
}

#endif
