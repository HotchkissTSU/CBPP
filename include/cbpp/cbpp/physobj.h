#ifndef CBPP_PHYSOBJ_H
#define CBPP_PHYSOBJ_H

#include "cbpp/vec2.h"

namespace cbpp {
    struct CollisionInfo {
        bool Result = false;
        Vec2 HitPoint;  //Average position of the collision
        Vec2 HitNormal; //The normal vector of the A and B impact, relative to the A
    };

    enum HITBOX_TYPE : uint8_t {
        HITBOX_AABB,            // Axis-aligned bounding box
        HITBOX_OBB,             // Oriented bounding box
        HITBOX_CIRCLE,          // A perfect circle
        HITBOX_CONVEX_MULTIPLE  // A set of convex meshes
    };

    class IHitbox {
        CB_VAR_GETSETE(Vec2, Pos, m_vPos)

        public:
            IHitbox(HITBOX_TYPE iHBclass) : m_iType(iHBclass) {};

            virtual CollisionInfo TestCollision(IHitbox* pOther) const = 0;
            virtual void GetRotated(IHitbox* pTarget, float_t fAngle) const = 0;
            virtual HITBOX_TYPE GetType() const noexcept = 0;

            virtual void ComputeSurface() = 0;

            float_t GetSurface() noexcept {
                if(!m_bSurfComputed) {
                    ComputeSurface();
                    m_bSurfComputed = true;
                }

                return m_fSurface;
            }

            virtual ~IHitbox() = default;

        protected:
            HITBOX_TYPE m_iType;

            Vec2 m_vPos;

            float_t m_bSurfComputed = false;
            float_t m_fSurface = 0.0f;
    };

    //A perfect circle hitbox
    class CircleHitbox : public IHitbox {
        CB_VAR_GETE(float_t, Radius, m_fRadius)

        public:
            CircleHitbox(float_t fRadius) : m_fRadius(fRadius), IHitbox(HITBOX_CIRCLE) {};

            void SetRadius(float_t fNewRad) noexcept;

            virtual void GetRotated(IHitbox* pTarget, float_t fAngle) const noexcept {
                //Rotating a circle probably have no meaning, so do nothing
            }

            virtual void ComputeSurface();
            virtual CollisionInfo TestCollision(IHitbox* pOther) const;

        private:
            float_t m_fRadius = 0.0f;
    };
}

#endif
