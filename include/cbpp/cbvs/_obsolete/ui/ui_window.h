#ifndef CBDRAW_UI_WINDOWH
#define CBDRAW_UI_WINDOWH

#include "cbdraw/ui/b_unit.h"
#include "cbpp/misc.h"
#include "cbdraw/ui/ui_basic.h"
#include "cbasset/font.h"

#include "cbdraw/util.h"

namespace cbdraw{
	namespace UI{
		extern "C" class Window : public BaseUnit {
			public:
				Window(){}
				Window(BaseUnit* parent, cbpp::Vec2 pos, cbpp::Vec2 size, cbasset::Font* fnt, std::wstring title);
				
				void SetTitle(std::wstring n_title){ w_title = n_title; }
				std::wstring GetTitle(){ return w_title; }
				
				virtual void Render(RenderSurface* rt);
				virtual void OnEvent(SDL_Event ev);
				virtual void Tick();
			
			private:
				bool w_active, w_drag;
				cbpp::Vec2 w_drag_relpos;
				
				std::wstring w_title;
				
				TextLabel* ui_title_unit = nullptr;
				cbasset::Font* title_font;
		};
	}
}

#endif