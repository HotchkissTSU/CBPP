#ifndef CBDRAW_UI_BASICH
#define CBDRAW_UI_BASICH

#include "cbasset/texture.h"
#include "cbasset/font.h"

#include "cbdraw/ui/b_unit.h"
#include "cbpp/vec2.h"

namespace cbdraw{
	namespace UI{		
		extern "C" class TextLabel : public BaseUnit{
			public:
				TextLabel(){}
				TextLabel(BaseUnit* parent, cbpp::Vec2 pos, cbasset::Font* font, std::wstring text);
				
				virtual void Render(RenderSurface* rt);
				
				void SetFont(cbasset::Font* n_font){ l_font = n_font; }
				void SetPadding(int padx, int pady){ pad_x = padx; pad_y = pady; }
				
				void SetText(std::wstring n_text){ l_text = n_text; }
				std::wstring GetText(){ return l_text; }
				
			private:
				std::wstring l_text = L"txt";
				cbasset::Font* l_font;
				
				int pad_x,pad_y;
		};
		
		extern "C" class Button : public BaseUnit{
			public:
				Button(){}
				Button(BaseUnit* parent, cbpp::Vec2 pos, cbpp::Vec2 size);
				
				void SetFunction(void (*command));
				
				virtual void Render(RenderSurface* rt);
				
			private:
				void (*command);
				TextLabel* lb;
		};
		
		extern "C" class Image : public BaseUnit{
			public:
				Image(){}
				Image(BaseUnit* parent, cbasset::TextureCollection* tex_coll, std::string tex_index, cbpp::Vec2 pos, cbpp::Vec2 size, float angle);
				
				virtual void Render(RenderSurface* rt);
				
			private:
				std::string texture_index = "missing";
				cbasset::TextureCollection* tex_collection;
				cbpp::Vec2 size;
		};
	}
}

#endif