#include "cbdraw/ui/b_unit.h"

#include <iostream>

using namespace cbpp;

namespace cbdraw{
	static std::map<cbdraw::UI::BaseUnit*, bool> WindowContainer;
	
	namespace UI{
		BaseUnit::BaseUnit(BaseUnit* parent, Vec2 pos){
			classname = "base";
			children.resize(0);
			
			SetParent(parent);
			SetPos(pos);
		}
		
		void BaseUnit::_setclass(std::string cl){
			classname = cl;
		}
		
		void BaseUnit::Deparent(){
			if(parent != NULL){ parent->_remove_child(this); parent = nullptr; }
		}
		
		void BaseUnit::SetParent(BaseUnit* nparent){
			bool loop = false;

			for(std::size_t i = 0; i < children.size(); i++){
				if(children[i] == nparent){ //не допускаем перекрестного парента, который вызовет вечную рекурсию и вылет, но
					loop = true;			//при этом все еще можно сделать более длинное кольцо из парентов
				}							//существует лимит на длину рекурсии, но это кольцо похоронит fps
			}

			if(loop){ parent = nullptr; }
			else{
				Deparent();
				parent = nparent;
				if(nparent != nullptr){
					nparent->_add_child(this);
				}
			}
		}
		
		BaseUnit* BaseUnit::GetParent(){
			return parent;
		}
		
		void BaseUnit::_add_child(BaseUnit* child){
			if(child != nullptr){
				bool has = false;
				
				for(std::size_t i = 0; i < children.size(); i++){ //не добавляем один и тот же объект два раза
					if(children[i] == child){
						has = true;
						break;
					}
				}
				
				if(!has){ children.push_back(child); }
			}
		}
		
		void BaseUnit::_remove_child(BaseUnit* child){
			if(child != nullptr){
				for(std::size_t i = 0; i < children.size(); i++){
					if(child == children[i]){
						
						std::vector<BaseUnit*>::iterator it = children.begin();
						std::advance(it, i);
						children.erase(it);
						
						break;
					}
				}
			}
		}
		
		void BaseUnit::SetPos(Vec2 npos){
			pos = npos;
		}
		
		Vec2 BaseUnit::GetPos(int _count){
			if(parent != nullptr && _count <= CB_UI_MAX_TREE_LENGTH){
				return parent->GetPos(_count+1) + pos;
			}else{
				return pos;
			}
		}
		
		void BaseUnit::OnEvent(SDL_Event ev){}
		
		Vec2 BaseUnit::GetRawPos(){ return pos; }
		
		std::string BaseUnit::GetClass(){ return classname; }
		
		void BaseUnit::SendEvent(SDL_Event ev){
			last_event = ev;
			
			for(std::size_t i = 0; i < children.size(); i++){
				if(children[i] != nullptr){ children[i]->SendEvent(ev); } //делимся событием со всеми дочерними объектами
			}
			
			this->OnEvent(ev);
		}
		
		BaseUnit::~BaseUnit(){
			SetParent(NULL);
			
			for(std::size_t i = 0; i < children.size(); i++){ //выпиливаем объект из дерева, чтобы не делать "висяков"
				if(children[i] != nullptr){ children[i]->SetParent(NULL); }
			}
		}
		
		void BaseUnit::Render(RenderSurface* rt){
			rt->DrawCircle(Color(255,0,0,255), GetPos(), 15, 2);
		}
		
		void BaseUnit::Tick(){}
	}
}