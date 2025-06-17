#include "cbpp/polygon.h"
#include "cbpp/geomath.h"

namespace cbpp {
    bool PolygonIsConvex(List<cbvs::SpriteVertex>& aSourceVerts) {
        for( size_t i = 1; i < aSourceVerts.Length()-1; i++ ) {
            Vec2 vPrev = aSourceVerts.At(i-1).vPos;
            Vec2 vCur = aSourceVerts.At(i).vPos;

            Vec2 vSideVector = vCur - vPrev;

            for( size_t j = 0; j < aSourceVerts.Length(); j++ ) {
                Vec2 vPoint = aSourceVerts.At(i).vPos;
                if(i != j) {
                    Vec2 vCheck = vPoint - vPrev;
                    float_t fDot = vSideVector.Dot(vCheck);

                    if(fDot >= 0) {
                        return false;
                    }
                }
            }
        }

        return true;
    }
}
