#ifndef CBPP_ENT_TEST_H
#define CBPP_ENT_TEST_H

#include "cbpp/entity/BaseEntity.h"

namespace cbpp {
    class TestEntity : public BaseEntity {
        private:
            float m_fTest = 15.8f;

        public:
            CB_EPROP_DESC {
                CB_EPROP_EX(m_fTest, "test_float", "a test float member")
            }

            TestEntity(){ TestEntity::ConstructProps(); }

            virtual void Tick(){}
            virtual void Render(){}

            virtual const char* Class() const noexcept;

            virtual ~TestEntity(){}
    };
    CB_ENTITY_REGISTER(ent_test, TestEntity)
}

#endif
