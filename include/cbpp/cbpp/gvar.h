#ifndef CBPP_GVAR_H
#define CBPP_GVAR_H

#include <map>
#include "cdf/cdf.h"

#include "cbpp/cbdef.h"
#include "cbpp/cbstring.h"

namespace cbpp {
    enum GVAR_TYPE : uint8_t {
        GVAR_TYPE_NUMBER,
        GVAR_TYPE_STRING
    };

    enum GVAR_CLASS : uint8_t {
        GVAR_CLASS_GLOBAL,          // A global variable. Affects everything, exists everywhere
        GVAR_CLASS_SAVE,            // Exists only in it`s game savefile
        GVAR_CLASS_SESSION,         // Variable, local to the current launch session
        GVAR_CLASS_MAP,             // Local to the currently played map

        GVAR_CLASSES_AMOUNT
    };

    class GVar {
        public:
            union Value {
                float_t f32;
                char* str;
            };

            GVar(const char* sName, float_t fValue);
            GVar(const char* sName, const char* sValue);

            float_t GetNumber() const;
            const char* GetString() const;

            bool SetValue(float_t fNumber) noexcept;
            bool SetValue(const char* sString) noexcept;

            const char* Name() const;

        private:
            GVAR_TYPE m_iType;
            Value m_uData;
            const char* m_sName;
    };

    class GVarNamespace {
        CB_VAR_GET(GVAR_CLASS, Class, m_iClass)

        public:
            const char* Name() const noexcept;
            cdf_object* Save(cdf_document* pDoc = NULL) const noexcept;

            //Reset all values to 0
            void Reset() noexcept;

            //Remove all values
            void Clear() noexcept;

        private:
            std::map<CString, GVar> m_mValues;
    };

    extern GVarNamespace g_GVarScopes[];
    extern const char* g_aGVarScopeNames[];
}

#endif
