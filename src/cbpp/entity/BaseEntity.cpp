#include "cbpp/entity/BaseEntity.h"

#include <string.h>

#include "cbpp/error.h"

namespace cbpp {
    std::map<CString, BaseEntity* (*)(void)>& GetEntityFactories() noexcept {
        static std::map<CString, BaseEntity* (*)(void)> s_mEntityFactoryDict;
        return s_mEntityFactoryDict;
    }
    
    EntityRegistrator::EntityRegistrator(const char* classname, BaseEntity* (*pfactory)(void)) {
        if(GetEntityFactories().count(classname) > 1) {
            CbThrowErrorf("Attempt to re-register entity class with name '%s'", classname);
        }

        GetEntityFactories()[CString(classname)] = pfactory;
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

    BaseEntity::~BaseEntity() {
        delete m_pPropsHead;
    }
    
    cdf_object* BaseEntity::Dump(cdf_document* pDoc) const noexcept {
        cdf_object* pOut = cdf_object_create(pDoc, Class(), CDF_TYPE_OBJECT);

        EPropNode* pCurrent = m_pPropsHead;
        while(pCurrent != NULL) {
            IProperty* pProp = pCurrent->m_pProperty;
            cdf_data_push_ex(pDoc, pOut, pProp->Name(), (void*)(pProp->GetBuffer()), pProp->Sizeof(), CDF_TYPE_BINARY);
            pCurrent = pCurrent->m_pNextNode;
        }

        return pOut;
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
