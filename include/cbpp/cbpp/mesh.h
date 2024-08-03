#ifndef CBPP_MESH_H
#define CBPP_MESH_H

#include <cstdint>
#include <vector>

#include "cbpp/vec2.h"
#include "cbpp/ttype/segarr.h"

namespace cbpp {	
	struct MeshVertex {
		Vec2 pos;
		float angle;
	};

	class Mesh {
		public:
			Mesh();
			Mesh(uint32_t len);
			Mesh(Vec2* varr, uint32_t vlen);
			Mesh(const std::vector<Vec2>& vvec);
			
			Mesh(Mesh& other);
			
			void Set(Mesh* src);
			void Set(Vec2* varr, uint32_t vlen);
			
			bool IsValid();
			uint32_t Size();
			
			void AdjustVertex(Vec2 vtx);
			void InsertVertex(uint32_t index, Vec2 vtx);
			void PopVertex(uint32_t vid);
			
			void RotateByCenter(float ang);
			Mesh GetRotatedByCenter(float ang);
			
			void RotateByOffset(float ang, Vec2 offset);
			Mesh GetRotatedByOffset(float ang, Vec2 offset);
			
			void MakeConvex();
			Mesh GetConvex();			
			
			void Clip(Vec2 pos1, Vec2 pos2, int8_t dir);
			Mesh GetClipped(Vec2 pos1, Vec2 pos2, int8_t dir);
			
			void Triangulate();
			SegArray<Mesh> GetTriangulated();
			
			bool CollidePoint(Vec2 pt);		

			const Vec2* GetArray();
			const float* GetArrayf();
			
			Vec2& operator[](uint32_t index);
			Vec2& At(uint32_t index);
			
			void Allocate(uint32_t length);
			void Free();
			
			~Mesh();
			
		private:
			bool allocated = false;
			Vec2* vecarr = nullptr;
			uint32_t arrlen = 0;
			
			Vec2 nullvec = Vec2(0);
	};
}

#endif