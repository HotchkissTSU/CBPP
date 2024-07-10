/*
Наборы хуков (состояния) и система переключения между ними
*/

#ifndef CBPP_EP_STATEREG_H
#define CBPP_EP_STATEREG_H

#include "cbpp/ttype/segarr.h"

#include <cstdint>
#include <cstring>

#define CB_STATEINDEX_MAX (uint32_t)(-1)
#define event_t int

namespace cbpp {
	static uint32_t current_gs_id = CB_STATEINDEX_MAX;
	
	struct GameState {
		char* StateName = nullptr;
		
		bool (*Tick)( void ) = nullptr; //итерация основного цикла движка
		
		bool (*ChangeTo)( uint32_t ) = nullptr; //аргумент передал нам управление
		bool (*ChangeFrom)( uint32_t ) = nullptr; //передаём управление аргументу
		
		bool (*Event)( event_t& ) = nullptr; //получено событие
	};
	
	static SegArray<GameState*> statereg(256, 64);
	
	SegArray<GameState*>& GetStateRegList();
	
	uint32_t RegisterGameState(GameState* gs_ptr);
	uint32_t LookupGameState(const char* name);
	
	void SetGameState(uint32_t id);
	uint32_t GetCurrentGameState();
	
	GameState* CreateGameState( const char* name, bool (*Tick)( void ), bool (*Event)( event_t& ), bool (*ChangeTo)( uint32_t ) = nullptr, bool (*ChangeFrom)( uint32_t ) = nullptr );

	void CleanupGameStates();
}

#endif