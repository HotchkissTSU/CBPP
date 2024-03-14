#ifndef CBASSET_TEXTUREH
#define CBASSET_TEXTUREH

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "cbdraw/window.h"

#include <map>
#include <string>

namespace cbasset{
	extern "C" struct TextureCollection{
		TextureCollection();
		
		virtual void LoadImage(std::string index, std::string path);
		
		virtual SDL_Texture* GetImage(std::string index);
		
		~TextureCollection();
		
		std::map<std::string, SDL_Texture*> tex;
	};
	
	extern "C" struct TextureCollectionFont : public TextureCollection{ //шаблон отказался работать, делаем глупость
		TextureCollectionFont();
		
		SDL_Surface* GetImage(uint16_t index);
		void LoadImage(uint16_t index, std::string path);
		
		std::map<uint16_t, SDL_Surface*> tex;
	};
}

#endif