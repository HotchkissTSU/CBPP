#ifndef CBPP_ENT_BASE_H
#define CBPP_ENT_BASE_H

#include <map>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "cbpp/vec2.h"
#include "cbpp/cbdef.h"
#include "cbpp/error.h"
#include "cbpp/yyjson.h"
#include "cbpp/print.h"

/*
    Connect a class to a custom text label
    Must appear AFTER a class declaration
    Registered entities can be created via their text name
*/
#define CB_ENTITY_REGISTER(textName, className)\
    const char* className::Class() const noexcept { return #textName; }\
    BaseEntity* cb_entfactory_##className(){\
        return static_cast<BaseEntity*>(new className());\
    }\
    static cbpp::EntityRegistrator _g_##className##Registrator(#textName, &cb_entfactory_##className);

//Begin describing this classes attributes
#define CB_EPROP_DESC virtual void ConstructProps() noexcept override

#define CB_EPROP(member_name) EntityProperty<decltype(member_name)>* member_name##_EPROP = \
                                    new EntityProperty<decltype(member_name)>(this, member_name, #member_name, NULL);

#define CB_EPROP_EX(member_name, name, desc) EntityProperty<decltype(member_name)>* member_name##_EPROP = \
                                    new EntityProperty<decltype(member_name)>(this, member_name, name, desc);

//Entity properties
namespace cbpp {
    class BaseEntity;

    class IProperty : public BasePrintable {
        public:
            enum PARAMS : uint8_t {
                PARAM_EMPTY       = 0b00000000, //Desolate, dead-silent bitmask, with zero signs of life
                PARAM_ALL         = 0b11111111, //All and everything
                PARAM_NETWORKABLE = 0b00000001, //Sync this value between the client and the server
                PARAM_SAVELOAD    = 0b00000010  //Save and restore this value
            };

            virtual void Print(FILE* hTarget = stdout) const = 0;
            virtual size_t SPrint(char* sTarget, size_t iMaxWrite) const = 0;

            virtual const char* Name() const noexcept = 0;
            virtual const char* Desc() const noexcept = 0;
            virtual BaseEntity* GetMaster() noexcept = 0;

            virtual size_t Sizeof() const noexcept = 0;
            virtual void Serialize(void* aTarget) const noexcept = 0;
            virtual void Deserialize(void* aSource) noexcept = 0;
            virtual const void* GetBuffer() const noexcept = 0;

            ~IProperty() = default;
    };

    /*
        Entity properties are stored in an one-way linked list.
        Each knot in an inheritance tree adds his own properties in 
        this list in it`s constructor via ConstructProps() call.
    */
    struct EPropNode {
        EPropNode(){};
        EPropNode(EPropNode* pPrevNode, IProperty* pValue);

        IProperty* GetByName(const char* sPropName) noexcept;

        EPropNode* m_pNextNode;
        IProperty* m_pProperty;

        ~EPropNode();
    };

    //A wrapper for class members to store them as meta-properties
    template <typename T> class EntityProperty : public IProperty {
        public:
            virtual void Print(FILE* hTarget = stdout) const {
                cbpp::Print<T>(m_Data, hTarget);
            }

            virtual size_t SPrint(char* sTarget, size_t iMaxWrite) const {
                return cbpp::SPrint<T>(m_Data, sTarget, iMaxWrite);
            }

            virtual const char* Name() const noexcept override {
                return m_sName;
            }

            virtual const char* Desc() const noexcept override {
                return m_sDesc;
            }

            operator T() {
                return m_Data;
            }

            EntityProperty(BaseEntity* pMaster, 
                           T& refData, 
                           const char* sName, 
                           const char* sDesc) noexcept;

            void operator=(const T& other) {
                m_Data = other;
            }

            virtual BaseEntity* GetMaster() noexcept override {
                return m_pMaster;
            }

            virtual size_t Sizeof() const noexcept override { return sizeof(T); }

            virtual void Serialize(void* aBuffer) const noexcept override {
                memcpy(aBuffer, &m_Data, sizeof(T));
            }

            virtual void Deserialize(void* aSource) noexcept override {
                memcpy(&m_Data, aSource, sizeof(T));
            }

            virtual const void* GetBuffer() const noexcept override {
                return reinterpret_cast<const void*>(&m_Data);
            }

        private:
            const char *m_sName = NULL, *m_sDesc = NULL;
            BaseEntity* m_pMaster = NULL;
            IProperty::PARAMS m_iParams = PARAM_EMPTY;
            T& m_Data;
    };
}

namespace cbpp {
    //Entity ID
    typedef size_t entid_t;

    class World;

    //The basis for all game entities
    class BaseEntity : public BasePrintable {
        CB_VAR_GETSETE(Vec2, Position, m_vPos)
        CB_VAR_GETSETE(float_t, Angle, m_fAngle)
        CB_VAR_GETSETE(bool, Spawned, m_bSpawned)

        public:
            virtual void Print(FILE* hTarget = stdout) const;
            virtual size_t SPrint(char* sTarget, size_t iMaxWrite) const;

            BaseEntity(){ ConstructProps(); }
            BaseEntity(BaseEntity* pMaster) : m_pMaster(pMaster) { ConstructProps(); }

            size_t GetDumpLength() const noexcept;
            void Dump(uint8_t* pTarget) const noexcept;
            void Load(uint8_t* pSource) noexcept;

            yyjson_mut_val* ToJSON() const noexcept;
            void FromJSON(yyjson_val* jValue) noexcept;
            
            virtual void ConstructProps() noexcept {
                m_pPropsHead = NULL;
                CB_EPROP_EX(m_vPos, "Position", "Position of the entity")
                CB_EPROP_EX(m_fAngle, "Angle", "Rotation of the entity")
            }

            EPropNode*& GetProperties() noexcept;
            const EPropNode* GetProperties() const noexcept;

            virtual const char* Class() const noexcept = 0;

            //One logic update
            virtual void Tick() = 0;

            //Rendering time!
            virtual void Render() = 0;

            virtual ~BaseEntity();
            
        protected:
            bool m_bSpawned = false;
            World* m_pWorld = NULL; //The world this entity is in
            size_t m_iChunk = -1;   //Our chunk

            //Our movement and rotation are relative to this entity
            BaseEntity* m_pMaster = NULL;

            Vec2 m_vPos;
            float_t m_fAngle = 0.0f;

            EPropNode* m_pPropsHead;
    };

    template <typename T> EntityProperty<T>::EntityProperty(BaseEntity* pMaster, 
                    T& refData, 
                    const char* sName, 
                    const char* sDesc) noexcept : m_Data(refData), m_sName(sName), m_sDesc(sDesc), m_pMaster(pMaster)
    {
        EPropNode*& pMasterProps = pMaster->GetProperties();

        if(pMasterProps != NULL) {
            EPropNode* pNode = new EPropNode();
            pNode->m_pNextNode = pMasterProps;
            pNode->m_pProperty = static_cast<IProperty*>(this);
            pMasterProps = pNode;
        }else{
            pMasterProps = new EPropNode(); //This property is the first one to be attached,
            pMasterProps->m_pNextNode = NULL; //so prepare the linked list head
            pMasterProps->m_pProperty = static_cast<IProperty*>(this);
        }
    }

    std::map<const char*, BaseEntity* (*)(void)>& GetEntityFactories() noexcept;

    //An extremely dumb way of executing some code outside of any function
    //This bro simply inserts a function pointer in a global static table
    class EntityRegistrator {
        public:
            EntityRegistrator(const char* classname, BaseEntity* (*pfactory)(void));            
    };

    //Create an entity by it`s classname
    BaseEntity* CreateEntity(const char* sClassName);
}

#endif
