#ifndef CBPP_BITWORK_H
#define CBPP_BITWORK_H

#include <cstdint>

namespace cbpp {
	extern "C" template<typename INT_T> bool TestBitmask(INT_T word, INT_T bmask){
		return (word & bmask) == bmask;
	}
	
	extern "C" template<typename INT_T> uint8_t GetBit(INT_T word, uint16_t index){
		return (word & (1 << index)) >> index;
	}
	
	extern "C" template<typename INT_T> void SetBit(INT_T& word, uint16_t index, uint8_t bit){
		INT_T mask = 1 << index;
		if(bit == 0){
			mask = ~mask;
		}
		
		word = word & mask;
	}
}

#endif