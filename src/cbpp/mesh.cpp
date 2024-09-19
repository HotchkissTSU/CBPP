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
	
	Mesh::Mesh(const MeshVertex* varr, int64_t vlen, bool sorted) {
		vecarr.Set(varr, vlen);
		compute_mid();
		compute_left();
		
		if(!sorted) {
			sort_mesh();
		}
	}
	
	Mesh::Mesh(SegArray<MeshVertex>& other, bool sorted) {
		vecarr.Set(other);
		if(!sorted) { sort_mesh(); }
	}
	
	void Mesh::AdjustVertex(MeshVertex& vtx) {
		vecarr.PushBack(vtx);
		compute_left();
		compute_mid();
		sort_mesh();
	}
	
	MeshVertex& Mesh::At(int64_t index) {
		return vecarr.At(index % vecarr.Length());
	}
	
	int64_t Mesh::Length() {
		return vecarr.Length();
	}
	
	void Mesh::compute_mid() {		
		midpos = Vec2(0);
		for(uint32_t i = 0; i < vecarr.Length(); i++) {
			MeshVertex vtx_info = vecarr[i];
			midpos = midpos + Vec2(vtx_info.x, vtx_info.y);
		}
		
		midpos = midpos / (int)( vecarr.Length() );
	}
	
	void Mesh::compute_left() {
		if(vecarr.Length() == 0) {
			CbThrowError("Mesh is empty");
		}
		
		float mx = vecarr.At(0).x;
		float my = vecarr.At(0).y;
		
		for(int64_t i = 0; i < vecarr.Length(); i++) {
			MeshVertex vtx = vecarr.At(i);
			Vec2 vc(vtx.x, vtx.y);
			
			if(vc.x < mx) {
				leftpos = vc;
			}
		}
	}
	
	bool Mesh::compare_vtx(MeshVertex& a, MeshVertex& b) {
		Vec2 av = Vec2(a.x, a.y), bv = Vec2(b.x, b.y);
		Vec2 v1 = av - leftpos, v2 = bv - leftpos;
		
		float vmul = v1.VectorMul(v2);
		
		if(fEqual(vmul, 0.0f)) {
			//
		}
		
		return v1.VectorMul(v2) < 0;
	}
	
	void Mesh::sort_mesh() {		
		MeshVertex temp;
		bool exit_loop = false;
		 
		while(!exit_loop) {
			exit_loop = true;
			for(uint32_t i = 0; i < (vecarr.Length() - 1); i++) {
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