#include "cbpp/physobj.h"

#include "cbpp/geomath.h"

// Circle hitbox
namespace cbpp {
    void CircleHitbox::SetRadius(float_t fNewRad) noexcept {
        m_fRadius = fNewRad;
        m_bSurfComputed = false;
    }

    void CircleHitbox::ComputeSurface() {
        m_fSurface = CBPP_MATH_PI * m_fRadius * m_fRadius;
    }

    CollisionInfo CircleHitbox::TestCollision(IHitbox* pOther) const {
        CollisionInfo out;

        switch(pOther->GetType()) {
            case HITBOX_CIRCLE: { //circle-circle
                float_t fDist = cbpp::math::SquareDistance(m_vPos, pOther->GetPos());

                //math::Intersection icCircCollide = math::Intersect();

                break;
            }
        }

        return out;
    }
}
