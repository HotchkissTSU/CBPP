#ifndef CBVS_RENDER_H
#define CBVS_RENDER_H

#include "cbvs/shader.h"

namespace cbvs {
    class IRenderable {
        public:
            IRenderable(const char* sPipeName);

            void CallPipe() const noexcept;

            virtual void Render() const noexcept = 0;

        protected:
            const Pipe* m_pPipe;
    };

    extern cbpp::float_t g_fScreenRatio;
}

#endif
