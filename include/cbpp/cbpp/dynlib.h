#ifndef CBPP_DYNLIB_H
#define CBPP_DYNLIB_H

#include "cbdef.h"

ON_LINUX( #include <dlfcn.h> )
ON_WINDOWS( #include <Windows.h> )

namespace cbpp {
    class DynamicLibrary {
        public:
            DynamicLibrary() = default;
            DynamicLibrary(const char* sPath);

            void Load(const char* sPath);
            void* GetSymbol(const char* sName);
    };
}

#endif
