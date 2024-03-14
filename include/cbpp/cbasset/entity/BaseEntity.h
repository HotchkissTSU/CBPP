#ifndef CBASSET_ENT_BASE
#define CBASSET_ENT_BASE

#include <map>
#include <string>

#include "cbsv/sh_netstate.h"
#include "cbpp/btable.h"
#include "cbpp/vec2.h"

namespace cbasset{
	namespace ent{		
		enum ENT_NETCLASS : uint8_t{
			ENT_SVSIDE,
			ENT_CLSIDE,
			ENT_SHARED
		};
		
		enum ENT_SYNCCLASS : uint8_t{
			SYNC_SINGLE, //энтити синхронизируется один раз после создания
			SYNC_ALWAYS, //энтити синхронизируется постоянно
			SYNC_PVS,    //энтити синхронизируется с клиентом, только если оно попадает в его PVS.
						 //На клиенте работает как SYNC_ALWAYS
			
			SYNC_MANUAL  //энтити синхронизируется только по вручную поданной команде
		};
		
		extern "C++" class BaseEntity{
			public:
				BaseEntity();
				BaseEntity(cbpp::Vec2 pos);
				
				virtual void Tick(); //обновление энтити
				
				virtual void Render();  //рендеринг бывает только на клиенте
				
				virtual void Sync(); //попытка синхронизации клиента и сервера
				
				std::string GetClass(){ return ent_class; }
				ENT_NETCLASS NetClass(){ return ent_netclass; }
				ENT_SYNCCLASS SyncMode(){ return ent_syncclass; }
				
				template<typename T> void WriteNetVar(std::string vname, T var, cbpp::BTABLE_TYPES vtype){
					
				}
				
				template<typename T> T    ReadNetVar(std::string vname){
					
				}
				
				virtual ~BaseEntity();
				
			private:			
				std::string ent_class = "cbpp_baseent";
				ENT_NETCLASS ent_netclass = ENT_SHARED;
				ENT_SYNCCLASS ent_syncclass = SYNC_PVS;
				
				cbpp::Vec2 ent_position;
				
				cbpp::ByteTable ent_netvars;
		};
	}
}

#endif