/*
    The basic class for an AI-driven character who can also be controlled by a player
*/

#ifndef CBPP_ENT_CHARACTER_H
#define CBPP_ENT_CHARACTER_H

#include "cbpp/entity/BaseSolid.h"

namespace cbpp {
    //Abstract entity for a basic AI character
    class BaseCharacter : public BaseRigid {
    };
}

#endif
