#ifndef CBDRAW_GPU_SURFH
#define CBDRAW_GPU_SURFH

#include <cstdint>
#include <string>

#include "SDL2/SDL.h"
#include "cbpp/cbpp.h"
#include "cbdraw/util.h"

#include "cbasset/font.h"

namespace cbdraw{
	extern "C" class RenderSurface{ //аналог, т.к. SDL_Surface рисует процессором, а мы очень хотим напрячь видяшку
		public:
			RenderSurface(){};
			RenderSurface(cbpp::Vec2 pos, cbpp::Vec2 res, SDL_Renderer* render);
			
			void SetPos(cbpp::Vec2 pos);
			cbpp::Vec2 GetPos();
			
			void SetResolution(cbpp::Vec2 res);
			cbpp::Vec2 GetResolution();
			
			void VSync(bool vsync);
			SDL_Renderer* GetRender();
			SDL_Texture* GetRT();
			
			void ApplyColor(Color clr);
			
			
			void DrawLine(Color clr, cbpp::Vec2 pos1, cbpp::Vec2 pos2, uint8_t width);
			void DrawCircle(Color clr, cbpp::Vec2 pos, uint16_t radius, uint8_t width);
			void DrawMesh(Color clr, cbpp::Vec2 pos, cbpp::Mesh* msh, uint8_t width);
			
			void DrawPolygon(Color clr, std::vector<cbpp::Vec2> verts);
			void DrawFilledPolygon(Color clr, std::vector<cbpp::Vec2> verts);
			
			void DrawRect(Color clr, cbpp::Vec2 p1, cbpp::Vec2 p2, uint8_t width);
			void DrawFilledRect(Color clr, cbpp::Vec2 p1, cbpp::Vec2 p2);
			
			void DrawImage(SDL_Texture* tex, cbpp::Vec2 pos, cbpp::Vec2 size, float roto);
			void DrawText(Color clr, cbasset::Font* font, std::wstring text, cbpp::Vec2 pos, cbpp::Vec2 size, int xpad, int ypad);
			
			void DrawVector(Color clr, cbpp::Vec2 pos, cbpp::Vec2 vector);
			
			
			void Render(cbpp::Vec2 scal);
			void RenderRotated(cbpp::Vec2 scal, cbpp::Vec2 rotopoint, float roto);
			
			void SetColorMod(Color clr);
			void SetBlendMode(SDL_BlendMode blend);
			
			void SetCirclePrecision(uint16_t prec);
			
			void Clear();
			
			~RenderSurface(){
				if(rnd_tar != nullptr){
					SDL_DestroyTexture(rnd_tar);
				}
			}
		
		private:
			uint16_t circ_precision = 32;
			
			Color clear_color = Color(0,0,0,255);
		
			SDL_Renderer* rd;
			SDL_Texture* rnd_tar;
			cbpp::Vec2 cam_pos, cam_res;
	};
	
	SDL_Surface* RenderText(cbasset::Font* font, std::wstring text, float scale = 1.0f, int padx = 5, int pady = 10, bool debug = false);
}

#endif