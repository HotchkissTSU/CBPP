#include "cbasset/entity/BaseEntity.h"

using cbpp::Vec2;
using std::string;

namespace cbasset{
	namespace ent{
		BaseEntity::BaseEntity(Vec2 pos){
			
			
			this->Sync(); //сделали гадость, рассказали об этом всему миру
		}
		
		BaseEntity::BaseEntity(){
			BaseEntity(Vec2());
		}
		
		void BaseEntity::Sync(){
			if(cbsv::NetStatus() == cbsv::CBSV_SERVER){ //сервер ведает клиентам
				
			}else{ //клиент ведает серверу
				
			}
		}
		
		void BaseEntity::Tick(){
			
		}
		
		void BaseEntity::Render(){
			
		}
		
		BaseEntity::~BaseEntity(){
			
		}
	}
}