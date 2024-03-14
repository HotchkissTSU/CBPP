#include "cbpp/mesh.h"

using namespace cbpp;

namespace cbpp{
	Mesh::Mesh(){}
	
	Mesh::Mesh(std::size_t size){
		if(size <= 0){size = 1;}
		mesh.resize(size);
	}
	
	Mesh::Mesh(std::vector<Vec2> verts){
		mesh.resize(verts.size());
		for(int i = 0; i < verts.size(); i++){
			mesh.push_back(verts[i]);
		}
		
		_validate();
	}
	
	void Mesh::Set(std::size_t index, Vec2 p){
		if(index >= 0 && index < mesh.size()){
			mesh[index] = p;
		}
		
		if(index >= mesh.size()){
			AdjustVertex(p);
		}
	}
	
	void Mesh::SetMesh(std::vector<Vec2> verts){
		this->SetSize(verts.size());
		for(std::size_t i = 0; i < verts.size(); i++){
			mesh[i] = verts[i];
		}
		
		_validate();
	}
	
	void Mesh::SetMesh(Mesh* msh){
		this->SetSize(msh->Size());
		for(std::size_t i = 0; i < msh->Size(); i++){
			mesh[i] = msh->At(i);
		}
		
		_validate();
	}
	
	std::size_t Mesh::Size(){
		return mesh.size();
	}
	
	void Mesh::Clear(){
		mesh.clear();
		valid = false;
	}
	
	void Mesh::SetSize(std::size_t sz){
		if(sz <= 0){ sz = 1; }
		mesh.resize(sz);
		
		_validate();
	}
	
	void Mesh::AdjustVertex(Vec2 v){
		mesh.push_back(v);
		
		_validate();
	}
	
	void Mesh::PopVertex(std::size_t index){
		mesh.erase(mesh.begin()+index);
		
		_validate();
	}
	
	Vec2& Mesh::operator[](std::size_t index){
		return mesh[index];
	}
	
	void Mesh::GetRotatedByCenter(Mesh* target, float ang){
		target->SetSize(mesh.size());
		for(std::size_t i = 0; i < mesh.size(); i++){
			target->Set(i, mesh[i].GetRotated(ang));
		}
	}
	
	void Mesh::RotateByCenter(float ang){
		for(std::size_t i = 0; i < mesh.size(); i++){
			mesh[i].Rotate(ang);
		}
	}
	
	void Mesh::RotateByOffset(float ang, Vec2 p){
		for(std::size_t i = 0; i < mesh.size(); i++){
			mesh[i] = (mesh[i]-p).GetRotated(ang);
		}
	}
	
	void Mesh::GetRotatedByOffset(Mesh* target, float ang, Vec2 p){
		target->Clear();
		for(std::size_t i = 0; i < mesh.size(); i++){
			target->AdjustVertex((mesh[i]-p).GetRotated(ang));
		}
	}
	
	void Mesh::Scale(float scal){
		for(std::size_t i = 0; i < mesh.size(); i++){
			mesh[i] = mesh[i]*scal;
		}
	}
	
	void Mesh::GetScaled(Mesh* target, float scal){
		target->Clear();
		for(std::size_t i = 0; i < mesh.size(); i++){
			target->AdjustVertex(mesh[i]*scal);
		}
	}
	
	void Mesh::Clip(Vec2 pos1, Vec2 pos2, int dir){	
		std::vector<Vec2> mesh_tmp;
		for(std::size_t i = 0; i < mesh.size(); i++){
			Vec2 p1,p2;
			
			if(i == mesh.size() - 1){
				p1 = mesh[mesh.size()-1];
				p2 = mesh[0];
			}else{
				p1 = mesh[i];
				p2 = mesh[i+1];
			}
			
			auto result = GetLineIntersection(pos1, pos2, p1, p2);

			if(result.intersect){
				Vec2 ps = result.intersect_pos;
				mesh_tmp.push_back(p1);
				mesh_tmp.push_back(ps);
				mesh_tmp.push_back(p2);
			}else{
				mesh_tmp.push_back(p1);
				mesh_tmp.push_back(p2);
			}
		}
		
		std::vector<Vec2> mesh_new;
		for(std::size_t i = 0; i < mesh_tmp.size(); i++){
			Vec2 pos = mesh_tmp[i];
			
			float D = GetLinePointOrientation(pos1, pos2, pos);
			
			if( (D < 0 && dir < 0) || (D > 0 && dir > 0) || std::abs(D) < 0.01f ){
				mesh_new.push_back(pos);
			}
		}
		
		SetMesh(mesh_new);
	}
	
	void Mesh::GetClipped(Mesh* target, Vec2 pos1, Vec2 pos2, int dir){
		target->Clip(pos1, pos2, dir);
	}
	
	bool Mesh::CollidePoint(Vec2 v){
		uint16_t counter = 0;
		for(std::size_t i = 0; i < mesh.size(); i++){
			Vec2 p1,p2;
			
			if(i == mesh.size() - 1){
				p1 = mesh[mesh.size()-1];
				p2 = mesh[0];
			}else{
				p1 = mesh[i];
				p2 = mesh[i+1];
			}
			
			auto result = GetLineIntersection(Vec2(), Vec2(0.0f, 0.93f), p1, p2);
			counter += (uint16_t)result.intersect;
		}
		
		return (counter%2) != 0;
	}
	
	std::vector<Vec2> Mesh::GetLineIntersections(Vec2 _p1, Vec2 _p2){
		std::vector<Vec2> out;
		
		for(std::size_t i = 0; i < mesh.size(); i++){
			Vec2 p1,p2;
			
			if(i == mesh.size() - 1){
				p1 = mesh[mesh.size()-1];
				p2 = mesh[0];
			}else{
				p1 = mesh[i];
				p2 = mesh[i+1];
			}
			
			auto result = GetLineIntersection(_p1, _p2, p1, p2);
			if(result.intersect){
				out.push_back(result.intersect_pos);
			}
		}
		
		return out;
	}
	
	Vec2& Mesh::At(std::size_t index){ return mesh[index]; }
	
	Mesh& Mesh::operator=(Mesh& other){
		this->SetSize(other.Size());
		for(std::size_t i = 0; i < other.Size(); i++){
			mesh[i] = other.At(i);
		}
		return *this;
	}
	
	void Mesh::_validate(){ valid = mesh.size() >= 3; }
}