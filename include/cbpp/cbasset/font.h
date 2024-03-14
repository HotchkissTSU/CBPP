#ifndef CBASSET_FONTH
#define CBASSET_FONTH

#include "SDL2/SDL.h"
#include "SDL2/SDL2_ttf.h"
#include "json/json.h"
#include "cbasset/texture.h"

#include "cbdraw/window.h"
#include "cbpp/collection.h"

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <map>

namespace cbasset{
	extern "C" struct LetterData{
		LetterData(){}

		int ox,oy,x,y,w,h;
		uint16_t l_code;
		wchar_t letter;
	};

	extern "C" struct Font{
		Font(){}
		Font(std::string src_img, std::string json_data);
		
		~Font(){};
		
		TextureCollectionFont fontmap;
		std::map<uint16_t, LetterData> fontdata;
	};
	
	extern "C" struct FontCollection : public cbpp::BaseCollection<std::string, Font*>{
		FontCollection(){}
		
		void LoadFont(std::string name, std::string json_path);
	};
}

#endif