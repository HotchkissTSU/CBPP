#include "cbpp/dynlib.h"

namespace cbpp {
    bool DynamicLibrary::Load(const char* sPath) {
        ON_LINUX (
            m_pLibHandle = dlopen(sPath, RTLD_NOW);

            if(m_pLibHandle == NULL) { return false; }
            return true;
        )

        ON_WINDOWS (
            static_assert(true, "DLL loading is not supported");
        )
    }

    void* DynamicLibrary::GetSymbol(const char* sName) const {
        ON_LINUX (
            void* pSymbol = dlsym(m_pLibHandle, sName);
            return pSymbol;
        )

        ON_WINDOWS (
            static_assert(true, "DLL loading is not supported");
        )
    }
}
