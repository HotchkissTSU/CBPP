#ifndef CBPP_MESH
#define CBPP_MESH

#include <vector>
#include <cstdint>
#include <cmath>

#include "cbpp/vec2.h"
#include "cbpp/misc.h"

namespace cbpp{
	extern "C" class Triplex{ //треугольник для триангулированных мешей
		public:
			Triplex();
			Triplex(std::vector<Vec2> verts);
			Triplex(Vec2 p1, Vec2 p2, Vec2 p3);
			
			bool CollidePoint(Vec2 p);
			bool CollideTriplex(Triplex tripx);
			Vec2 operator[](short index);
			
		private:
			Vec2 pos1,pos2,pos3;
	};
	
	extern "C" class Mesh{
		public:			
			Mesh();
			Mesh(std::size_t size);
			Mesh(std::vector<Vec2> verts);
			
			Vec2& operator[](std::size_t index);
			Vec2& At(std::size_t index);
			void Set(std::size_t index, Vec2 p);
			
			void SetMesh(std::vector<Vec2> verts);
			void SetMesh(Mesh* msh);
			
			std::size_t Size();
			void SetSize(std::size_t sz);
			
			void GetConvexHull(Mesh* target); //-
			void MakeConvex(); //-
			
			std::vector<Triplex> GetTriangulated();
			std::vector<Vec2> GetLineIntersections(Vec2 pos1, Vec2 pos2);
			
			bool CollidePoint(Vec2 p);
			bool CollideMesh(Vec2 p);
			
			void AdjustVertex(Vec2 p);
			void PopVertex(std::size_t index);
			void Clear();
			
			void GetRotatedByCenter(Mesh* target, float ang);
			void RotateByCenter(float ang);
			
			void GetRotatedByOffset(Mesh* target, float ang, Vec2 p);
			void RotateByOffset(float ang, Vec2 p);
			
			void Scale(float scal);
			void GetScaled(Mesh* target, float scal);
			
			void Clip(Vec2 pos1, Vec2 pos2, int dir);
			void GetClipped(Mesh* target, Vec2 pos1, Vec2 pos2, int dir);
			
			int16_t* GetXArray();
			int16_t* GetYArray();
			void BuildArrays();
			
			Mesh& operator=(Mesh& other);
			
		private:
			void _validate();
		
			std::vector<Vec2> mesh;
			
			bool valid = false;
	};
}

#endif