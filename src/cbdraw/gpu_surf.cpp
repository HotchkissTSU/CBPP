#include "cbdraw/gpu_surf.h"
#include <iostream>

#include "SDL2/SDL2_gfxPrimitives.h"

using namespace cbpp;

namespace cbdraw{	
	RenderSurface::RenderSurface(Vec2 pos, Vec2 res, SDL_Renderer* render){
		cam_pos = pos;
		cam_res = res;
		
		rd = render;
		rnd_tar = SDL_CreateTexture(rd, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, (int)res.x, (int)res.y);
		SDL_SetTextureBlendMode(rnd_tar, SDL_BLENDMODE_BLEND);
	}
	
	void RenderSurface::SetPos(Vec2 pos){ cam_pos = pos; }
	Vec2 RenderSurface::GetPos(){ return cam_pos; }
	
	void RenderSurface::SetResolution(Vec2 res){ cam_res = res; }
	Vec2 RenderSurface::GetResolution(){ return cam_res; }
	
	void RenderSurface::VSync(bool vsync){ if( rd != nullptr ){ SDL_RenderSetVSync(rd, (int)vsync); } }
	
	SDL_Renderer* RenderSurface::GetRender(){ return rd; }
	SDL_Texture* RenderSurface::GetRT(){ return rnd_tar; }
	
	void RenderSurface::Clear(){
		ApplyColor(clear_color);
		SDL_RenderClear(rd);
	}
	
	//--
	void RenderSurface::Render(Vec2 scale){
		SDL_SetRenderTarget(rd, rnd_tar);
		
		const SDL_Rect dst = {(int)cam_pos.x, (int)cam_pos.y, (int)scale.x, (int)scale.y};
		
		SDL_RenderCopy(rd, rnd_tar, NULL, &dst);
		
		SDL_SetRenderTarget(rd, NULL);
		SDL_RenderPresent(rd);
	}
	
	void RenderSurface::RenderRotated(Vec2 scale, Vec2 rotopos, float roto){
		SDL_RenderPresent(rd);
		
		Vec2 rotopos_local = rotopos - cam_pos;
		
		const SDL_Rect dst = {(int)cam_pos.x, (int)cam_pos.y, (int)cam_res.x, (int)cam_res.y}; //чини
		const SDL_Point center = {(int)(rotopos_local.x), (int)(rotopos_local.y)};
		
		SDL_RenderCopyEx(rd, rnd_tar, NULL, NULL, (double)roto, &center, SDL_FLIP_NONE);
	}
	//--
	
	void RenderSurface::SetColorMod(Color clr){
		SDL_SetTextureColorMod(rnd_tar, clr.r, clr.g, clr.b);
		SDL_SetTextureAlphaMod(rnd_tar, clr.a);
	}
	
	void RenderSurface::SetBlendMode(SDL_BlendMode blend){
		SDL_SetTextureBlendMode(rnd_tar, blend);
	}
	
	void RenderSurface::SetCirclePrecision(uint16_t prec){
		circ_precision = prec;
	}
}

namespace cbdraw{ //функции для рисования
	void RenderSurface::ApplyColor(Color cl){
		SDL_SetRenderDrawColor(rd, cl.r, cl.g, cl.b, cl.a);
		SDL_SetTextureAlphaMod(rnd_tar, cl.a);
	}
	
	void RenderSurface::DrawPolygon(Color clr, std::vector<cbpp::Vec2> verts){
		std::size_t sz = verts.size();
		
		if(sz <= 0){ return; }
		
		int16_t* vx = new int16_t[sz];
		int16_t* vy = new int16_t[sz];
		
		if(vx == nullptr || vy == nullptr){ return; }
		
		for(std::size_t i = 0; i < sz; i++){
			Vec2 tmp = verts[i];
			vx[i] = (int16_t)tmp.x;
			vy[i] = (int16_t)tmp.y;
		}
		
		polygonRGBA(rd, vx, vy, sz, clr.r, clr.g, clr.b, clr.a);
		
		delete[] vx, vy;
	}
	
	void RenderSurface::DrawFilledPolygon(Color clr, std::vector<cbpp::Vec2> verts){
		std::size_t sz = verts.size();
		
		if(sz <= 0){ return; }
		
		int16_t* vx = new int16_t[sz];
		int16_t* vy = new int16_t[sz];
		
		if(vx == nullptr || vy == nullptr){ return; }
		
		for(std::size_t i = 0; i < sz; i++){
			Vec2 tmp = verts[i];
			vx[i] = (int16_t)tmp.x;
			vy[i] = (int16_t)tmp.y;
		}
		
		filledPolygonRGBA(rd, vx, vy, sz, clr.r, clr.g, clr.b, clr.a); //кто-то решил, что будет удобно передавать в функцию два
																	   //массива с x и y координатами точек (это не удобно)
		delete[] vx, vy;
	}
	
