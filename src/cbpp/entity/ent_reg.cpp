#include "cbpp/entity/ent_reg.h"
#include <stdio.h>

#define REG(class_name, factory_name) Factory()->RegisterEntityClass( #class_name, factory_name )

namespace cbent {
    EntityFactory* Factory() {
        static EntityFactory ent_manager;
        return &ent_manager;
    }

    void RegisterEntities() {
        REG( base_entity, BaseEntity::Create );
        REG( test_entity, TestEntity::Create );

        Factory()->Print();
    }

    void EntityFactory::RegisterEntityClass(const char* class_name, BaseEntity* (*factory_ptr)(void)) {
        if(ent_reg.count(class_name) > 0) {
            char err_log[256];
            snprintf(err_log, 256, "Attempt to re-register entity class '%s'", class_name);
            CbThrowError(err_log);
        }
        printf("Entity class registration: %s\n", class_name);
        ent_reg[class_name] = factory_ptr;
    }

    BaseEntity* EntityFactory::CreateEntity(const char* class_name) {
        if(ent_reg.count(class_name) == 0) {
            char err_log[256];
            snprintf(err_log, 256, "Attempt to create entity of the unknown class '%s'", class_name);
            CbThrowError(err_log);
        }

        return ent_reg.at(class_name)();
    }

    void EntityFactory::Print() {
        printf("List of currently registered entities:\n");
        for(auto it = ent_reg.begin(); it != ent_reg.end(); it++) {
            const char* ent_classname = it->first.c_str();
            BaseEntity* (*ent_factory)( void ) = it->second;

            printf("%s -> %x\n", ent_classname, ent_factory);
        }
    }
}