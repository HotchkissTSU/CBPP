#ifndef CBPP_ENT_ENTREG_H
#define CBPP_ENT_ENTREG_H

#include <map>
#include <string>

#include "cbpp/entity/base_entity.h"
#include "cbpp/error.h"

namespace cbent {
    class EntityFactory {
        public:
            BaseEntity* CreateEntity(const char* class_name);

            void RegisterEntityClass(const char* class_name, BaseEntity* (*factory_ptr)( void ));

            void Print();

        private:
            std::map<std::string, BaseEntity* (*)( void )> ent_reg;
    };

    EntityFactory* Factory();
    void RegisterEntities();
}

#endif