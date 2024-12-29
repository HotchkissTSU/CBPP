#ifndef CBPP_ENT_BASE_H
#define CBPP_ENT_BASE_H

#include <map>
#include <cstdint>

#include "cbpp/vec2.h"
#include "cbpp/cbdef.h"
#include "cbpp/error.h"

/*
    Connect a class to a custom text label
    Must appear AFTER the class declaration
    Registered entities can be created via their text classname
*/
#define CB_ENTITY_REGISTER(textName, className)\
    const char* className::Class() { return #textName; }\
    BaseEntity* cb_entfactory_##className() { return static_cast<BaseEntity*>(new className()); }\
    static cbpp::EntityRegistrator _g_##className##Registrator(#textName, &cb_entfactory_##className);

#define CB_EPROP_BEGIN\
    public: virtual IProperty** GetProperties() noexcept {\
        static IProperty* s_aProps[] = {

#define CB_EPROP_END\
        };\
        return s_aProps;\
    }\
    virtual size_t GetPropertiesNum() noexcept {\
        IProperty** aProps = this->GetProperties();\
        return sizeof(aProps) / sizeof(IProperty*);\
    }\
    virtual IProperty* GetPropertyByName(const char* sPName) noexcept {\
        return __get_prop_by_name(this->GetProperties(), this->GetPropertiesNum(), sPName);\
    }

//Entity properties
namespace cbpp {
    class IProperty {
        public:
            enum PARAMS : uint8_t {
                PARAM_EMPTY       = 0b00000000, //Empty bitmask
                PARAM_NETWORKABLE = 0b00000001, //Send this value to the client/server
                PARAM_SAVELOAD    = 0b00000010  //Save and restore this value
            };

            virtual const char* Name() const noexcept = 0;
            virtual const char* Desc() const noexcept = 0;

            virtual size_t Sizeof() const noexcept = 0;
            virtual void Serialize(void* aTarget) const noexcept = 0;
            virtual void Deserialize(void* aSource) noexcept = 0;
            virtual const void* GetBuffer() const noexcept = 0;
    };

    //A wrapper for class members to store them as a meta-properties
    template <typename T> class EntityProperty : public IProperty {
        public:
            virtual const char* Name() const noexcept override {
                return m_sName;
            }

            virtual const char* Desc() const noexcept override {
                return m_sDesc;
            }

            operator T() {
                if(m_Data == NULL) {
                    CbThrowErrorf("Property '%s' has no value attached", m_sName);
                }

                return m_Data;
            }

            EntityProperty(const T& refData, const char* sName, const char* sDesc) noexcept : m_Data(refData), m_sName(sName), m_sDesc(sDesc) {};
            EntityProperty(const T& refData, const char* sName)                    noexcept : m_Data(refData), m_sName(sName), m_sDesc(NULL) {};

            void operator=(const T& other) {
                m_Data = other;
            }

            /*T* operator&() {
                return &m_Data;
            }*/

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
            IProperty::PARAMS m_iParams = PARAM_EMPTY;
            T m_Data;
    };
}

namespace cbpp {
    //Entity ID
    typedef size_t entid_t;

    //spooky scary hidden function
    IProperty* __get_prop_by_name(IProperty** aProps, size_t iPropsNum, const char* sName);

    class World;

    //The basis for all game entities
    class BaseEntity {
        CB_VAR_GETSETE(Vec2, Position, m_vPos)
        CB_VAR_GETSETE(float_t, Angle, m_fAngle)
        CB_VAR_GETSETE(bool, Spawned, m_bSpawned)

        public:
            BaseEntity(){};
            BaseEntity(BaseEntity* pMaster) : m_pMaster(pMaster) {};

            virtual IProperty** GetProperties() = 0;
            virtual size_t GetPropertiesNum() = 0;
            virtual IProperty* GetPropertyByName(const char* sName) = 0;

            virtual const char* Class() = 0;

            //One iteration of the logic update
            virtual void Tick() = 0;

            //Rendering time!
            virtual void Render() = 0;

            virtual ~BaseEntity() = default;
            
        protected:
            bool m_bSpawned = false;
            World* m_pWorld = NULL; //The world this entity is in
            size_t m_iChunk = -1;   //Our chunk

            //Our movement and rotation are relative to this entity
            BaseEntity* m_pMaster = NULL;

            Vec2 m_vPos;
            float_t m_fAngle;
    };

    /*
        I have to declare this global table this terrible way to ensure that it`s static initialisation happens
        before ones for entity registrators that refer it (or segfault happens).

        Basically, all files where this header is included will have a stupid empty instance of this poor table.
        Literally all other ways of declaration lead to segfault even before main() is called.
    */
    static std::map<const char*, BaseEntity* (*)(void)> g_mEntFactoryDict;

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
