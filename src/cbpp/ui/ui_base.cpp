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

        
    }
}
