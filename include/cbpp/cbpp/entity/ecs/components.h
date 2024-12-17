#ifndef CBPP_ENTITY_COMPONENTS_H
#define CBPP_ENTITY_COMPONENTS_H

#include "cbpp/cbdef.h"
#include "cbpp/vec2.h"

#include <cstring>

//Tune the current class to become a proper CBPP entity component
#define CB_ECS(class_name)\
    private:static constexpr cbpp::hash_t comp_uid = cbpp::Hash(#class_name);\
    public: static cbpp::hash_t GetID() { return comp_uid; }\
            static const char* GetName() { return #class_name; }\
            class_name() = default;\
            class_name(Entity* owner) : CBase(owner) {};

#define DT_PROPERTY(type, name, value) Property<type> m_##name = Property<type>(#name, value)
#define DT_PROPERTY_DESC(type, name, desc, value) Property<type> m_##name = Property<type>(#name, #desc, value)

namespace cbent {
    class Entity;
    template<typename T> class Property;

	class IProperty {
		public:
			IProperty& operator=(const IProperty&) = delete;

			template<typename T> Property<T>* Cast() {
				return static_cast<Property<T>*>(this);
			}

			virtual size_t Serialize(void* buffer, size_t ln) const = 0;
			virtual const char* Name() const noexcept = 0;
			virtual const char* Desc() const noexcept = 0;

		protected:
			virtual ~IProperty() = default;
    };

	template <typename T> class Property : public IProperty {
		public:
			Property(const char* i_name, const T& ref) : m_Value(ref), m_sExternalName(i_name), m_sDesc(NULL) {};
			Property(const char* i_name, const char* desc, T& ref) : m_Value(ref), m_sExternalName(i_name), m_sDesc(desc) {};

			virtual size_t Serialize(void* buffer, size_t sz) const override {
				return 0;
			}

			Property<T>& operator=(const T& other) {
				m_Value = other;
				return *this;
			}

			operator T() {
				return m_Value;
			}

			virtual const char* Name() const noexcept { return m_sExternalName; };
			virtual const char* Desc() const noexcept { return m_sDesc; };

			virtual ~Property() override = default;

		private:
			T m_Value;
			const char* const m_sExternalName;

			//field description
			const char* const m_sDesc;
	};
}

/* === ENTITY COMPONENTS === */
namespace cbent {
    class CBase {
		CB_VAR_GETE(Entity*, Owner, m_eOwner);

		public:
			CBase() = default;
			CBase(Entity* owner) : m_eOwner(owner) {};

			virtual bool Init() = 0;
			virtual void Destroy() = 0;

			virtual IProperty** GetDataTable() = 0;
			virtual size_t GetDataTableLength() = 0;

			virtual IProperty* GetIFieldByName(const char* vname) {
				IProperty** aDataTable = GetDataTable();
				for(size_t i = 0; i < GetDataTableLength(); i++) {
					if( strcmp(aDataTable[i]->Name(), vname) == 0 ) {
						return aDataTable[i];
					}
				}

				return NULL;
			}

			template<typename T> Property<T>* GetFieldByName(const char* vname) {
				return dynamic_cast<Property<T>*>( GetIFieldByName(vname) );
			}

		protected:
			Entity* m_eOwner = NULL;
	};

	/*
		Gives the entity a health level and an ability to die/break/etc.
	*/
	class CHealth : public CBase{
		CB_ECS(CHealth)

		public:
			virtual bool Init() { return true; }
			virtual void Destroy() {}

			void Damage(cbpp::float_t aDmg) {
                m_fHealth = (cbpp::float_t)m_fHealth + aDmg;
            }

			cbpp::float_t HealthRatio() {
                return (cbpp::float_t)m_fHealth / (cbpp::float_t)m_fMaxHealth;
            }

			bool IsAlive() {
                return (cbpp::float_t)m_fHealth > (cbpp::float_t)0.0f;
            }

			virtual IProperty** GetDataTable() {
				static IProperty* aDataStatic[] = {
					&m_fHealth,
					&m_fMaxHealth
				};

				return aDataStatic;
			}

			virtual size_t GetDataTableLength() {
				IProperty** aDt = GetDataTable();
				return sizeof(aDt) / sizeof(void*);
			}

		private:
			DT_PROPERTY(cbpp::float_t, fHealth, (cbpp::float_t)0.0f);
			DT_PROPERTY(cbpp::float_t, fMaxHealth, (cbpp::float_t)100.0f);
	};

	/*
		This entity will have physically based motion
	*/
	class CPhysics : public CBase {
		CB_ECS(CPhysics)
		
		public:
			virtual bool Init();
			virtual void Destroy() {}

			void ApplyForce(cbpp::Vec2& aForce);
			void ApplyAngForce(cbpp::float_t aAngle);

			void Simulate(cbpp::float_t adt);

			virtual IProperty** GetDataTable() {
				static IProperty* aDataStatic[] = {
					&m_vVel,
					&m_vAccel,
					&m_fAngVel,
					&m_fAngVel,
					&m_fMass
				};

				return aDataStatic;
			}

			virtual size_t GetDataTableLength() {
				IProperty** aDt = GetDataTable();
				return sizeof(aDt) / sizeof(void*);
			}

		private:
			DT_PROPERTY(cbpp::Vec2, vVel, cbpp::Vec2());
			DT_PROPERTY(cbpp::Vec2, vAccel, cbpp::Vec2());
			DT_PROPERTY(cbpp::float_t, fAngVel, (cbpp::float_t)0.0f);
			DT_PROPERTY(cbpp::float_t, fAngAccel, (cbpp::float_t)0.0f);
			DT_PROPERTY(cbpp::float_t, fMass, (cbpp::float_t)1.0f);
	};
}

#endif