#include "cbpp/entity/BaseEntity.h"

#include <cstring>

#include "cbpp/error.h"

namespace cbpp {
    EntityRegistrator::EntityRegistrator(const char* classname, BaseEntity* (*pfactory)(void)) {
        if(g_mEntFactoryDict.count(classname) > 1) {
            CbThrowErrorf("Attempt to re-register entity class with name '%s'", classname);
        }

        g_mEntFactoryDict[classname] = pfactory;
    }

    BaseEntity* CreateEntity(const char* sClassName) {
        if(g_mEntFactoryDict.count(sClassName) == 0) {
            return NULL;
        }

        return g_mEntFactoryDict[sClassName]();
    }
    
    IProperty* __get_prop_by_name(IProperty** aProps, size_t iPropsNum, const char* sName) {
        for(size_t i = 0; i < iPropsNum; i++) {
            if( strcmp(sName, aProps[i]->Name()) == 0 ) {
                return aProps[i];
            }
        }

        return NULL;
    }
}
