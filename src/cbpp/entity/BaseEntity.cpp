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

    size_t BaseEntity::GetDumpLength() const noexcept {
        EPropNode* pCurrent = m_pPropsHead;
        size_t iOutLen = 0;

        while(pCurrent != NULL) {
            iOutLen = iOutLen + pCurrent->m_pProperty->Sizeof();
            pCurrent = pCurrent->m_pNextNode;
        }

        return iOutLen;
    }

    void BaseEntity::Dump(uint8_t* pTarget) const noexcept {
        size_t iOffset = 0;
        EPropNode* pCurrent = NULL;
        size_t iPropSize;

        while(pCurrent != NULL) {
            iPropSize = pCurrent->m_pProperty->Sizeof();

            memcpy(pTarget + iOffset, pCurrent->m_pProperty->GetBuffer(), iPropSize);
            iOffset = iOffset + iPropSize;
            pCurrent = pCurrent->m_pNextNode;
        }
    }

    void BaseEntity::Load(uint8_t* pTarget) noexcept {
        size_t iOffset = 0;
        EPropNode* pCurrent = NULL;
        size_t iPropSize;

        while(pCurrent != NULL) {
            iPropSize = pCurrent->m_pProperty->Sizeof();
            pCurrent->m_pProperty->Deserialize(pTarget + iOffset);

            iOffset = iOffset + iPropSize;
            pCurrent = pCurrent->m_pNextNode;
        }
    }

    yyjson_mut_val* BaseEntity::ToJSON() const noexcept {
        
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

    void PrintEntity(BaseEntity* eTarget, FILE* hStream) noexcept {
        fprintf(hStream, "Entity of class '%s':\n", eTarget->Class());
        EPropNode* pCurrent = eTarget->GetProperties();
        
        if(pCurrent == NULL) {
            fprintf(hStream, "\tNo attributes providen\n");
            return;
        }

        size_t iCounter = 1;
        while(pCurrent != NULL) {
            fprintf(hStream, "\t[%u] %s\n", iCounter, pCurrent->m_pProperty->Name());
            iCounter++;
            pCurrent = pCurrent->m_pNextNode;
        }
    }
}
