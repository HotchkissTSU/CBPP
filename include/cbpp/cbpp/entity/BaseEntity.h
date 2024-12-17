#ifndef CBPP_ENT_BASE_H
#define CBPP_ENT_BASE_H

#include <map>
#include <cstdint>

#include "cbpp/vec2.h"
#include "cbpp/cbdef.h"
#include "cbpp/error.h"

//Connect a class to a custom text label
//Must appear AFTER the class declaration
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
    }\

//Entity properties
namespace cbpp {
    class IProperty {
        public:
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
            T m_Data;
    };
}

namespace cbpp {
    //spooky scary hidden function
    IProperty* __get_prop_by_name(IProperty** aProps, size_t iPropsNum, const char* sName);

    //The basis for all game entities
    class BaseEntity {
        CB_VAR_GETSETE(Vec2, Position, m_vPos)
        CB_VAR_GETSETE(float_t, Angle, m_fAngle)
        CB_VAR_GETSETE(float_t, Health, m_fHealth)
        CB_VAR_GETSETE(float_t, MaxHealth, m_fMaxHealth)

        public:
            BaseEntity(){};
            BaseEntity(BaseEntity* pMaster) : m_pMaster(pMaster) {};

            virtual IProperty** GetProperties() = 0;
            virtual size_t GetPropertiesNum() = 0;
            virtual IProperty* GetPropertyByName(const char* sName) = 0;

            virtual const char* Class() = 0;

            virtual void Tick() = 0;
            virtual void Render() = 0;

            virtual ~BaseEntity() = default;

        protected:
            //Our movement and rotation are relative to this entity
            BaseEntity* m_pMaster = NULL;

            Vec2 m_vPos;
            float_t m_fAngle, m_fHealth, m_fMaxHealth;
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
