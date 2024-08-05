#ifndef CBPP_MESH_H
#define CBPP_MESH_H

#include <cstdint>
#include <vector>

#include "cbpp/vec2.h"
#include "cbpp/ttype/segarr.h"

namespace cbpp {	
	class MeshVertex : public Vec2 {
		public:
			float u, v;
			
			MeshVertex(){};
			MeshVertex(Vec2& other);
			
			Vec2 GetUV();
			void SetUV(float u, float v);
			void SetUV(Vec2 uv);
	};
	
	class Mesh {
		public:
			Mesh();
			Mesh(Mesh& other, bool sorted = false);
			Mesh(const MeshVertex* varr, uint32_t vlen, bool sorted = false);
			Mesh(SegArray<MeshVertex>& sarr, bool sorted = false);
			
			void ConvexHull(Mesh& target);
			void Triangulate(SegArray<Mesh>& target);
			void TriangulateConvex(SegArray<Mesh>& target); //use simple triangulation for convex meshes
			void Clip(Mesh& target, Vec2 p1, Vec2 p2, int8_t dir);
			void Clip2(Mesh& target_1, Mesh& target_2, Vec2 p1, Vec2 p2); //clip and save both parts of the mesh
			
			void CenterRotate(Mesh& target, float angle);
			void OffsetRotate(Mesh& target, Vec2 offset, float angle);
			
			void AdjustVertex(Vec2 vtx, Vec2 uv = Vec2());
			MeshVertex PopBack();
			MeshVertex PopFront();
			
			MeshVertex& At(int64_t index);
			
			uint32_t Length();
			
			float* Unpack();
			float* UnpackXY();
			float* UnpackUV();
			
		private:
			void compute_mid();
			void sort_mesh();
			bool compare_vtx(MeshVertex& a, MeshVertex& b);
			
			Vec2 midpos;
			uint32_t arrlen = 0;
			MeshVertex* vecarr = nullptr;
			bool allocated = false;
	};
}

#endif