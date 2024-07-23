#include "cbvs/window.h"
#include <cstring>

using namespace cbpp;

namespace cbvs {
	extern "C" void RegisterInstance(HINSTANCE& inst){
		CBPP_WINAPI_INSTANCE = inst;
	}
	
	extern "C" void RegisterShowType(int tp){
		CBPP_WINAPI_nCmdShow = tp;
	}
	
	LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam) {
		switch (Message) {
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProcW(hwnd, Message, wparam, lparam);
		}
		
		return 0;
	}
	
	BOOL RegisterWindowClass(){
		WNDCLASSW wc = {};
		
		wc.lpfnWndProc = WndProc;
		wc.hInstance = CBPP_WINAPI_INSTANCE;
		wc.lpszClassName = CBPP_WINDOWCLASS;
		
		return (RegisterClassW(&wc) != 0);
	}
	
	HWND& Window::GetHandler(){ return hwnd; }
	
	MSG Window::Update(){
		MSG msg;
		
		GetMessage(&msg, hwnd, 0, 0);
		uint32_t msg_code = msg.message;
		if(IsBound(msg_code)){
			bindmap[msg_code](*this, msg);
		}
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		return msg;
	}
	
	Window::Window(Window& parent, cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, DWORD style_ex, const wchar_t* title){
		wname = new wchar_t[wcslen(title)];
		wcscpy(wname, title);
		
		hwnd = CreateWindowExW(
			style_ex,
			CBPP_WINDOWCLASS,
			wname,
			style,
			
			(int)pos.x,
			(int)pos.y,
			(int)size.x,
			(int)size.y,
			
			parent.GetHandler(),
			NULL,
			CBPP_WINAPI_INSTANCE,
			NULL
		);
		
		ShowWindow(hwnd, CBPP_WINAPI_nCmdShow);
	}
	
	Window::Window(Window& parent, cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, const wchar_t* title){
		Window(parent, pos, size, style, 0, title);
	}
	
	Window::Window(cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, DWORD style_ex, const wchar_t* title){		
		wname = new wchar_t[wcslen(title)];
		wcscpy(wname, title);
		
		hwnd = CreateWindowExW(
			style_ex,
			CBPP_WINDOWCLASS,
			wname,
			style,
			
			(int)pos.x,
			(int)pos.y,
			(int)size.x,
			(int)size.y,
			
			NULL,
			NULL,
			CBPP_WINAPI_INSTANCE,
			NULL
		);
		
		ShowWindow(hwnd, CBPP_WINAPI_nCmdShow);
	}
	
	Window::Window(cbpp::Vec2 pos, cbpp::Vec2 size, DWORD style, const wchar_t* title){
		Window(pos, size, style, 0, title);
	}
	
	Window::Window(cbpp::Vec2 pos, cbpp::Vec2 size, const wchar_t* title){
		Window(pos, size, WS_OVERLAPPEDWINDOW, title);
	}
	
	Window::Window(const wchar_t* title){
		Window(Vec2(100), Vec2(250), WS_OVERLAPPEDWINDOW, title);
	}
	
	Window::Window(){
		Window(L"WINDOW");
	}
	
	BOOL Window::IsBound(uint32_t msg){
		for(auto it = bindmap.begin(); it != bindmap.end(); it++){
			uint32_t tmp = it->first;
			
			if(tmp == msg){
				return TRUE;
			}
		}
		
		return FALSE;
	}
	
	void Window::Bind(uint32_t msg, void (*hook_ptr)(Window&, MSG)){
		bindmap[msg] = hook_ptr;
	}
	
	void Window::UnBind(uint32_t msg){
		if(IsBound(msg)){
			bindmap.erase(msg);
		}
	}
}