#ifndef CBPP_DYNLIB_H
#define CBPP_DYNLIB_H

#include "cbdef.h"

#if CBPP_LINUX
    #include <dlfcn.h>
#endif

#if CBPP_WINDOWS
    #include <Windows.h>
#endif

namespace cbpp {
    class DynamicLibrary {
        public:
            DynamicLibrary() = default;
            DynamicLibrary(const char* sPath);

            bool Load(const char* sPath);
            void Unload() const;
            void* GetSymbol(const char* sName) const;

            const char* GetLastError() const;

        private:
            ON_LINUX (
                void* m_pLibHandle = NULL;
            )
    };
}

#endif
