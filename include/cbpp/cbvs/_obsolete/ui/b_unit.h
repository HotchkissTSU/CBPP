#ifndef CBDRAW_UI_BASEH
#define CBDRAW_UI_BASEH

#include "cbpp/vec2.h"
#include "SDL2/SDL.h"
#include "cbdraw/gpu_surf.h"
#include "cbdraw/util.h"

#include "cbasset/font.h"

#include <string>
#include <map>

#define CB_UI_MAX_TREE_LENGTH 128

namespace cbdraw{
	namespace UI{
		extern "C" class BaseUnit{
			public:
				BaseUnit(){}
				BaseUnit(BaseUnit* parent, cbpp::Vec2 pos);
				
				void _setclass(std::string cl);
				
				void SetParent(BaseUnit* parent);
				BaseUnit* GetParent();
				
				void Deparent();
				
				void SetPos(cbpp::Vec2 npos);
				
				cbpp::Vec2 GetPos(int _count = 0);
				cbpp::Vec2 GetRawPos();
				
				void _add_child(BaseUnit* child);
				void _remove_child(BaseUnit* child);
				void _basetick();
				
				virtual void Tick(); //все рассчеты ведутся здесь
				virtual void Render(RenderSurface* rt); //а все рисование - тут
				
				virtual void OnEvent(SDL_Event ev);
				
				std::string GetClass();
				
				void SendEvent(SDL_Event ev);
				
				virtual cbpp::Vec2 GetSize(){ return w_size; }
				virtual void SetSize(cbpp::Vec2 sz){ w_size = sz; }
				
				void SetAngle(float ang){ u_angle = ang; }
				float GetAngle(){ return u_angle; }
				
				void SetShadeOffset(uint8_t noffset){ shade_offset = noffset; }
				uint8_t GetShadeOffset(){ return shade_offset; }
				
				void SetBorderWidth(uint8_t nwidth){ border_width = nwidth; }
				uint8_t GetBorderWidth(){ return border_width; }
				
				void SetFont(cbasset::Font* n_font){ if(n_font != nullptr){ u_font = n_font; } }
				cbasset::Font* GetFont(){ return u_font; }
				
				~BaseUnit();
				
				ColorPalette Palette;
				bool _internal = false; //внутренние виджеты не обрабатываются автоматически и сидят на попечении хозяев
										//например, кнопки у виджета окна
			private:
				BaseUnit* parent = nullptr;
				std::vector<BaseUnit*> children;
				cbpp::Vec2 pos; //позиция виджета относительно его хозяина
				
				SDL_Event last_event;
				
				uint8_t shade_offset, border_width;
				float u_angle;
				
				cbpp::Vec2 w_size;
				
				cbasset::Font* u_font;
				
				std::string classname; //каждый класс виджета должен определять это название в своем конструкторе (_setclass)
		};
	}
}

#endif