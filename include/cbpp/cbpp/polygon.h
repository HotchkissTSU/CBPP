#ifndef CBPP_POLYGON_H
#define CBPP_POLYGON_H

#include "cbpp/vec2.h"
#include "cbpp/ttype/list.h"
#include "cbvs/render.h"

namespace cbpp {
    struct PolygonTris {
        Vec2 A,B,C;
    };

    bool PolygonIsConvex(List<cbvs::SpriteVertex>& aSourceVerts);
    PolygonTris* PolygonTriangulate(List<cbvs::SpriteVertex>& aSourceVerts, size_t& refTrisAmount);
}

#endif
