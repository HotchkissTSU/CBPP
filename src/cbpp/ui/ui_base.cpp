#include "cbpp/ui/ui_base.h"

namespace cbui {
    bool BaseUnit::SetParent(BaseUnit* pParent) noexcept {
        if(pParent == NULL) { return false; }
        if(pParent->PushChild(this)) {
            m_pParent = pParent;
            return true;
        }
        return false;
    }

    bool BaseUnit::PushChild(BaseUnit* pChild) noexcept {
        if(m_iChildren >= CBPP_UI_MAX_CHILDREN) { return false; }

        PTreeNode* pNode = PTreeNode::Allocate();

        if(pNode == NULL) { return false; }

        pNode->m_pNextNode = NULL;
        pNode->m_pUnit = pChild;

        if(m_pChildrenHead == NULL) {
            m_pChildrenHead = pNode;
        }else{
            m_pChildrenHead->m_pNextNode = pNode;
        }

        m_iChildren++;
        return true;
    }

    bool BaseUnit::RemoveChild(BaseUnit* pChild) noexcept {
        PTreeNode* pCurrent = m_pChildrenHead;
        if(pCurrent->m_pUnit == pChild) {
            PTreeNode* pNext = pCurrent->m_pNextNode;
            PTreeNode::Free(pCurrent);
            m_pChildrenHead = pNext;

            m_iChildren--;

            return true;
        }
        
        while(pCurrent != NULL) {
            PTreeNode* pNext = pCurrent->m_pNextNode;
            if(pNext != NULL && pNext->m_pUnit == pChild) {
                PTreeNode* pNextNext = pNext->m_pNextNode;
                PTreeNode::Free(pNext);
                pCurrent->m_pNextNode = pNextNext;

                m_iChildren--;

                return true;
            }

            pCurrent = pNext;
        }

        return false;
    }

    BaseUnit* BaseUnit::GetParent() noexcept {
        return m_pParent;    
    }

    PTreeNode* BaseUnit::GetChildren() noexcept {
        return m_pChildrenHead;
    }
}
