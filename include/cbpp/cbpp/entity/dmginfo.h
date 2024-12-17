#ifndef CBPP_DMGINFO_H
#define CBPP_DMGINFO_H

#include "cbpp/cbdef.h"
#include "cbpp/vec2.h"

namespace cbent {
    class Entity;

    struct DamageInfo {
        Entity* m_eAttacker = NULL, m_eVictim = NULL;
        cbpp::float_t m_fDamage = (cbpp::float_t)(0.0f);
        cbpp::Vec2 m_vDmgPos; //global position of where the damage is applied
    };
}

#endif
