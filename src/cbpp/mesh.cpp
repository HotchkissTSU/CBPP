#include "cbpp/mesh.h"

#include "cbpp/math.h"
#include <stdexcept>

namespace cbpp {
	//MeshVertex
	MeshVertex::MeshVertex(Vec2& other) {
		x = other.x;
		y = other.y;
		u = 0.0f;
		v = 0.0f;
	}
	
	Vec2 MeshVertex::GetUV() {
		return Vec2(u, v);
	}
	
	void MeshVertex::SetUV(float _u, float _v) {
		u = _u;
		v = _v;
	}
	
	void MeshVertex::SetUV(Vec2 uv) {
		u = uv.x;
		v = uv.y;
	}
	
	//Mesh
	Mesh::Mesh(){}
	
	Mesh::Mesh(const MeshVertex* varr, uint32_t vlen, bool sorted) {
		vecarr = new MeshVertex[vlen];
		memcpy(vecarr, varr, vlen);
		arrlen = vlen;
		allocated = true;
	}
	
	Mesh::Mesh(SegArray<MeshVertex>& other, bool sorted) {
		vecarr = new MeshVertex[other.Length()];
		
		for(uint32_t i = 0; i < other.Length(); i++) {
			vecarr[i] = other.At(i);
		}
		
		arrlen = other.Length();
		allocated = true;
	}
	
	void Mesh::compute_mid() {
		if(!allocated) {
			throw std::runtime_error("Mesh not allocated");
		}
		
		midpos = Vec2(0);
		for(uint32_t i = 0; i < arrlen; i++) {
			MeshVertex vtx_info = vecarr[i];
			midpos = midpos + Vec2(vtx_info.x, vtx_info.y);
		}
		
		midpos = midpos / (int)arrlen;
	}
	
	bool Mesh::compare_vtx(MeshVertex& a, MeshVertex& b) {
		Vec2 av = Vec2(a.x, a.y), bv = Vec2(b.x, b.y);
		Vec2 v1 = av - midpos, v2 = bv - midpos;
		
		float vmul = v1.VectorMul(v2);
		
		if(fEqual(vmul, 0.0f)) {
			//meh
		}
		
		return v1.VectorMul(v2) < 0;
	}
	
	void Mesh::sort_mesh() {
		if(!allocated) {
			throw std::runtime_error("Mesh not allocated");
		}
		
		MeshVertex temp;
		bool exit_loop = false;
		 
		while(!exit_loop) {
			exit_loop = true;
			for(uint32_t i = 0; i < (arrlen - 1); i++) {
				if( compare_vtx(vecarr[i], vecarr[i + 1]) ) {
					temp = vecarr[i];
					vecarr[i] = vecarr[i + 1];
					vecarr[i + 1] = temp;
					exit_loop = false;
				}
			}
		}
	}
}