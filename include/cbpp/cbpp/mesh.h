#ifndef CBPP_MESH_H
#define CBPP_MESH_H

#include "cbpp/vec2.h"
#include "cbpp/ttype/array.h"

namespace cbpp {
    //A vector with an additional U,V pair
    struct Vertex : public Vec2 {
        float_t u,v;

        Vec2 UV() { return Vec2(u,v); }
        Vec2 XY() { return Vec2(x,y); }
    };
    
    class Mesh {
        public:
            Mesh(){};
            Mesh(size_t iVtxCount) : m_aVerts(iVtxCount) {};
            Mesh(const Mesh& aOther) : m_aVerts(aOther.GetArray()) {};
            Mesh(Array<Vertex>& aOther) : m_aVerts(aOther) {};

            const Array<Vertex>& GetArray() const;
            size_t Size() const;
            bool Resize(size_t iVtxCount);

            bool IsValid() const;

            //The indexation is looped within mesh boundaries
            Vertex& At(size_t iIndex);
            Vertex& operator[](size_t iIndex);

            void PushVertex(const Vertex& vOther);

            void Sort();
            void ComputeMid();

            void Triangulate();
            Mesh GetTriangulated() const;

            void MakeConvex();
            Mesh ConvexHull() const;

            void Rotate(float_t fAng);
            Mesh GetRotated(float_t fAng) const;

        private:
            Array<Vertex> m_aVerts;
            Vec2 m_vMidPos;
    };
}

#endif
