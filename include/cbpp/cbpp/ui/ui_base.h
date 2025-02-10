#ifndef CBPP_UI_BASE_H
#define CBPP_UI_BASE_H

#include "cbpp/vec2.h"

#define CBPP_UI_PTREE_DEPTH 128

namespace cbui {
    struct Rect {
        Vec2 vTopLeft, vBottomRight;
    };

    struct PTreeNode {
        PTreeNode* m_pNextNode;
        BaseUnit* m_pUnit;
    };

    class BaseUnit {
        public:
            virtual Vec2 GetBounds() = 0;

            void Deparent() noexcept;
            bool SetParent(BaseUnit* pTarget) noexcept;
            BaseUnit* GetParent() noexcept;
            PTreeNode* GetChildren() noexcept;

            virtual void Render() = 0;
            virtual void Think()  = 0;

            virtual void OnEvent() = 0;

        private:
            Vec2 m_vBounds, m_vLocalPosition;

            BaseUnit* m_pParent = NULL;
            PTreeNode* m_pChildrenHead = NULL:
    };
};

#endif
