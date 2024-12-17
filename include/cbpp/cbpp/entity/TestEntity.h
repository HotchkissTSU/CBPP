#ifndef CBPP_ENT_TEST_H
#define CBPP_ENT_TEST_H

#include "cbpp/entity/BaseEntity.h"

namespace cbpp {
    class TestEntity : public BaseEntity {
        private:
            EntityProperty<float> m_pfTestFloat = EntityProperty<float>(15.0f, "test_float", "test? maybe");

            CB_EPROP_BEGIN
                &m_pfTestFloat
            CB_EPROP_END

        public:
            TestEntity(){};

            virtual void Tick(){}
            virtual void Render(){}

            virtual const char* Class();

            virtual ~TestEntity(){}
    };
    CB_ENTITY_REGISTER(ent_test, TestEntity)
}

#endif
