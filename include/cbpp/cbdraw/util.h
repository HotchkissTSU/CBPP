#ifndef CBDRAW_UTIL
#define CBDRAW_UTIL

#include "SDL2/SDL.h"

#include "cbpp/cbpp.h"

#include <cstdint>
#include <map>
#include <string>

namespace cbdraw{	
	extern "C" struct Color{
		Color(){}
		Color(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca){ r = cr; g = cg; b = cb; a = ca; }
		Color(uint8_t cr, uint8_t cg, uint8_t cb){ Color(cr,cg,cb,255); }
		uint8_t r,g,b,a;
	};
	
	extern "C" struct ColorPalette{
		ColorPalette(){}
		ColorPalette(std::map<std::string, Color> n_palette){			
			palette = n_palette;
		}
		
		Color& operator[](std::string key){
			return palette[key];
		}
		
		std::map<std::string, Color> palette;
	};
}

#endif