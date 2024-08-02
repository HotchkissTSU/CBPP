#include "cbpp/mesh.h"
#include "cbpp/misc.h"

#include "cbpp/cb_alloc.h"

#include <iostream>

namespace cbpp{ //Mesh
	bool MeshValidate(Mesh* msh){
		if(msh == nullptr){ return false; }
		
		return msh->IsValid();
	}
	
	Mesh::Mesh(Mesh& other){
		if(other.IsValid()){
			Set(&other);
		}
	}

	Mesh::Mesh(){
		Mesh(3);
	}
	
	Mesh::Mesh(uint32_t len){
		Allocate( std::max((int)len, 3) );
	}
	
	Mesh::Mesh(Vec2* varr, uint32_t len){
		if(varr != nullptr && len >= 3){
			Allocate(len);
			
			for(uint32_t i = 0; i < len; i++){
				vecarr[i] = varr[i];
			}
		}
	}
	
	Mesh::Mesh(const std::vector<Vec2>& vvec){
		if(vvec.size() >= 3){
			Allocate(vvec.size());
			
			for(uint32_t i = 0; i < vvec.size(); i++){
				vecarr[i] = vvec[i];
			}
		}
	}
	
	void Mesh::Set(Mesh* src){
		if(!MeshValidate(src)){ return; }
		
		Free();
		Allocate(src->Size());
		
		for(uint32_t i = 0; i < src->Size(); i++){
			At(i) = src->At(i);
		}
	}
	
	void Mesh::Set(Vec2* varr, uint32_t vlen){
		if(varr == nullptr){ return; }
		
		Free();
		Allocate(vlen);
		
		for(uint32_t i = 0; i < vlen; i++){
			At(i) = varr[i];
		}
	}
	
	bool Mesh::IsValid(){ return allocated && (arrlen >= 3); }
	uint32_t Mesh::Size(){ return arrlen; }
	
	Vec2& Mesh::At(uint32_t vid){
		if(allocated){
			return vecarr[vid % arrlen]; //циклическая индексация
		}else{
			return nullvec;
		}
	}
	
	Vec2& Mesh::operator[](uint32_t vid){
		return At(vid);
	}
	
	void Mesh::AdjustVertex(Vec2 vtx){
		if(allocated){			
			uint32_t oldlen = arrlen;
			
			Vec2* buff = cbpp::Allocate<Vec2>(oldlen+1);
			
			for(uint32_t i = 0; i < arrlen+1; i++){
				buff[i] = vecarr[i];
			}
			
			delete[] vecarr;
			vecarr = cbpp::Allocate<Vec2>(oldlen+1);
			arrlen = oldlen+1;
			
			for(uint32_t i = 0; i < arrlen; i++){
				vecarr[i] = buff[i];
			}
			
			vecarr[arrlen-1] = vtx;
			
			delete[] buff;
		}
	}
	
	void Mesh::PopVertex(uint32_t vid){
		if(allocated && vid < arrlen){
			uint32_t oldlen = arrlen;
			
			Vec2* buff = cbpp::Allocate<Vec2>(oldlen-1);
			
			uint32_t k = 0;
			for(uint32_t i = 0; i < oldlen-1; i++){
				if(i != vid){
					buff[k] = vecarr[i];
					k++;
				}
			}
			
			delete[] vecarr;
			vecarr = cbpp::Allocate<Vec2>(oldlen-1);
			arrlen = oldlen - 1;
			
			for(uint32_t i = 0; i < arrlen; i++){
				vecarr[i] = buff[i];
			}
			
			delete[] buff;
		}
	}
	
	void Mesh::RotateByCenter(float ang){
		if(allocated){
			Vec2 X = Vec2(1,0).GetRotated(ang);
			Vec2 Y = Vec2(0,1).GetRotated(ang); //вместо того, чтобы отдельно вертеть каждую точку в меше,
												//делаем немного векторного волшебства
			for(uint32_t i = 0; i < arrlen; i++){
				vecarr[i] = X*vecarr[i].x + Y*vecarr[i].y;
			}
		}
	}
	
	Mesh Mesh::GetRotatedByCenter(float ang){
		Mesh out(arrlen);
		
		out.Set(vecarr, arrlen);
		out.RotateByCenter(ang);
		
		return out;
	}
	
	void Mesh::RotateByOffset(float ang, Vec2 offset){
		if(allocated){
			Vec2 X = Vec2(1,0).GetRotated(ang);
			Vec2 Y = Vec2(0,1).GetRotated(ang);
			
			for(uint32_t i = 0; i < arrlen; i++){
				Vec2 pt = vecarr[i] - offset;
				vecarr[i] = X*pt.x + Y*pt.y + offset;
			}
		}
	}
	
	Mesh Mesh::GetRotatedByOffset(float ang, Vec2 offset){
		Mesh out;
		out.Set(vecarr, arrlen);
		
		out.RotateByOffset(ang, offset);
		
		return out;
	}
	
	void Mesh::InsertVertex(uint32_t index, Vec2 vtx){
		Vec2* newarr = cbpp::Allocate<Vec2>(arrlen+1);
		
		index = index % (arrlen+1);
		newarr[index] = vtx;
		
		for(uint32_t i = 0; i < arrlen+1; i++){
			if(i < index){
				newarr[i] = At(i);
			}else if(i > index){
				newarr[i+1] = At(i);
			}else{
				newarr[i] = vtx;
			}
		}
		
		Set(newarr, arrlen+1);
	}
	
	void Mesh::MakeConvex(){
		if(!allocated){ return; }
		
		Vec2* convex = cbpp::Allocate<Vec2>(arrlen);
		uint32_t convex_len = 0;
		
		uint32_t left_pt = 0;
		float min_x = vecarr[0].x; //находим самую левую точку, она гарантированно попадает в выпуклую оболочку
		
		for(uint32_t i = 0; i < arrlen; i++){
			if(vecarr[i].x < min_x){
				min_x = vecarr[i].x;
				left_pt = i;
			}
		}
		
		Vec2 c = vecarr[0];
		vecarr[0] = vecarr[left_pt]; //суём левейшую точку в начало массива
		vecarr[left_pt] = c;
		
		uint32_t current = 0;
		
		while(1){
			convex[convex_len] = vecarr[current];
			convex_len++;
			
			uint32_t vmul_min_point = -1;
			
			for(uint32_t i = 0; i < arrlen; i++){				
				if( (i != current) && (vmul_min_point == -1 || (vecarr[i] - vecarr[current]).VectorMul(vecarr[vmul_min_point] - vecarr[current]) < 0)){
					vmul_min_point = i;
				}
			}
			
			if(vmul_min_point == 0){
				break;
			}
			
			current = vmul_min_point;
		}
		
		Set(convex, convex_len);
		
		delete[] convex;
	}
	
	Mesh Mesh::GetConvex(){
		Mesh out;
		out.Set(vecarr, arrlen);
		out.MakeConvex();
		
		return out;
	}
	
	void Triangulate() {
	}
	
	bool Mesh::CollidePoint(Vec2 v){
		return false;
	}
	
	void Mesh::Allocate(uint32_t len){
		if(allocated){
			Free();
		}
		
		vecarr = cbpp::Allocate<Vec2>(len);
		arrlen = len;
		
		allocated = (vecarr != nullptr);
	}
	
	void Mesh::Free(){
		if(allocated){
			delete[] vecarr;
			
			vecarr = nullptr;
			allocated = false;
			arrlen = 0;
		}
	}
	
	Mesh::~Mesh(){
		Free();
	}
}