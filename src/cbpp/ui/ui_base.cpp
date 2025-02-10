#include "ui/ui_base.h"

namespace cbui {
    void BaseUnit::Deparent() noexcept {
        if(m_pParent == NULL) {
            return;
        }

        PTreeNode* pCurrent = m_pParent->m_pChildrenHead, *pPrev = NULL;
        while(pCurrent != NULL) {
            if(pCurrent->m_pUnit == this) {
                if(pPrev == NULL) { //we are the single child
                    delete pCurrent;
                    m_pChildrenHead = NULL;
                }else{
                    pPrev->m_pNextNode = pCurrent->m_pNextNode;
                    delete pCurrent;
                }

                m_pParent = NULL;
                return;
            }

            pPrev = pCurrent;
            pCurrent = pCurrent->m_pNextNode;
        }
    }

    void BaseUnit::SetParent(BaseUnit* pTarget) noexcept {
        if(pTarget == NULL) {
            Deparent();
            return;
        }

        if(m_pParent != NULL) { //we already have one
            Deparent();
        }

        m_pParent = pTarget;



        m_pParent->m_pChildrenHead

        return true;
    }
}