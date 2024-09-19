#include "cbpp/statereg.h"

namespace cbpp {
	uint32_t RegisterGameState(GameState* ptr){
		uint32_t index = CB_STATEINDEX_MAX;
		for(uint32_t i = 0; i < statereg.Length(); i++){ //ищем дырку посередине массива
			if(statereg[i] == nullptr){
				index = i;
				break;
			}
		}
		
		if(index == CB_STATEINDEX_MAX){ //нет дырки посередине, пихаем в конец
			statereg.PushBack(ptr);
			index = statereg.Length() - 1;
		}else{
			statereg[index] = ptr;
		}
		
		return index;
	}
	
	uint32_t LookupGameState(const char* name){
		uint32_t index = CB_STATEINDEX_MAX;
		
		for(uint32_t i = 0; i < statereg.Length(); i++){
			GameState* gs = statereg[i];
			if(gs != nullptr){
				if( strcmp(name, gs->StateName) == 0 ){
					index = i;
					break;
				}
			}
		}
		
		return index;
	}
	
	void SetGameState(uint32_t id){
		if(id != CB_STATEINDEX_MAX){

			if(current_gs_id != CB_STATEINDEX_MAX){
				if(statereg[current_gs_id] != nullptr){
					if(statereg[current_gs_id]->ChangeFrom != nullptr){ statereg[current_gs_id]->ChangeFrom(id); }
				}
			}
			
			uint32_t old_id = current_gs_id;
			current_gs_id = id;
			
			if(statereg[id] != nullptr){
				if(statereg[id]->ChangeTo != nullptr){ statereg[id]->ChangeTo(old_id); }
			}
		}else{
			current_gs_id = CB_STATEINDEX_MAX;
		}
	}
	
	uint32_t GetCurrentGameState(){
		return current_gs_id;
	}
	
	SegArray<GameState*>& GetStateRegList(){
		return statereg;
	}

	GameState* CreateGameState( const char* name, bool (*Tick)( void ), bool (*OnEvent)( Event& ), bool (*ChangeTo)( uint32_t ), bool (*ChangeFrom)( uint32_t ) ){
		GameState* out = new GameState;
		memset(out, 0, sizeof(GameState));
		
		out->StateName = new char[strlen(name)+1];
		memset(out->StateName, 0, strlen(name)+1);
		strcpy(out->StateName, name);
		
		out->Tick = Tick;
		out->OnEvent = OnEvent;
		out->ChangeTo = ChangeTo;
		out->ChangeFrom = ChangeFrom;
		
		return out;
	}
	
	void CleanupGameStates(){
		for(uint32_t i = 0; i < statereg.Length(); i++){
			GameState* gs = statereg[i];
			
			if(gs == nullptr){ continue; }
			
			delete[] gs->StateName;
			delete[] gs;
			statereg[i] = nullptr;
		}
	}
}