	void RenderSurface::DrawLine(Color clr, Vec2 pos1, Vec2 pos2, uint8_t width){		
		Vec2 offset = (pos1-pos2).Norm();
		offset.Set(-offset.y, offset.x); // (X,Y)->(-Y,X) дает поворот на 90, находим нормаль к прямой
		
		std::vector tmp = {pos1+offset*width*0.5f, pos1-offset*width*0.5f, pos2-offset*width*0.5f, pos2+offset*width*0.5f};
		
		//рисуем смешной многогранник (thickLineRGBA рисует какое-то непотребство)
		DrawFilledPolygon(clr, tmp);
	}
	
	void RenderSurface::DrawCircle(Color clr, Vec2 pos, uint16_t radius, uint8_t width = 1){
		float step = (6.28)/circ_precision;
		float ang;
		
		for(int i = 0; i < circ_precision; i++){
			ang = i*step;
			
			Vec2 p1 = Vec2(cosf(ang), sinf(ang))*radius + pos;
			Vec2 p2 = Vec2(cosf(ang - step), sinf(ang - step))*radius + pos;
			
			DrawLine(clr, p1, p2, width);
		}
	}
	
	void RenderSurface::DrawMesh(Color clr, Vec2 pos, Mesh* mesh, uint8_t width = 1){
		for(std::size_t i = 0; i < mesh->Size()-1; i+=1){
			DrawLine(clr, pos + mesh->At(i), pos + mesh->At(i+1), width);
		}
		DrawLine(clr, pos + mesh->At(mesh->Size()-1), pos + mesh->At(0), width);
	}
	
	void RenderSurface::DrawRect(Color clr, Vec2 p1, Vec2 p2, uint8_t width){
		std::vector<Vec2> points = {p1, Vec2(p2.x, p1.y), p2, Vec2(p1.x, p2.y)};
		
		for(std::size_t i = 0; i < 3; i++){
			DrawLine(clr, points[i], points[i+1], width);
		}
		DrawLine(clr, points[0], points[3], width);
	}
	
	void RenderSurface::DrawFilledRect(Color clr, Vec2 p1, Vec2 p2){
		boxRGBA(rd, (int16_t)p1.x, (int16_t)p1.y, (int16_t)p2.x, (int16_t)p2.y, clr.r, clr.g, clr.b, clr.a);
	}
	
	void RenderSurface::DrawImage(SDL_Texture* tex, cbpp::Vec2 pos, cbpp::Vec2 size, float roto){
		const SDL_Rect dst = {(int)pos.x, (int)pos.y, (int)size.x, (int)size.y};
		
		if(tex != NULL){
			SDL_RenderCopyEx(rd, tex, NULL, &dst, (double)roto, NULL, SDL_FLIP_NONE);
		}
	}
	
	void RenderSurface::DrawText(Color clr, cbasset::Font* font, std::wstring text, cbpp::Vec2 pos2, cbpp::Vec2 size, int xpad = 5, int ypad = 10){
		
	}
	
	void RenderSurface::DrawVector(Color clr, cbpp::Vec2 pos, cbpp::Vec2 vector){
		DrawCircle(clr, pos, 1);
		DrawLine(clr, pos, pos + vector,1);
	}
}

namespace cbdraw{
	SDL_Surface* RenderText(cbasset::Font* font, std::wstring text, float scale, int padx, int pady, bool debug){
		SDL_Surface* result;

		if(font == nullptr || text.length() == 0){ return result; }
		
		int max_height = -1;
		
		for(std::size_t i = 0; i < text.length(); i++){
			auto ldata = font->fontdata[(uint16_t)text[i]];
			if(ldata.h > max_height){
				max_height = ldata.h;
			}
		}
		
		int size_x = 0, size_y = max_height;
		
		for(std::size_t i = 0; i < text.length(); i++){
			auto ldata = font->fontdata[(uint16_t)text[i]];
			
			size_x = size_x + (float)(ldata.w + padx)*scale;
			if(text[i] == '\n'){
				size_y = size_y + (float)(max_height + pady)*scale;
			}
		}
		
		result = SDL_CreateRGBSurface(0, size_x, size_y, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

		int x = 0, y = 0;
		SDL_Renderer* rd = cbutil::GetRender();
		
		if(rd == nullptr){ return result; }
		
		for(std::size_t i = 0; i < text.length(); i++){
			auto ldata = font->fontdata[(uint16_t)text[i]];
			auto char_tex = font->fontmap.GetImage((uint16_t)text[i]);
			
			SDL_Rect char_rect = {0, 0, (int)((float)ldata.w*scale), (int)((float)ldata.h*scale)};
			SDL_Rect dst_rect = {x, y, (int)((float)ldata.w*scale), (int)((float)ldata.h*scale)};
			
			//std::cout<<dst_rect.x<<" "<<dst_rect.y<<" "<<dst_rect.w<<" "<<dst_rect.h<<std::endl;

			//if(debug){
			SDL_FillRect(result, &dst_rect, 0);
			//}
			
			if(char_tex != nullptr && result != nullptr){
				//SDL_BlitSurface(char_tex, &char_rect, result, &dst_rect);
			}

			x = x + (float)(ldata.w + padx)*scale;
			if(text[i] == '\n'){
				y = y + (float)(max_height + pady)*scale;
				x = 0;
			}
		}
		
		return result;
	}
}