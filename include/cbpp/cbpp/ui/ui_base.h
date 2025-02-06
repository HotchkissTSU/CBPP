#ifndef CBPP_UI_BASE_H
#define CBPP_UI_BASE_H

#include "cbpp/vec2.h"

namespace cbui {
    struct Rect {
        Vec2 vTopLeft, vBottomRight;
    };

    class BaseUnit {
        public:
            virtual Vec2 GetBounds() = 0;

            virtual BaseUnit* GetParent() = 0;

            virtual void Render() = 0;
            virtual void Think()  = 0;

            virtual void OnEvent() = 0;

        private:
            Vec2 m_vBounds, m_vLocalPosition;

            BaseUnit* m_pParent = NULL;
    };
};

#endif
