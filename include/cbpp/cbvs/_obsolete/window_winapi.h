#ifndef CBVS_WINDOW_H
#define CBVS_WINDOW_H

#include <windows.h>
#include <cstdint>
#include <map>

#include "cbpp/vec2.h"

namespace cbvs {
	static const wchar_t CBPP_WINDOWCLASS[] = L"CBPP_WNDCLASS";
	static HINSTANCE CBPP_WINAPI_INSTANCE;
	static int CBPP_WINAPI_nCmdShow;
	
	extern "C" void RegisterInstance(HINSTANCE& inst);
	extern "C" void RegisterShowType(int tp);
	
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	extern "C" BOOL RegisterWindowClass();
	
	extern "C" class Window {
		public:
			Window();
			Window(const wchar_t* title);
			Window(cbpp::Vec2 pos, cbpp::Vec2 size, const wchar_t* title);
			Window(cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, const wchar_t* title);
			Window(cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, DWORD style_ex, const wchar_t* title);
			
			Window(Window& parent, cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, const wchar_t* title);
			Window(Window& parent, cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, DWORD style_ex, const wchar_t* title);
			
			BOOL Show(int showtype);
			MSG Update();
			
			void Bind(uint32_t message, void(*hook)(Window&, MSG));
			void UnBind(uint32_t message);
			BOOL IsBound(uint32_t message);
			
			HWND& GetHandler();
			
		private:
			wchar_t* wname = nullptr;
			
			std::map<uint32_t, void(*)(Window&, MSG)> bindmap;
			
			HWND hwnd;
			HWND* parent_hwnd = nullptr;
	};
}

#endif