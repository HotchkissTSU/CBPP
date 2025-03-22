#ifndef CBPP_UI_BASE_H
#define CBPP_UI_BASE_H

#include "cbpp/vec2.h"
#include "cbpp/asset/resource.h"
#include "cbpp/event.h"

#define CBPP_UI_MAX_CHILDREN 128

namespace cbui {
    class BaseUnit;

    pooled_struct(PTreeNode) {
        PTreeNode* m_pNextNode = NULL;
        BaseUnit* m_pUnit = NULL;
    };

    class BaseUnit {
        public:
            BaseUnit(BaseUnit* pParent);

            virtual cbpp::Vec2 GetBounds() = 0;

            void Deparent() noexcept;
            bool SetParent(BaseUnit* pTarget) noexcept;
            BaseUnit* GetParent() noexcept;
            PTreeNode* GetChildren() noexcept;

            bool PushChild(BaseUnit* pChild) noexcept;
            bool RemoveChild(BaseUnit* pChild) noexcept;

            virtual bool OnEvent(Event* pEvent) noexcept = 0;

            virtual void Render() = 0;
            virtual void Think()  = 0;

        private:
            cbpp::Vec2 m_vBounds, m_vLocalPosition;

            BaseUnit* m_pParent = NULL;
            PTreeNode* m_pChildrenHead = NULL;
            size_t m_iChildren = 0;
    };
};

#endif
