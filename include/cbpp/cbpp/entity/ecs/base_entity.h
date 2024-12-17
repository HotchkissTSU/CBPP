#ifndef CBPP_BASE_ENTITY_H
#define CBPP_BASE_ENTITY_H

#include "cbpp/vec2.h"
#include "cbpp/cbdef.h"
#include "cbpp/cbstring.h"

#include <map>
#include <cstdint>

#include "cbpp/entity/components.h"

namespace cbent {
	//what type is used to store entity ID`s
	typedef uint32_t entid_t;

	class Entity;
	
	template<class T> CBase* CreateComponent(Entity* owner) {
		return static_cast<CBase*>(new T(owner));
	}

	class Entity {
		public:
			Entity() = default;

			template<class T> void AttachComponent() {
				cbpp::hash_t comp_id = T::GetID();

				if(m_mComponents.count(comp_id) == 0) {
					m_mComponents[comp_id] = CreateComponent<T>(this);
				}
			}

			template<class T> T* GetComponent() {
				cbpp::hash_t comp_hash = T::GetID();
				
				if(m_mComponents.count(comp_hash) > 0) {
					return (T*)( m_mComponents.at(comp_hash) );
				}else {
					return NULL;
				}
			}

			template<class T> void DetachComponent() {
				cbpp::hash_t comp_uid = T::GetID();

				if( m_mComponents.count(comp_uid) > 0 ) {
					m_mComponents.erase(comp_uid);
				}
			}

			bool Spawn();

			~Entity() {
				for(auto it = m_mComponents.begin(); it != m_mComponents.end(); it++) {
					delete it->second;
				}
			}

		private:
			std::map<cbpp::hash_t, CBase*> m_mComponents;

			cbpp::Vec2 m_vPos;
			float_t m_fAng;
			Entity* m_eParent = NULL;
	};
}

#endif