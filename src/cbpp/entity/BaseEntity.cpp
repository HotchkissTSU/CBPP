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

    const EPropNode* BaseEntity::GetProperties() const noexcept {
        return const_cast<const EPropNode*>(m_pPropsHead);
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

    BaseEntity::~BaseEntity() {
        delete m_pPropsHead;
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

    void BaseEntity::Print(FILE* hStream) const {
        fprintf(hStream, "Entity of class '%s':\n", Class());
        EPropNode* pCurrent = m_pPropsHead;
        
        if(pCurrent == NULL) {
            fprintf(hStream, "\tNo attributes providen\n");
            return;
        }

        size_t iCounter = 1;
        while(pCurrent != NULL) {
            fprintf(hStream, "\t[%u] %s = ", iCounter, pCurrent->m_pProperty->Name());
            pCurrent->m_pProperty->Print(hStream);
            fprintf(hStream, "\n");

            iCounter++;
            pCurrent = pCurrent->m_pNextNode;
        }
    }

    size_t BaseEntity::SPrint(char* sTarget, size_t iMax) const {
        size_t iWritten = 0;

        iWritten += snprintf(sTarget, iMax, "Entity of class '%s':\n", Class());
        EPropNode* pCurrent = m_pPropsHead;
        
        if(pCurrent == NULL) {
            iWritten += snprintf(sTarget, iMax, "\tNo attributes providen\n");
            return iWritten;
        }

        size_t iCounter = 1;
        while(pCurrent != NULL) {
            iWritten += snprintf(sTarget, iMax, "\t[%u] %s = ", iCounter, pCurrent->m_pProperty->Name());
            iWritten += pCurrent->m_pProperty->SPrint(sTarget+iWritten, iMax);
            iWritten += snprintf(sTarget+iWritten, iMax, "\n");

            iCounter++;
            pCurrent = pCurrent->m_pNextNode;
        }

        return iWritten;
    }
}
