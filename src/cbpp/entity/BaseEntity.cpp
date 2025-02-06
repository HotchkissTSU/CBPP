#include "cbpp/entity/BaseEntity.h"

#include <string.h>

#include "cbpp/error.h"

namespace cbpp {
    std::map<const char*, BaseEntity* (*)(void)>& GetEntityFactories() noexcept {
        static std::map<const char*, BaseEntity* (*)(void)> s_mEntityFactoryDict;
        return s_mEntityFactoryDict;
    }
    
    EntityRegistrator::EntityRegistrator(const char* classname, BaseEntity* (*pfactory)(void)) {
        if(GetEntityFactories().count(classname) > 1) {
            CbThrowErrorf("Attempt to re-register entity class with name '%s'", classname);
        }

        GetEntityFactories()[classname] = pfactory;
    }

    BaseEntity* CreateEntity(const char* sClassName) {
        if(GetEntityFactories().count(sClassName) == 0) {
            return NULL;
        }

        return GetEntityFactories()[sClassName]();
    }

    EPropNode*& BaseEntity::GetProperties() noexcept {
        return m_pPropsHead;
    }

    EPropNode::EPropNode(EPropNode* pPrevNode, IProperty* pValue) {
        m_pProperty = pValue;
        
        if(pPrevNode != NULL) {
            pPrevNode->m_pNextNode = this;
        }
    }

    IProperty* EPropNode::GetByName(const char* sPropName) noexcept {
        if(m_pNextNode == NULL) {
            return NULL;
        }

        EPropNode* pCurrent = this;
        while(pCurrent != NULL) {
            if( strcmp(pCurrent->m_pProperty->Name(), sPropName) == 0 ) {
                return pCurrent->m_pProperty;
            }

            pCurrent = pCurrent->m_pNextNode;
        }

        return NULL;
    }

    EPropNode::~EPropNode() {
        if(m_pNextNode != NULL) {
            m_pNextNode->~EPropNode();
        }

        delete m_pProperty;
    }
}
