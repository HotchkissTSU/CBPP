#include "cbasset/texture.h"
#include <iostream>

namespace cbasset{
	TextureCollection::TextureCollection(){
		LoadImage("missing", "assets/missing.png");
	}
	
	TextureCollection::~TextureCollection(){
		for(auto iter = tex.begin(); iter != tex.end(); ++iter){
			if(iter->second != NULL){
				SDL_DestroyTexture(iter->second);
				tex[iter->first] = NULL;
			}
		}
	}
	
	void TextureCollection::LoadImage(std::string index, std::string path){
		if(tex.count(index) != 0){
			if(tex[index] != nullptr){
				SDL_DestroyTexture(tex[index]);
			}
		}

		SDL_Surface* img = IMG_Load(path.c_str());
		if(img != NULL){
			SDL_Texture* img_tex = SDL_CreateTextureFromSurface(cbutil::GetRender(), img);
			
			tex[index] = img_tex;
			
			SDL_FreeSurface(img);
		}
	}
	
	SDL_Texture* TextureCollection::GetImage(std::string index){
		if(tex.count(index) != 0){
			return tex[index];
		}else{
			return tex["missing"];
		}
	}
	
	TextureCollectionFont::TextureCollectionFont(){
		LoadImage(0, "assets/missing.png");
	}
	
	void TextureCollectionFont::LoadImage(uint16_t index, std::string path){
		if(tex.count(index) != 0){
			if(tex[index] != nullptr){
				SDL_FreeSurface(tex[index]);
			}
		}

		SDL_Surface* img = IMG_Load(path.c_str());
		
		if(img != NULL){
			tex[index] = img;
		}
	}
	
	SDL_Surface* TextureCollectionFont::GetImage(uint16_t index){
		if(tex.count(index) != 0){
			return tex[index];
		}else{
			return tex[0];
		}
	}
}