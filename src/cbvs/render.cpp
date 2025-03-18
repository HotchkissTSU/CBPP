#include "cbvs/render.h"
#include "cbpp/error.h"

namespace cbvs {
    cbpp::float_t g_fScreenRatio = 1.0f;

    IRenderable::IRenderable(const char* sPipeName) {
        m_pPipe = cbvs::GetPipe(sPipeName);

        if(m_pPipe == NULL) {
            CbThrowErrorf("Unable to get '%s' rendering pipeline", sPipeName);
        }
    }

    void IRenderable::CallPipe() const noexcept {
        if(m_pPipe != NULL) { m_pPipe->Use(); }
    }
}
