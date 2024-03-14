#include "cbdraw/ui/ui_window.h"

#include <iostream>

using namespace cbpp;

namespace cbdraw{
	namespace UI{
		Window::Window(BaseUnit* parent, Vec2 pos, Vec2 size, cbasset::Font* fnt, std::wstring title) : BaseUnit(parent, pos){
			_setclass("window");
			
			ui_title_unit = new TextLabel(this, Vec2(2), NULL, title);
			
			if(ui_title_unit != nullptr){
				ui_title_unit->SetSize(Vec2(0.8f));
			}
			
			title_font = new cbasset::Font(L"consolas");
			
			SetSize(size);
			SetFont(fnt);
			SetTitle(title);
		}
		
		void Window::Render(GPU_Surface* rt){	
			
			Vec2 pos_real = GetPos();
			
			if(GetShadeOffset() > 0){ rt->DrawFilledRect(Palette["shade"], pos_real+GetShadeOffset(), pos_real+GetSize()+GetShadeOffset()); }
			
			rt->DrawFilledRect(Palette["bg"], pos_real, pos_real + GetSize());
			
			if(GetBorderWidth() > 0){ rt->DrawRect(Palette["fg"], pos_real, pos_real + GetSize(), GetBorderWidth()); }
			
			rt->DrawFilledRect(Palette["menu"], pos_real, pos_real + Vec2((int)GetSize().x, 20));
			
			if(ui_title_unit != nullptr){
				ui_title_unit->Palette["fg"] = this->Palette["title"];
				ui_title_unit->Render(rt);
			}
		}
		
		void Window::OnEvent(SDL_Event ev){
			int mx,my;
			SDL_GetMouseState(&mx, &my);
			
			Vec2 wpos = GetPos();
			
			switch (ev.type){
				case SDL_MOUSEBUTTONDOWN:
					if(ev.button.button == SDL_BUTTON_LEFT && VecInBox(Vec2(mx,my), wpos, wpos + Vec2((int)GetSize().x, 20))){
						w_drag = true;
						w_drag_relpos = wpos - Vec2(mx,my);
					}
					break;
					
				case SDL_MOUSEBUTTONUP:
					if(ev.button.button == SDL_BUTTON_LEFT && w_drag){
						w_drag = false;
					}
					break;
			}
		}
		
		void Window::Tick(){			
			int mx,my;
			SDL_GetMouseState(&mx, &my);
			
			if(w_drag){
				if(GetParent() != nullptr){					
					Vec2 new_pos = Vec2(mx,my) + w_drag_relpos - GetParent()->GetPos();
					SetPos(new_pos);
				}else{
					SetPos(Vec2(mx,my) + w_drag_relpos);
				}
			}
		}
	}
}