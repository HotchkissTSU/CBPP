#include "cbdraw/ui/ui_basic.h"

#include <iostream>

using namespace cbpp;

namespace cbdraw{ //Button
	namespace UI{
		Button::Button(BaseUnit* parent, cbpp::Vec2 pos, cbpp::Vec2 size) : BaseUnit(parent, pos) {
			_setclass("button_text");
			
			//lb = new TextLabel(this, Vec2(), );
		
			SetSize(size);
		}
		
		void Button::SetFunction(void (*ncommand)){
			command = ncommand;
		}
		
		void Button::Render(GPU_Surface* rt){
			uint8_t sho = GetShadeOffset();
			Vec2 upos = GetPos();
			Vec2 usz = GetSize();
			
			if(sho > 0){ rt->DrawFilledRect(Palette["shade"], upos + sho, upos + usz + sho); }
			
			rt->DrawFilledRect(Palette["bg"], upos, upos + usz);
			rt->DrawRect(Palette["fg"], upos, upos + usz, GetBorderWidth());
		}
	}
	
	namespace UI{ //text label
		TextLabel::TextLabel(BaseUnit* parent, Vec2 pos, cbasset::Font* font, std::wstring text) : BaseUnit(parent, pos){
			_setclass("label");
			
			pad_x = 0;
			pad_y = 0;
			
			
			if(font != NULL){
				SetFont(font);
			}else{
				l_font = new cbasset::Font(L"consolas");
			}
			SetText(text);
		}
		
		void TextLabel::Render(GPU_Surface* rt){
			rt->DrawText(Palette["fg"], l_font, l_text, GetPos(), GetSize(), pad_x, pad_y);
		}
	}
	
	namespace UI{ //image
		Image::Image(BaseUnit* parent, cbasset::TextureCollection* tex_coll, std::string tex_index, cbpp::Vec2 pos, cbpp::Vec2 size, float angle) : BaseUnit(parent, pos){
			_setclass("image");
			
			SetAngle(angle);
			SetSize(size);
			
			tex_collection = tex_coll;
			texture_index = tex_index;
		}
		
		void Image::Render(GPU_Surface* rt){
			rt->DrawImage(tex_collection->GetImage(texture_index), GetPos(), GetSize(), GetAngle());
		}
	}
}